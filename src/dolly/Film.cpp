/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2014, PSI
 */
#include <Film.h>
#include <cairo/cairo.h>
#include "../config.h"
#ifdef HAVE_LIBAV
extern "C" {
#include <libavutil/mathematics.h>
}
#define AV_ROUND_PASS_MINMAX 0
#endif
namespace dolly {

Film::Film(
	std::string const& filename,
	const int width,
	const int height,
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
,sws_(std::move(sws))
,fmt_(std::move(fmt))
,vstr_(vstr)
,codec_(codec)
,vframe_(std::move(vframe))
{
	avformat_write_header(fmt_.get(), nullptr);
	av_dump_format(fmt_.get(), 0, filename.c_str(), 1);
}

void Film::shot(cairo_surface_t* surf)
{
	{
		uint8_t * src[4] = { cairo_image_surface_get_data(surf), nullptr, nullptr, nullptr };
		int stride[4] = { cairo_image_surface_get_stride(surf), 0, 0, 0 };
		sws_scale(sws_.get(), src, stride, 0, vframe_->height, vframe_->data, vframe_->linesize);
	}
	{ // encode
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

Film::~Film() noexcept
{
	for( int gotOut = 1; gotOut; ) {
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.data = nullptr;    // packet data will be allocated by the encoder
		pkt.size = 0;
		if( avcodec_encode_video2(vstr_->codec, &pkt, nullptr, &gotOut) < 0 ){
			av_free_packet(&pkt);
			//throw std::logic_error("Failed to encode video");
			return;
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
