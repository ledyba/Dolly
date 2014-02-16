/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once

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

