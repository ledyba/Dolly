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
#include <vector>

struct World {
	double th1;
	double th2;
	double th1a;
	double th2a;
};

const static double m1 = 1;
const static double m2 = 1;
const static double l1 = 1;
const static double l2 = 1;
const static double g = 9.8;
const static double h = 1.0/3000;

double th1(const double th1a){
	return th1a;
}
double th2(const double th2a){
	return th2a;
}
double th1a(const double th1, const double th2, const double th1a, const double th2a){
	const double C = cos(th1-th2);
	const double S = sin(th1-th2);
	const double m = m1+m2;
	return (-m2*l1*l2*S*m2*l2*l2*th2a*th2a
			-m2*m2*l1*l1*l2*l2*C*S*th1a*th1a
			-m*g*l1*sin(th1)*m2*l2*l2
			+m2*m2*g*l1*l2*l2*C*sin(th2))/
			(m*l1*l1*m2*l2*l2-m2*m2*l1*l1*l2*l2*C*C);
}

double th2a(const double th1, const double th2, const double th1a, const double th2a){
	const double C = cos(th1-th2);
	const double S = sin(th1-th2);
	const double m = m1+m2;
	return (-m2*m2*l1*l1*l2*l2*C*S*th2a*th2a
			-m*l1*l1*m2*l1*l2*S*th1a*th1a
			-m*m2*g*l1*l1*l2*C*sin(th1)
			+m*l1*l1*m2*g*l2*sin(th2))
			/
			(m2*m2*l1*l1*l2*l2*C*C-m*l1*l1*m2*l2*l2);
}

void update(World& w)
{
	const double k1_th1  = th1(w.th1a);
	const double k1_th2  = th2(w.th2a);
	const double k1_th1a = th1a(w.th1, w.th2, w.th1a, w.th2a);
	const double k1_th2a = th2a(w.th1, w.th2, w.th1a, w.th2a);

	const double k2_th1 = th1(w.th1a+h*k1_th1a);
	const double k2_th2 = th2(w.th2a+h*k1_th2a);
	const double k2_th1a = th1a(w.th1+h*k1_th1/2, w.th2+h*k1_th2/2, w.th1a+h*k1_th1a/2, w.th2a+h*k1_th2a/2);
	const double k2_th2a = th2a(w.th1+h*k1_th1/2, w.th2+h*k1_th2/2, w.th1a+h*k1_th1a/2, w.th2a+h*k1_th2a/2);

	const double k3_th1 = th1(w.th1a+0.5*h*k2_th1a);
	const double k3_th2 = th2(w.th2a+0.5*h*k2_th2a);
	const double k3_th1a = th1a(w.th1+h*k2_th1/2, w.th2+h*k2_th2/2, w.th1a+h*k2_th1a/2, w.th2a+h*k2_th2a/2);
	const double k3_th2a = th2a(w.th1+h*k2_th1/2, w.th2+h*k2_th2/2, w.th1a+h*k2_th1a/2, w.th2a+h*k2_th2a/2);

	const double k4_th1 = th1(w.th1a+h*k3_th1a);
	const double k4_th2 = th2(w.th2a+h*k3_th2a);
	const double k4_th1a = th1a(w.th1+h*k3_th1, w.th2+h*k3_th2, w.th1a+h*k3_th1a, w.th2a+h*k3_th2a);
	const double k4_th2a = th2a(w.th1+h*k3_th1, w.th2+h*k3_th2, w.th1a+h*k3_th1a, w.th2a+h*k3_th2a);

	w.th1 = w.th1 + (k1_th1 + 2*k2_th1 + 2*k3_th1 + k3_th1)*h/6;
	w.th2 = w.th2 + (k1_th2 + 2*k2_th2 + 2*k3_th2 + k3_th2)*h/6;
	w.th1a = w.th1a + (k1_th1a + 2*k2_th1a + 2*k3_th1a + k3_th1a)*h/6;
	w.th2a = w.th2a + (k1_th2a + 2*k2_th2a + 2*k3_th2a + k3_th2a)*h/6;
}

void renderOne(cairo_t* cr, World& w, int const width, int const height)
{
	const double cw = width/2.0;
	const double ch = height/2.0;
	const double line = cw/3;
	for(int i = 0; i<10; ++i){
		for(int j = 0; j<10; ++j){
			update(w);
		}
		cairo_set_source_rgba(cr, 0, 0, 0, 0.07);
		cairo_paint(cr);

		const double x1 = cw + line * sin(w.th1);
		const double y1 = ch + line * cos(w.th1);

		const double x2 = cw + line * sin(w.th1) + line * sin(w.th2);
		const double y2 = ch + line * cos(w.th1) + line * cos(w.th2);

		cairo_move_to(cr, cw, ch);
		cairo_line_to(cr, x1, y1);
		cairo_set_source_rgba(cr, 1, 0.6, 0.6, 1);
		cairo_stroke(cr);

		cairo_move_to(cr, x1, y1);
		cairo_line_to(cr, x2, y2);
		cairo_set_source_rgba(cr, 0.6, 1, 0.6, 1);
		cairo_stroke(cr);
	}
}

int main(int argc, char** argv)
{
	std::printf("Start rendering\n");
	std::flush(std::cout);
	World world;
	// 最初はまっすぐ
	world.th1 = -M_PI;
	world.th2 = -M_PI;
	world.th1a = 0;
	world.th2a = -0.001;
	{
		using namespace dolly;
		CameraBuilder b(640,480,"sim_test.mp4");
		std::unique_ptr<Camera> cam ( b.build() );
		cairo_set_source_rgba(cam->cairo(), 0, 0, 0, 1);
		cairo_paint(cam->cairo());
		cairo_set_font_size (cam->cairo(), 64.0);
		for( int i=0;i<18000;++i ) {
			std::printf("Rendering frame: %d\n", i);
			std::flush(std::cout);
			renderOne(cam->cairo(), world, 640, 480);
			cam->shot();
		}
	}
	return 0;
}
