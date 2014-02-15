/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
#include <libavformat/avformat.h>
}

#include "FFPtr.h"

namespace dolly {

class RecorderBuilder final {
private:
	int width_;
	int height_;
	std::string filename_;
	enum AVCodecID videoCodec_;
public:
	RecorderBuilder(const int width, const int height, std::string const& filename)
	:width_(width)
	,height_(height)
	,filename_(filename)
	,videoCodec_(AV_CODEC_ID_NONE)
	{
	}
#define SET_(TYPE, NAME) \
	inline RecorderBuilder& NAME(TYPE const& val) {\
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
#undef GET_SET_
#undef GET_
#undef SET_
};

class Recorder final {

};

}

