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
	AVIOContext* ioContex_;
	AVFormatContext* fmt_;
	AVCodecContext* codec_;
	AVStream* vstr_;
	AVFrame* vframe_;
	AVPicture pict_;
private: //cairo
	cairo_surface_t* cairo_surf_;
	cairo_t* cairo_;
private:
	int frame_count_;
public:
	Camera(int width, int height, std::string const& filename, std::string const& mime = "");
	~Camera();
	void start(std::string const& fname);
private:
	void openVideo(std::string const& fname);
public:
	void record();
	inline cairo_t* cairo() { return this->cairo_; };
};
}
