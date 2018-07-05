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

void setSourceHSV(cairo_t* cr, const double h, const double s, const double v)
{
	const double hh = std::min(h, 360.0);
	const int i = static_cast<int>(hh / 60.0);
	const double left = hh - i;
	const double p = v * (1.0 - s);
	const double q = v * (1.0 - (s * left));
	const double t = v * (1.0 - (s * (1.0 - left)));

	double r, g, b;

	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
	default:
		r = v;
		g = p;
		b = q;
		break;
	}
	cairo_set_source_rgba(cr, r, g, b, 1);
}

void setPix(cairo_t* cr, const int x, const int y, const double f, const int num, const int numMax)
{
	//const double sig = 1/(1+pow(M_E, -(f-50)/50));
	const double sig = (double)num/numMax;
	setSourceHSV(cr, (sig*180),1,sig);
	cairo_rectangle(cr, x,y, 1, 1);
	cairo_fill(cr);
}

void plot(cairo_t* cr, const int max, const int x, const int y, const double px, const double py)
{
	double x_=0,y_=0,x__=0,y__=0;
	for(int i=0;i<max;++i){
		y_ = 2*x_*y_ + py;
		x_ = x__ - y__ + px;
		x__ = x_ * x_;
		y__ = y_ * y_;
		if(x__+y__ >= 16){
			setPix(cr, x,y, x__+y__, i, max);
			return;
		}
	}
}

void renderOne(cairo_t* cr, const int max, const int twidth, const int theight, double centerX, double centerY, double width, double height)
{
	const int thwidth = twidth/2;
	const int thheight = theight/2;
	const float scaleX = width / twidth;
	const float scaleY = height / theight;
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
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
		CameraBuilder b(640,480,"mandelblot.mp4");
		std::unique_ptr<Camera> cam ( b.bitrate(1000*1000*10).build() );
		for( int i=0;i<900;++i ) {
			std::printf("Rendering frame: %d\n", i);
			std::flush(std::cout);
			cairo_set_source_rgba(cam->cairo(), 1, 1, 1, 1);
			cairo_paint(cam->cairo());
			const double m = (pow(1.015, i));
			const double s = 1/m;
			const double cx = -1.40115575 +s;
			renderOne(cam->cairo(), static_cast<int>(pow(i, 2.5)/1000)+300, 640, 480, cx, 0, 2*s, 2*s*3/4);
			cam->shot();
		}
	}
	return 0;
}
