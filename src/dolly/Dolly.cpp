/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */

#include <dolly/Dolly.h>
#include <dolly/Util.h>
extern "C" {
#include <libswscale/swscale.h>
}

namespace dolly {
class CameraInit final{
public:
	CameraInit() {
		av_register_all();
	}
	~CameraInit() = default;
};
static CameraInit init_;

/**
 * Convert an error code into a text message.
 * @param error Error code to be converted
 * @return Corresponding error text (not thread-safe)
 */
static char * const get_error_text(const int error)
{
	static char error_buffer[255];
	av_strerror(error, error_buffer, sizeof(error_buffer));
	return error_buffer;
}

std::unique_ptr<Camera> CameraBuilder::build()
{
	FFFormatContext fmt;
	AVStream* vstr;
	FFFrame vframe;
	AVCodecContext* codec;
	{
		int error;
		AVFormatContext* fmtCtx;
		avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, filename_.c_str());
		fmt.set(fmtCtx);
		if (!fmt) {
			throw std::logic_error("Could not allocate output format context\n");
		}

		std::copy(filename_.begin(), filename_.begin() + std::min(sizeof(fmt->filename), filename_.size()), fmt->filename);

		/** Find the encoder to be used by its name. */
		AVCodec *output_codec;
		if (!(output_codec = avcodec_find_encoder(videoCodec_ == AV_CODEC_ID_NONE ? fmt->oformat->video_codec : videoCodec_))) {
			throw std::logic_error("Could not find an encoder.\n");
		}

		/** Create a new audio stream in the output file container. */
		AVStream* stream;
		if (!(stream = avformat_new_stream(fmt.get(), output_codec))) {
			throw std::logic_error("Could not create new stream\n");
		}
		vstr = stream;
		stream->id = fmt->nb_streams-1;

		codec = vstr->codec;

		/**
		 * Set the basic encoder parameters.
		 * The input file's sample rate is used to avoid a sample rate conversion.
		 */
		codec->bit_rate = 400000;
		/* resolution must be a multiple of two */
		codec->width = this->width_;
		codec->height = this->height_;
		/* frames per second */
		codec->time_base = AVRational { 1, 30 };
		codec->gop_size = 12; /* emit one intra frame every twelve frames at most */
		codec->pix_fmt = AV_PIX_FMT_YUV420P;
		if (codec->codec_id == AV_CODEC_ID_MPEG2VIDEO)
		{
			/* just for testing, we also add B frames */
			codec->max_b_frames = 2;
		}
		if (codec->codec_id == AV_CODEC_ID_MPEG1VIDEO)
		{
			/* Needed to avoid using macroblocks in which some coeffs overflow.
			 * This does not happen with normal video, it just happens here as
			 * the motion of the chroma plane does not match the luma plane. */
			codec->mb_decision = 2;
		}

		/**
		 * Some container formats (like MP4) require global headers to be present
		 * Mark the encoder so that it behaves accordingly.
		 */
		if (fmt->oformat->flags & AVFMT_GLOBALHEADER) {
			codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}

		/** Open the encoder for the audio stream to use it later. */
		if ((error = avcodec_open2(codec, output_codec, nullptr)) < 0)
		{
			throw std::logic_error(format("Could not open output codec (error '%s')\n", get_error_text(error)));
		}
		if (!(fmt->oformat->flags & AVFMT_NOFILE)) {
			if ((error = avio_open(&fmt->pb, filename_.c_str(), AVIO_FLAG_WRITE)) < 0) {
				std::string msg( format("Could not open '%s': %s\n", filename_.c_str(), get_error_text(error)) );
				throw std::logic_error(msg);
			}
		}

		vframe.set(av_frame_alloc());
		vframe -> format = stream->codec->pix_fmt;
		vframe -> width = width_;
		vframe -> height = height_;
		/* allocate the encoded raw picture */
		if ( av_image_alloc(vframe->data, vframe->linesize, codec->width, codec->height, codec->pix_fmt, 32) < 0) {
			throw std::logic_error(format("Could not allocate raw picture buffer\n"));
		}
	}
	FFSwsContext sws;
	{
		sws.set(sws_getContext(width_,height_, PIX_FMT_BGRA, width_, height_, codec->pix_fmt, 0, nullptr, nullptr, nullptr));
	}
	CairoSurface surface( cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width_, height_) );
	Cairo cairo( cairo_create(surface.get()) );
	return std::unique_ptr<Camera>(new Camera(
		filename_,
		width_,
		height_,
		std::move(cairo),
		std::move(surface),
		std::move(sws),
		std::move(fmt),
		vstr,
		codec,
		std::move(vframe)
	));
}

