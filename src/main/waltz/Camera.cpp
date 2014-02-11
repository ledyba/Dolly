/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#include "Camera.h"
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
}
#include <cstdlib>
#include <cinamo/String.h>
#include <cstring>
namespace waltz {
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

Camera::Camera(int width, int height, std::string const& filename, std::string const& mime)
:width_(width)
,height_(height)
,vstr_(nullptr)
,vframe_(nullptr)
,cairo_surf_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height))
,cairo_(cairo_create(cairo_surf_))
,frame_count_(0)
{
}


void Camera::start(std::string const& fname)
{
	this->frame_count_ = 0;
	this->openVideo(fname);
	avformat_write_header(fmt_, nullptr);
	av_dump_format(fmt_, 0, fname.c_str(), 1);
}

void Camera::openVideo(std::string const& fname)
{
	int error;
	avformat_alloc_output_context2(&this->fmt_, NULL, NULL, fname.c_str());
	if (!this->fmt_) {
		throw std::logic_error("Could not allocate output format context\n");
	}

	std::copy(fname.begin(), fname.begin() + std::min(sizeof(fmt_->filename), fname.size()), fmt_->filename);

	/** Find the encoder to be used by its name. */
	AVCodec *output_codec;
	if (!(output_codec = avcodec_find_encoder(fmt_->oformat->video_codec))) {
		throw std::logic_error("Could not find an encoder.\n");
	}

	/** Create a new audio stream in the output file container. */
	AVStream* stream;
	if (!(stream = avformat_new_stream(fmt_, output_codec))) {
		throw std::logic_error("Could not create new stream\n");
	}
	this->vstr_ = stream;
	stream->id = fmt_->nb_streams-1;

	this->codec_ = stream->codec;

	/**
	 * Set the basic encoder parameters.
	 * The input file's sample rate is used to avoid a sample rate conversion.
	 */
	codec_->bit_rate = 400000;
	/* resolution must be a multiple of two */
	codec_->width = this->width_;
	codec_->height = this->height_;
	/* frames per second */
	codec_->time_base = AVRational { 1, 30 };
	codec_->gop_size = 12; /* emit one intra frame every twelve frames at most */
	codec_->pix_fmt = AV_PIX_FMT_YUV420P;
	if (codec_->codec_id == AV_CODEC_ID_MPEG2VIDEO)
	{
		/* just for testing, we also add B frames */
		codec_->max_b_frames = 2;
	}
	if (codec_->codec_id == AV_CODEC_ID_MPEG1VIDEO)
	{
		/* Needed to avoid using macroblocks in which some coeffs overflow.
		 * This does not happen with normal video, it just happens here as
		 * the motion of the chroma plane does not match the luma plane. */
		codec_->mb_decision = 2;
	}

	/**
	 * Some container formats (like MP4) require global headers to be present
	 * Mark the encoder so that it behaves accordingly.
	 */
	if (fmt_->oformat->flags & AVFMT_GLOBALHEADER) {
		codec_->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	/** Open the encoder for the audio stream to use it later. */
	if ((error = avcodec_open2(codec_, output_codec, nullptr)) < 0)
	{
		throw std::logic_error(cinamo::format("Could not open output codec (error '%s')\n", get_error_text(error)));
	}
	if (!(fmt_->oformat->flags & AVFMT_NOFILE)) {
		if ((error = avio_open(&fmt_->pb, fname.c_str(), AVIO_FLAG_WRITE)) < 0) {
			std::string msg( cinamo::format("Could not open '%s': %s\n", fname.c_str(), get_error_text(error)) );
			throw std::logic_error(msg);
		}
	}

	this->vframe_ = av_frame_alloc();
	this->vframe_ -> format = stream->codec->pix_fmt;
	this->vframe_ -> width = width_;
	this->vframe_ -> height = height_;
	/* allocate the encoded raw picture */
	if ( av_image_alloc(vframe_->data, vframe_->linesize, codec_->width, codec_->height, codec_->pix_fmt, 32) < 0) {
		throw std::logic_error(cinamo::format("Could not allocate raw picture buffer\n"));
	}
}

static void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height)
{
	int x, y, i;
		i = frame_index;
		/* Y */
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
		/* Cb and Cr */
	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
		{
			pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
			pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
		}
	}
}

void Camera::record()
{
	if (fmt_->oformat->flags & AVFMT_RAWPICTURE) {
		/* raw video case. The API will change slightly in the near
		   futur for that */
		AVPacket pkt;
		av_init_packet(&pkt);

		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index= vstr_->index;
		pkt.data= (uint8_t *) vframe_;
		pkt.size= sizeof(AVPicture);

		if ( av_write_frame(fmt_, &pkt) < 0 ){
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
		vframe_->pts = frame_count_;
		fill_yuv_image(vframe_, frame_count_, width_, height_);
		if( avcodec_encode_video2(this->codec_, &pkt, vframe_, &gotOutput) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to encode video");
		}
		if (gotOutput) {
			pkt.pts = av_rescale_q_rnd(pkt.pts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.dts = av_rescale_q_rnd(pkt.dts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.duration = av_rescale_q(pkt.duration, vstr_->codec->time_base, vstr_->time_base);
			pkt.stream_index = vstr_->index;

			av_interleaved_write_frame(fmt_, &pkt);
			av_free_packet(&pkt);
		}
	}
	++frame_count_;
}

void Camera::finishVideo(){
	for( int gotOut = 1; gotOut; ) {
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;
		if( avcodec_encode_video2(this->codec_, &pkt, nullptr, &gotOut) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to encode video");
		}
		if (gotOut) {
			pkt.pts = av_rescale_q_rnd(pkt.pts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.dts = av_rescale_q_rnd(pkt.dts, vstr_->codec->time_base, vstr_->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			pkt.duration = av_rescale_q(pkt.duration, vstr_->codec->time_base, vstr_->time_base);
			pkt.stream_index = vstr_->index;

			av_interleaved_write_frame(fmt_, &pkt);
			av_free_packet(&pkt);
		}
	}
}

Camera::~Camera()
{
	finishVideo();
	av_write_trailer(fmt_);
	avcodec_close(codec_);
	av_frame_clone(vframe_);
	av_free(vframe_->data[0]);
	avformat_free_context(fmt_);
}


}
