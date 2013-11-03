/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#include "Camera.h"
#include <cstdlib>
namespace waltz {
class CameraInit final{
public:
	CameraInit() {
		av_register_all();
	}
	~CameraInit() = default;
};
static CameraInit init_;

Camera::Camera(int width, int height, std::string const& filename, std::string const& mime)
:width_(width)
,height_(height)
,fmt_(av_guess_format(nullptr, filename.c_str(), mime.size() > 0 ? mime.c_str() : nullptr))
,ctx_(nullptr)
,vstr_(nullptr)
,vframe_(nullptr)
,pict_()
,cairo_surf_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height))
,cairo_(cairo_create(cairo_surf_))
,frame_count_(0)
{
	if( !fmt_ ) {
		throw std::logic_error("Failed to guess output format...");
	}
	int r = avformat_alloc_output_context2(&ctx_, fmt_, nullptr, filename.c_str());
	if( r < 0 ) {
		throw std::logic_error("Failed to alloc output context...");
	}
}

void Camera::start()
{
	this->frame_count_ = 0;
	this->addVideoStream( fmt_->video_codec );
	this->openVideo();
	//avformat_write_header(ctx_, nullptr);
}

void Camera::addVideoStream(enum AVCodecID codec_id)
{
	AVStream *st = avformat_new_stream(ctx_, 0);
	if( !st ) {
		throw std::logic_error("Failed to alloc video stream!");
	}
	AVCodecContext* codec = st->codec;
	codec->codec_id = codec_id;
	codec->bit_rate = 400000;
	codec->width = width_;
	codec->height = height_;
	codec->time_base = AVRational { 1,30 };
	codec->codec_type = AVMEDIA_TYPE_VIDEO;
	codec->pix_fmt = AV_PIX_FMT_YUV420P;
	//codec->gop_size = 12; /* emit one intra frame every twelve frames at most */
	 if (codec->codec_id == CODEC_ID_MPEG2VIDEO) {
		/* just for testing, we also add B frames */
		 codec->max_b_frames = 2;
	 }
	if (codec->codec_id == CODEC_ID_MPEG1VIDEO){
		/* needed to avoid using macroblocks in which some coeffs overflow
	 	 this doesnt happen with normal video, it just happens here as the
	 	 motion of the chroma plane doesnt match the luma plane */
		codec->mb_decision=2;
	}
	// some formats want stream headers to be seperate
	if(
		ctx_->oformat->name == std::string("mp4") ||
		ctx_->oformat->name == std::string("mov") ||
		ctx_->oformat->name == std::string("3gp")) {
		codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	this->vstr_ = st;
}

void Camera::openVideo()
{
	AVCodec *codec;

	/* find the video encoder */
	codec = avcodec_find_encoder(vstr_->codec->codec_id);
	if (!codec) {
		throw std::logic_error("could find encode");
	}

	/* open the codec */
	int r = avcodec_open2(vstr_->codec, codec, nullptr);
	if (r < 0) {
		throw std::logic_error("could not open codec\n");
	}

	/* allocate the encoded raw picture */
	if ( avpicture_alloc(&this->pict_, vstr_->codec->pix_fmt, vstr_->codec->width, vstr_->codec->height) < 0) {
		fprintf(stderr, "Could not allocate picture\n");
		exit(1);
	}
}

void Camera::record()
{
	if (ctx_->oformat->flags & AVFMT_RAWPICTURE) {
		/* raw video case. The API will change slightly in the near
		   futur for that */
		AVPacket pkt;
		av_init_packet(&pkt);

		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index= vstr_->index;
		pkt.data= (uint8_t *) vframe_;
		pkt.size= sizeof(AVPicture);

		if ( av_write_frame(ctx_, &pkt) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to write frame");
		}
		av_free_packet(&pkt);
	} else {
		AVPacket pkt;
		av_init_packet(&pkt);
		/* encode the image */
		int isEmpty;
		if( avcodec_encode_video2(vstr_->codec, &pkt, vframe_, &isEmpty) < 0 ){
			av_free_packet(&pkt);
			throw std::logic_error("Failed to encode video");
		}
		/* if zero size, it means the image was buffered */
		if (isEmpty == 1) {
			pkt.pts= vstr_->codec->coded_frame->pts;
			if(vstr_->codec->coded_frame->key_frame)
				pkt.flags |= AV_PKT_FLAG_KEY;
			pkt.stream_index= vstr_->index;

			/* write the compressed frame in the media file */
			if ( av_write_frame(ctx_, &pkt) < 0 ){
				av_free_packet(&pkt);
				throw std::logic_error("Failed to write frame");
			}
			av_free_packet(&pkt);
		}
	}
	++frame_count_;
}

Camera::~Camera()
{
	//av_write_trailer(ctx_);
	avformat_free_context(ctx_);
}


}
