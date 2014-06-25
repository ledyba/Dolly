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
#include <limits>

/**
 * Bスプライン
 */

template <int N>
constexpr double bsprineBasis(const double (&knot)[N], int j, int n, double t)
{
	return (n == 0)
			? ( (knot[j] <= t && t < knot[j+1]) ? 1.0 : 0.0 )
			: ((t-knot[j]) / (knot[j+n]-knot[j]) *bsprineBasis(knot, j, n-1, t) +
			  (knot[j+n+1]-t)/(knot[j+n+1] - knot[j+1])*bsprineBasis(knot, j+1, n-1, t));
}

template <int N, int M>
Vector4 calcBSprine(const double (&knot)[N], const Vector4 (&points)[M], int n, double t)
{
	Vector4 r;
	for(int i=0;i<M;++i) {
		r = r + bsprineBasis(knot, i, n, t) * points[i];
	}
	return std::move(r);
}

void drawBSprine(SuperPX& px, std::unique_ptr<dolly::Camera> const& cam)
{
	const static double knotA[10] = {-5,-4,-3,-2,-1,1,2,3,4,5};
	const static double knotB[10] = {-8, -7, -3, -2, -1,0,1,2,3,4};
	const static Vector4 points[5] = {
			Vector4(-1, -1, 2, 1),
			Vector4(-1,  1, 2, 1),
			Vector4( 0, -1, 2, 1),
			Vector4( 1,  1, 2, 1),
			Vector4( 1, -1, 2, 1)
	};
	SuperPX::SaveMatrix save_(px);
	for( int i=0;i<30*30;++i ) {
		Color white(1,1,1,1);
		Color red  (1,0,0,1);
		Color blue (0,0,1,1);
		px.clear();
		// rotate
		px.move(0, 0, -2);
		px.rotate(0.5f , 1 , 0.25f, 1.0f/180*3.14f);
		px.move(0, 0, +2);
		// draw
		px.move(0, 0, +4);
		{
			px.line(Vector4(-1, -1, 2, 1), Vector4( 1, -1, 2, 1), white);
			px.line(Vector4( 1, -1, 2, 1), Vector4( 1,  1, 2, 1), white);
			px.line(Vector4( 1,  1, 2, 1), Vector4(-1,  1, 2, 1), white);
			px.line(Vector4(-1,  1, 2, 1), Vector4(-1, -1, 2, 1), white);
			for( int j=0;j <4; ++j ) {
				px.line(points[j],points[j+1], white);
			}
			Vector4 lastA = calcBSprine(knotA, points, 3, knotA[0] + 0.01);
			Vector4 lastB = calcBSprine(knotB, points, 3, knotB[0] + 0.01);
			for( int j=0;j <8; ++j ) {
				for(int t_=1;t_<100;++t_){
					Vector4 nowA = calcBSprine(knotA, points, 3, knotA[j] + (t_ * (knotA[j+1]-knotA[j]) / 100.0));
					Vector4 nowB = calcBSprine(knotB, points, 3, knotB[j] + (t_ * (knotB[j+1]-knotB[j]) / 100.0));
					px.line(lastA, nowA, red);
					px.line(lastB, nowB, blue);
					lastA = nowA;
					lastB = nowB;
				}
			}
		}
		px.move(0, 0, -4);
		cam->shot();
	}
}

/**
 * ベジエ曲線
 */

class Bazier {
	const Vector4 a_;
	const Vector4 b_;
	const Vector4 c_;
public:
	Vector4 const& a() const { return a_; };
	Vector4 const& b() const { return b_; };
	Vector4 const& c() const { return c_; };
	Bazier(Vector4 const& a, Vector4 const& b, Vector4 const& c)
	:a_(a), b_(b), c_(c){}
	std::tuple<Bazier, Bazier> divideAt(double const& t) const{
		Vector4 d1 = (1-t)*a_ + t*b_;
		Vector4 d2 = (1-t)*b_ + t*c_;
		Vector4 e = (1-t)*d1 + t*d2;
		return std::tuple<Bazier, Bazier>(Bazier(a_,d1,e), Bazier(e,d2,c_));
	}
	Vector4 at(double const& t) {
		Vector4 d1 = (1-t)*a_ + t*b_;
		Vector4 d2 = (1-t)*b_ + t*c_;
		return (1-t)*d1 + t*d2;
	}
	bool cross( Bazier const& o ) {
		return true;
	}
};

