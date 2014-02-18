/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>

int main(int argc, char** argv)
{
	{
		using namespace dolly;
		RecorderBuilder b(640,480,"test.mp4");
		Recorder r = b.build();
		cairo_set_source_rgba(r.cairo(), 1, 1, 1, 1);
		cairo_paint(r.cairo());
		cairo_text_extents_t ext;
		cairo_set_font_size (r.cairo(), 64.0);
		cairo_text_extents(r.cairo(), "Hello dolly", &ext);
		cairo_translate(r.cairo(), 320, 240);
		for( int i=0;i<7*30;++i ) {
			cairo_set_source_rgba(r.cairo(), 1, 1, 1, 1);
			cairo_paint(r.cairo());
			cairo_save(r.cairo());
			{
				cairo_rotate(r.cairo(), i/M_PI/10);
				cairo_translate(r.cairo(), (-ext.x_advance)/2, -80);
				cairo_set_source_rgba(r.cairo(), 0, 0, 0, 1);
				cairo_text_path(r.cairo(), "Hello dolly");
				cairo_fill(r.cairo());
			}
			cairo_restore(r.cairo());
			r.shot();
		}
	}
	return 0;
}