Camera::Camera(
	std::string const& filename,
	const int width,
	const int height,
	Cairo&& cairo,
	CairoSurface&& surface,
	FFSwsContext&& sws,
	FFFormatContext&& fmt,
	AVStream* vstr,
	AVCodecContext* codec,
	FFFrame&& vframe
)
:frameCount_(0)
,filename_(filename)
,width_(width)
,height_(height)
,cairo_(std::move(cairo))
,surface_(std::move(surface))
,sws_(std::move(sws))
,fmt_(std::move(fmt))
,vstr_(vstr)
,codec_(codec)
,vframe_(std::move(vframe))
{
	avformat_write_header(fmt_.get(), nullptr);
	av_dump_format(fmt_.get(), 0, filename.c_str(), 1);
}

void Camera::shot()
{
	{
		uint8_t * src[4] = { cairo_image_surface_get_data(surface_.get()), nullptr, nullptr, nullptr };
		int stride[4] = { cairo_image_surface_get_stride(surface_.get()), 0, 0, 0 };
		sws_scale(sws_.get(), src, stride, 0, vframe_->height, vframe_->data, vframe_->linesize);
	}
	if (fmt_->oformat->flags & AVFMT_RAWPICTURE) {
		/* raw video case. The API will change slightly in the near
		   futur for that */
		AVPacket pkt;
		av_init_packet(&pkt);

		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index= vstr_->index;
		pkt.data= (uint8_t *) vframe_.get();
		pkt.size= sizeof(AVPicture);

		if ( av_write_frame(fmt_.get(), &pkt) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to write frame");
		}
		av_free_packet(&pkt);
	} else {
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;
		/* encode the image */
		int gotOutput;
		vframe_->pts = frameCount_;
		if( avcodec_encode_video2(vstr_->codec, &pkt, vframe_.get(), &gotOutput) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to encode video");
		}
		if (gotOutput) {
			pkt.pts = av_rescale_q_rnd(pkt.pts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.dts = av_rescale_q_rnd(pkt.dts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.duration = av_rescale_q(pkt.duration, vstr_->codec->time_base, vstr_->time_base);
			pkt.stream_index = vstr_->index;

			av_interleaved_write_frame(fmt_.get(), &pkt);
			av_free_packet(&pkt);
		}
	}
	++frameCount_;
}

Camera::~Camera()
{
	try {
		this->closeVideo();
	} catch (std::exception& e) {
	}
}
void Camera::closeVideo()
{
	for( int gotOut = 1; gotOut; ) {
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.data = nullptr;    // packet data will be allocated by the encoder
		pkt.size = 0;
		if( avcodec_encode_video2(vstr_->codec, &pkt, nullptr, &gotOut) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to encode video");
		}
		if (gotOut) {
			pkt.pts = av_rescale_q_rnd(pkt.pts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.dts = av_rescale_q_rnd(pkt.dts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.duration = av_rescale_q(pkt.duration, vstr_->codec->time_base, vstr_->time_base);
			pkt.stream_index = vstr_->index;

			av_interleaved_write_frame(fmt_.get(), &pkt);
			av_free_packet(&pkt);
		}
	}
	av_write_trailer(fmt_.get());
}


}

