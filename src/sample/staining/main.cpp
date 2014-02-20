/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include "../main/sha256.h"
#include "../main/dolly/Dolly.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>

int main(int argc, char** argv)
{
	{
		using namespace dolly;
		CameraBuilder b(640,480,"test.mp4");
		Camerar r = b.build();
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
	using namespace clr;
	Field out[8];
	Field in[100*3] = {
	};
	int idx=0;
	for( auto& i : in ) {
		i = Field(0x00000000,
				idx %3 == 0 ? Color(1,0,0) :
				idx %3 == 1 ? Color(0,1,0) : Color(0,0,1) );
		++idx;
	}
	std::printf("length: %lu\n", sizeof(in)/sizeof(Field));
	sha2(in, out);
	for( int i=0;i<8;++i ){
		Color c = out[i].color();
		std::printf("%08x -> %.2f,%.2f,%.2f",
				out[i].value(), c.red(), c.green(), c.blue());
		std::puts("");
	}

	return 0;
}
