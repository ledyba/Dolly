/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#pragma once

#include <string>
#include <functional>
#include <cairo/cairo.h>
#include "FFPtr.h"

namespace dolly {

class RecorderBuilder final {

};

class Recorder final {

};

class Camera final {
	const int width_;
	const int height_;
private: // ffmpeg
	FFFormatContext fmt_;
	FFCodecContext codec_;
	AVStream* vstr_;
	FFFrame vframe_;
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
