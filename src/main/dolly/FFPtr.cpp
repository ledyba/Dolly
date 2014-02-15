/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#include "FFPtr.h"

namespace dolly
{

template <>
void ffCloser(AVFrame* ptr)
{
	av_free(ptr->data[0]);
	av_frame_free(&ptr);
}

template <>
void ffCloser(AVCodecContext* ptr)
{
	avcodec_close(ptr);
	av_free(ptr);
}
template <>
void ffCloser(AVFormatContext* ptr)
{
	avformat_free_context(ptr);
}

}
