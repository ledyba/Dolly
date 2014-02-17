/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#include <dolly/CairoPtr.h>

namespace dolly
{

template <>
void ptrCloser(cairo_t* ptr)
{
	cairo_destroy(ptr);
}

template <>
void ptrCloser(cairo_surface_t* ptr)
{
	cairo_surface_destroy(ptr);
}

}
