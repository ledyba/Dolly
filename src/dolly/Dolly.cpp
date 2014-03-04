/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */

#include <dolly/Dolly.h>
#include <dolly/Util.h>
#include "Window.h"
#include "Film.h"
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
	CairoSurface surface( cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width_, height_) );
	Cairo cairo( cairo_create(surface.get()) );
	std::unique_ptr<Film> film;
	if( !dryRun_ ) {
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
			codec->bit_rate = this->bitrate_;
			/* resolution must be a multiple of two */
			codec->width = this->width_;
			codec->height = this->height_;
			/* frames per second */
			codec->time_base = AVRational { this->frameRateDen_, this->frameRateNum_ };
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
		film = std::unique_ptr<Film>(new Film(
				filename_,
				width_, height_,
				std::move(sws),
				std::move(fmt),
				vstr,
				codec,
				std::move(vframe)));
	}
	std::unique_ptr<Window> window;
	if( showWindow_ ) {
		window = std::move(createWindow(width_, height_));
	}
	return std::unique_ptr<Camera>(new Camera(
		std::move(window),
		std::move(cairo),
		std::move(surface),
		std::move(film)
	));
}

Camera::Camera(
	std::unique_ptr<Window>&& window,
	Cairo&& cairo,
	CairoSurface&& surface,
	std::unique_ptr<Film>&& film
)
:window_(std::move(window))
,cairo_(std::move(cairo))
,surface_(std::move(surface))
,film_(std::move(film))
{
}

void Camera::shot()
{
	if(this->window_){
		window_->showFrame(surface_.get());
		if(window_->handleWindowEvent(surface_.get())){
			std::unique_ptr<Window>().swap(window_);
		}
	}
	if( this->film_ ) {
		this->film_->shot(this->surface_.get());
	}
}

Camera::~Camera()
{
}

}

