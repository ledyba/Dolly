/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once
#define __STDC_CONSTANT_MACROS 1
#define __STDC_LIMIT_MACROS 1
#ifdef _STDINT_H
#undef _STDINT_H
#include <limits.h>
#include <cstdint>
#endif
extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "Ptr.h"

namespace dolly {
typedef Ptr<AVCodecContext> FFCodecContext;
typedef Ptr<AVFrame> FFFrame;
typedef Ptr<AVFormatContext> FFFormatContext;
typedef Ptr<SwsContext> FFSwsContext;
}

