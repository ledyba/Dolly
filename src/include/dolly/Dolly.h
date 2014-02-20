/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once

#ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS 1
#endif
#ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS 1
#endif

#include <cstdint>
#include <memory>
#include <limits.h>
#ifdef _STDINT_H
#undef _STDINT_H
#endif
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
#include <libavformat/avformat.h>
}

#include "FFPtr.h"
#include "CairoPtr.h"

namespace dolly {

class Camera;

class CameraBuilder final {
private:
	int width_;
	int height_;
	std::string filename_;
	enum AVCodecID videoCodec_;
	int bitrate_;
	int frameRateDen_;
	int frameRateNum_;
public:
	CameraBuilder(const int width, const int height, std::string const& filename)
	:width_(width)
	,height_(height)
	,filename_(filename)
	,videoCodec_(AV_CODEC_ID_NONE)
	,bitrate_(300*1024)
	,frameRateDen_(1)
	,frameRateNum_(30)
	{
	}
#define SET_(TYPE, NAME) \
	inline CameraBuilder& NAME(TYPE const& val) {\
		this->NAME##_ = val;\
		return *this;\
	}
#define GET_(TYPE, NAME)\
	inline TYPE const& NAME() {\
		return this->NAME##_;\
	}
#define GET_SET_(TYPE, NAME) GET_(TYPE,NAME);SET_(TYPE,NAME);
	GET_SET_(int, width);
	GET_SET_(int, height);
	GET_SET_(std::string, filename);
	GET_SET_(enum AVCodecID, videoCodec);
	GET_SET_(int, bitrate);
	CameraBuilder& frameRate(const int den, const int num) { this->frameRateDen_=den; this->frameRateNum_=num; return *this; }
	GET_(int, frameRateDen);
	GET_(int, frameRateNum);
#undef GET_SET_
#undef GET_
#undef SET_
public:
	std::unique_ptr<Camera> build();
};

class Camera final {
private:
	int frameCount_;
private: // generic
	const std::string filename_;
	const int width_;
	const int height_;
private: // cairo
	Cairo cairo_;
	CairoSurface surface_;
private: // connect
	FFSwsContext sws_;
private: // ffmpeg
	FFFormatContext fmt_;
	AVStream* vstr_;
	AVCodecContext* codec_;
	FFFrame vframe_;
private:
	friend class CameraBuilder;
	Camera() = delete;
	Camera(Camera const&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(Camera&&) = delete;
	Camera& operator=(Camera const&) = delete;
	Camera(
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
	);
private:
	void closeVideo();
public:
	~Camera();
public:
	cairo_t* cairo() { return cairo_.get(); }
	cairo_surface_t* surface() { return surface_.get(); }
	void shot();
};

}