/**
 * クーンズ曲面
 */

Vector4 hermite(double const t) {
	return Vector4((2*t+1)*(1-t)*(1-t), t*(1-t)*(1-t), -t*t*(1-t), t*t*(3-2*t));
}

Vector4 calcCoons(const Vector4 (&params)[4][4], double const u_, double const v_)
{
	Vector4 u(hermite((u_)));
	Vector4 v(hermite((v_)));
	Vector4 t[4];
	for( int i=0;i<4;++i ) {
		t[i] = Vector4();
		for( int j=0;j<4;++j ) {
			t[i] += u(j) * params[j][i];
		}
	}
	Vector4 r;
	for( int i=0;i<4;++i ) {
		r += t[i] * v(i);
	}
	return std::move(r);
}

void drawCoons(SuperPX& px, std::unique_ptr<dolly::Camera> const& cam)
{
	Vector4 params[4][4] = {
			{Vector4(-1,-1,2,1),Vector4(),Vector4(),Vector4(-1,1,2,1)},
			{Vector4(),         Vector4(),Vector4(),Vector4()},
			{Vector4(),         Vector4(),Vector4(),Vector4()},
			{Vector4(1,-1,2,1), Vector4(),Vector4(),Vector4(1,1,2,1)}
	};
	srand(10);
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j) {
			if(!( (i ==0 || i == 3) && (j ==0 || j == 3) ) ) {
				Vector4 r((rand() % 1000)-500,(rand() % 1000)-500,(rand() % 1000)-500, 0);
				r /= r.length();
				params[i][j] = r;
			}
		}
	SuperPX::SaveMatrix save_(px);
	for( int i=0;i<30*30;++i ) {
		Color white(1,1,1,1);
		Color red  (1,0,0,0.5);
		Color blue (0,0,1,0.5);
		px.clear();
		// rotate
		px.move(0, 0, -2);
		px.rotate(0.5f , 1 , 0.25f, 1.0f/180*3.14f);
		px.move(0, 0, +2);
		// draw
		px.move(0, 0, +4);
		static const int N = 30;
		{
			px.line(Vector4(-1, -1, 2, 1), Vector4( 1, -1, 2, 1), white);
			px.line(Vector4( 1, -1, 2, 1), Vector4( 1,  1, 2, 1), white);
			px.line(Vector4( 1,  1, 2, 1), Vector4(-1,  1, 2, 1), white);
			px.line(Vector4(-1,  1, 2, 1), Vector4(-1, -1, 2, 1), white);
			for( int u=0;u <= N; ++u ) {
				Vector4 last = calcCoons(params, (double)u/N, 0.0);
				for(int v=1; v<=N;++v){
					Vector4 now = calcCoons(params, (double)u/N, (double)v/N);
					px.line(last, now, red);
					last = now;
				}
			}
			for( int v=0;v <= N; ++v ) {
				Vector4 last = calcCoons(params, 0.0, (double)v/N);
				for(int u=1; u<=N;++u){
					Vector4 now = calcCoons(params, (double)u/N, (double)v/N);
					px.line(last, now, blue);
					last = now;
				}
			}
		}
		px.move(0, 0, -4);
		cam->shot();
	}
}

int main(int argc, char** argv)
{
	using namespace dolly;
	CameraBuilder b(640,480,"test.mp4");
	std::unique_ptr<Camera> cam(b.build());
	SuperPX px(cam->cairo(), cam->surface());
	px.clear();
	px.frustum(-1,1,1,-1,2,10);
	drawCoons(px, cam);
	drawBSprine(px, cam);
	return 0;
}
