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
	AVOutputFormat* fmt_;
	AVFormatContext* ctx_;
	AVStream* vstr_;
	AVFrame* vframe_;
	AVPicture pict_;
	cairo_surface_t* cairo_surf_;
	cairo_t* cairo_;
private:
	int frame_count_;
public:
	Camera(int width, int height, std::string const& filename, std::string const& mime = "");
	~Camera();
	void start();
private:
	void addVideoStream(enum AVCodecID codec_id);
	void openVideo();
public:
	void record();
	inline cairo_t* cairo() { return this->cairo_; };
};
}
