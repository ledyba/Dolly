/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>

int main(int argc, char** argv)
{
	{
		using namespace dolly;
		CameraBuilder b(640,480,"word_rotation.mp4");
		std::unique_ptr<Camera> cam(b.build());
		cairo_set_source_rgba(cam->cairo(), 1, 1, 1, 1);
		cairo_paint(cam->cairo());
		cairo_text_extents_t ext;
		cairo_set_font_size (cam->cairo(), 64.0);
		cairo_text_extents(cam->cairo(), "Hello dolly", &ext);
		cairo_translate(cam->cairo(), 320, 240);
		for( int i=0;i<7*30;++i ) {
			cairo_set_source_rgba(cam->cairo(), 1, 1, 1, 1);
			cairo_paint(cam->cairo());
			cairo_save(cam->cairo());
			{
				cairo_rotate(cam->cairo(), i/M_PI/10);
				cairo_translate(cam->cairo(), (-ext.x_advance)/2, -80);
				cairo_set_source_rgba(cam->cairo(), 0, 0, 0, 1);
				cairo_text_path(cam->cairo(), "Hello dolly");
				cairo_fill(cam->cairo());
			}
			cairo_restore(cam->cairo());
			cam->shot();
		}
	}
	return 0;
}
