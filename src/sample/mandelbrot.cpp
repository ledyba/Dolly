/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>
#include <cmath>

void setPix(cairo_t* cr, const int x, const int y, float a, float r, float g, float b)
{
	cairo_set_source_rgba(cr, r, g, b, a);
	cairo_rectangle(cr, x,y, 1, 1);
	cairo_fill(cr);
}

void plot(cairo_t* cr, const int max, const int x, const int y, const double px, const double py)
{
	double x_ = 0;
	double y_ = 0;
	double x__ = 0;
	double y__ = 0;
	for(int i=0;i<max;++i){
		y_ = 2*x_*y_ + py;
		if(y_ >= 4){
			return;
		}
		x_ = x__ - y__ + px;
		if(x_ >= 4){
			return;
		}
		x__ = x_ * x_;
		y__ = y_ * y_;
	}
	setPix(cr, x,y, 1,0,0,0);
}

void renderOne(cairo_t* cr, const int max, const int twidth, const int theight, double centerX, double centerY, double width, double height)
{
	const int thwidth = twidth/2;
	const int thheight = theight/2;
	const float scaleX = width / twidth;
	const float scaleY = height / theight;
	cairo_set_source_rgba(cr, 1, 1, 1 ,1);
	cairo_paint(cr);
	for(int x=0;x<twidth; ++x){
		for(int y=0;y<theight;++y){
			const double px = (x-thwidth)*scaleX + centerX;
			const double py = (y-thheight)*scaleY + centerY;
			plot(cr, max, x, y, px, py);
		}
	}
}

int main(int argc, char** argv)
{
	std::printf("Start rendering\n");
	std::flush(std::cout);
	{
		using namespace dolly;
		CameraBuilder b(640,480,"sim_test.mp4");
		std::unique_ptr<Camera> r ( b.bitrate(1000*1000).build() );
		for( int i=0;i<500;++i ) {
			std::printf("Rendering frame: %d\n", i);
			std::flush(std::cout);
			cairo_set_source_rgba(r->cairo(), 1, 1, 1, 1);
			cairo_paint(r->cairo());
			const double m = (pow(1.015, i));
			const double s = 1/m;
			const double cx = -1.401292+s;
			renderOne(r->cairo(), i+400, 640, 480, cx, 0, 2*s, 2*s*3/4);
			r->shot();
		}
	}
	return 0;
}
