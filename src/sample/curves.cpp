/* coding: utf-8 */
/**
 * Dolly
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
	using namespace dolly;
	CameraBuilder b(640,480,"test.mp4");
	std::unique_ptr<Camera> cam(b.build());
	SuperPX px(cam->cairo(), cam->surface());
	px.clear();
	px.frustum(-1,1,1,-1,2,10);
	for( int i=0;i<30*30;++i ) {
		std::printf("frame: %d\n", i+1);
		px.clear();
		Color c(1,1,1,1);
		// rotate
		px.move(0, 0, -2);
		px.rotate(0.5f , 1 , 0.25f, 1.0f/180*3.14f);
		px.move(0, 0, +2);
		// draw
		px.move(0, 0, +4);
		{
			px.line(Vector4(-1, -1, 2, 1), Vector4( 1, -1, 2, 1), c);
			px.line(Vector4( 1, -1, 2, 1), Vector4( 1,  1, 2, 1), c);
			px.line(Vector4( 1,  1, 2, 1), Vector4(-1,  1, 2, 1), c);
			px.line(Vector4(-1,  1, 2, 1), Vector4(-1, -1, 2, 1), c);
		}
		px.move(0, 0, -4);
		cam->shot();
	}
	return 0;
}
