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
#include <random>
#include "matrix.h"

typedef dolly::Matrix<4,4,double> Matrix;
typedef dolly::Vector<4,double> Vector;

class Star final {
private:
	double r_,g_,b_;
	Vector xs_;
	Vector x_;
	constexpr const static double G = 300;
	constexpr const static double dt = 0.01;
public:
	Star(const double x, const double y, const double u, const double v, const double r,const double g,const double b)
	:xs_(x,y,u,v),x_(),r_(r),g_(g),b_(b){
	}
public:
	double x() const { return xs_(0); }
	double y() const { return xs_(1); }
public:
	Vector next(Vector v, std::vector<Star*> const& stars)
	{
		double fx = 0;
		double fy = 0;
		for(auto it : stars) {
			if(it == this) {
				continue;
			}
			const double dx = it->x() - v(0);
			const double dy = it->y() - v(1);
			const double r = sqrt(dx*dx + dy*dy);
			const double r3 = r*r*r;
			fx += G * dx / r3;
			fy += G * dy / r3;
		}
		return Vector(v(2), v(3), fx, fy);
	}
	void move(std::vector<Star*> const& stars) {
		auto k1 = next(xs_, stars);
		auto k2 = next(xs_ + (k1 * (dt / 2.0)), stars);
		auto k3 = next(xs_ + (k2 * (dt / 2.0)), stars);
		auto k4 = next(xs_ + (k3*dt), stars);
		x_ = xs_ + ((k1 + k2*2 + k3*2 + k4) * (dt / 6.0));
 	}
	void update()
	{
		xs_ = x_;
	}
	void draw(cairo_t * cr)
	{
		cairo_set_source_rgba(cr, r_, g_, b_, 1);
		cairo_arc(cr, 320+x(), 240+y(), 2, 0, 360);
		cairo_fill(cr);
	}
};
class System final {
private:
	std::vector<Star*> stars_;
	cairo_t* cr_;
public:
	System(cairo_t* cr, int const n):cr_(cr){
		const double pi = 3.14;//atan(1)*4;
		for(int i=0;i<n;++i){
			stars_.push_back(
					new Star(
							cos(2*i*pi/n)*100,
							sin(2*i*pi/n)*100,
							1 * sin(2*i*pi/n),
							1 * cos(2*i*pi/n),
							fabs(cos(2*i*pi/n)),
							fabs(sin(2*i*pi/n)),
							fabs((cos(2*i*pi/n)+sin(2*i*pi/n))/2)
							));
		}
	}
	~System() {
		for(auto it : stars_) {
			delete it;
		}
	};
public:
	void moveStars() {
		for(int i=0;i<15;++i) {
			for(auto it : stars_) {
				it->move(stars_);
			}
			for(auto it : stars_) {
				it->update();
			}
		}
	}
	void drawStars() {
		for(auto it : stars_) {
			it->draw(cr_);
		}
	}
};

int main(int argc, char** argv)
{
	std::printf("Start rendering\n");
	std::flush(std::cout);
	{
		using namespace dolly;
		CameraBuilder b(640,480,"sim_test.mp4");
		std::unique_ptr<Camera> cam ( b.build() );
		cairo_set_source_rgba(cam->cairo(), 0, 0, 0, 1);
		cairo_paint(cam->cairo());
		System system(cam->cairo(), 3);
		for( int i=0;i<5000;++i ) {
			std::printf("Rendering frame: %d\n", i);
			std::flush(std::cout);
			cairo_set_source_rgba(cam->cairo(), 0, 0, 0, 1);
			//cairo_paint(cam->cairo());
			system.moveStars();
			system.drawStars();
			cam->shot();
		}
	}
	return 0;
}
