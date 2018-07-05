/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>
#include <cassert>
#include "super_px.h"
#include <libgen.h>

int main(int argc, char** argv)
{
	{

		char dname[ strlen(argv[0]) + 10 ];
		memcpy(dname, argv[0], strlen(argv[0])+1);
		dirname(dname);
		strcat(dname, "/tex.png");
		puts(dname);
		cairo_surface_t* img = cairo_image_surface_create_from_png(dname);

		if( cairo_surface_get_type(img) != CAIRO_SURFACE_TYPE_IMAGE ) {
			std::printf("Unknwon texture image type: %d", cairo_surface_get_type(img));
			return -1;
		}

		using namespace dolly;
		CameraBuilder b(640,480,"3d_texture.mp4");
		std::unique_ptr<Camera> cam(b.build());
		SuperPX px(cam->cairo(), cam->surface());
		px.clear();
		px.frustum(-1,1,1,-1,2,10);
		for( int i=0;i<30*30;++i ) {
			std::printf("frame: %d\n", i+1);
			px.clear();
			Color c(1,1,1,1);
			px.move(0, 0, -2);
			px.rotate(0.5f , 1 , 0.25f, 1.0f/180*3.14f);
			px.move(0, 0, +2);
			px.move(0, 0, +4);
			px.triangle(
					Vector4(-1, -1, 2, 1),
					Vector4(-1,  1, 2, 1),
					Vector4( 1,  1, 2, 1),
					img,
					Vector4(   0,  0, 0, 1),
					Vector4(   0, 512, 0, 1),
					Vector4( 512, 512, 0, 1)
			);
			px.triangle(
					Vector4( 1,  1, 2, 1),
					Vector4( 1, -1, 2, 1),
					Vector4(-1, -1, 2, 1),
					img,
					Vector4( 512, 512, 0, 1),
					Vector4( 512,   0, 0, 1),
					Vector4(   0,   0, 0, 1)
			);
			px.line(Vector4(-1, -1, 2, 1), Vector4( 1, -1, 2, 1), c);
			px.line(Vector4( 1, -1, 2, 1), Vector4( 1,  1, 2, 1), c);
			px.line(Vector4( 1,  1, 2, 1), Vector4(-1,  1, 2, 1), c);
			px.line(Vector4(-1,  1, 2, 1), Vector4(-1, -1, 2, 1), c);
			px.move(0, 0, -4);
			cam->shot();
		}
	}
	return 0;
}
