/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2014, PSI
 */
#pragma once
#include "FFPtr.h"
#include <cairo/cairo.h>

namespace dolly
{
class Film final {
private:
	int frameCount_;
	const std::string filename_;
	const int width_;
	const int height_;
private:
	FFSwsContext sws_;
	FFFormatContext fmt_;
	AVStream* vstr_;
	AVCodecContext* codec_;
	FFFrame vframe_;
	friend class CameraBuilder;
private:
	Film(
		std::string const& filename,
		const int width,
		const int height,
		FFSwsContext&& sws,
		FFFormatContext&& fmt,
		AVStream* vstr,
		AVCodecContext* codec,
		FFFrame&& vframe
	);
public:
	~Film() noexcept;
	void shot(cairo_surface_t* surf);
};

}
