/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#pragma once

#include <string>
#include <functional>
extern "C" {
#include <libavformat/avformat.h>
}
#include <cairo/cairo.h>

namespace waltz {

class Camera final {
	const int width_;
	const int height_;
private: // ffmpeg
	AVFormatContext* fmt_;
	AVCodecContext* codec_;
	AVStream* vstr_;
	AVFrame* vframe_;
private: //cairo
	cairo_surface_t* cairo_surf_;
	cairo_t* cairo_;
private:
	int frame_count_;
public:
	Camera(int width, int height);
	~Camera();
	void start(std::string const& fname, enum AVCodecID videoCodec = AV_CODEC_ID_NONE);
private:
	void openVideo(std::string const& fname, enum AVCodecID videoCodec);
	void finishVideo();
public:
	void record();
	inline cairo_t* cairo() { return this->cairo_; };
};
}
