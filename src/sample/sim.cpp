/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <SDL2/SDL.h>
#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>

// パラメータの設定（100で割っているのは1/100日ごとに計算するから、刻みが粗いとカオス的挙動が生成することも）
const double r1=0.05/100; // 植物の可食部分（葉を想定）の最大増殖率（サバンナの雨期なら0.1程度としても現実的か）

const double K1=200; // 植物の可食部分の環境収容力(200g/m^2)
const double K2=20; //肉食動物の補食速度の基準値を与える草食動物の量（純粋な環境収容力ではないが、これをどうするかが肉食動物の生存にかなり重要。当面20くらいで）

//const double a2=0.05/100;  // 草食動物のの最大摂食能力(本当は最大0.045くらいか)
//const double a2c=0.05/100; // 食われない草食動物の最大摂食能力
const double a3=0.05/100;  // 肉食動物の基準捕食能力(これが実は最大であり、有効値で最大0.04くらいか)

const double m=0.02/100   ;    //動物の維持呼吸（人間並みを仮定し0.02を基本にする）
const double d1=0.01/100  ;    //植物の自然減少（落葉などを想定する）
const double d2=0.001/100 ;    //草食動物の自然減少（本当のところはわからないが0.001程度か）
const double d2c=0.001/100;    //食われない草食動物の自然減減少を別に決めることにした

const double d3=0.001/100;  // 肉食動物の自然減少（自然死、本当のところはわからないが0.001としている）
const double e=0.5       ;  // 食物の生物体への変換効率（微生物などでもこれは同じで0.5）
const double n1=30       ;  // 植物の成長曲線の形を変える（n1が大きいとKまで急速に成長、現実には30あたりか）
const double n2=3        ;  // n2が小さいと、探索能力が大きくなる（イネ科草原なら探索は簡単なので1を与えておく）
const double n3=1        ;  // n2と同じ（肉食動物はあまり探索が上手ではないようだが、サバンナは見晴らしが良いので1を与えておく）
const double dig2=0.5    ;  // 草食動物の消化効率
const double dig3=0.5    ;  // 肉食動物の消化効率


void renderGraph(cairo_t* cr, const double width, const double height, std::vector<double> const& pts, double r,double g,double b)
{
	auto hcv = [=](double y){
		return height * (1-y/200.0);
	};
	cairo_move_to(cr, 0, height);
	cairo_set_source_rgba(cr, r, g, b, 1);
	for( int i=0;i< pts.size(); ++i) {
		const double pt = pts.at(i);
		cairo_line_to(cr, width*i/pts.size(), hcv(pt));
	}
	cairo_stroke(cr);
}

void renderOne(cairo_t* cr, const double width, const double height, const double a2, const double a2c)
{
	auto f = [=](double x_, double y_, double yc_) {
		return r1*x_*(1-pow((x_/K1),n1)) - a2 * pow((x_/K1), n2)*y_ -d1*x_ - a2c*pow((x_/K1),n2)*yc_;
	};
	auto g = [=](double x_, double y_, double z_) {
		return e*(dig2*a2*pow((x_/K1),n2)*y_-m*y_) - a3*pow((y_/K2),n3)*z_ - d2*y_;
	};
	auto gc = [=](double x_, double yc_) {
		return e*(dig2*a2c*pow((x_/K1),n2)*yc_-m*yc_) - d2c*yc_;
	};
	auto h = [=](double y_, double z_) {
		return e*(dig3*a3*pow((y_/K2), n3)*z_-m*z_) - d3*z_;
	};
	double x0=100;
	double y0=10;
	double yc0=10;
	double z0=1;
	std::vector<double> xs;
	std::vector<double> ys;
	std::vector<double> ycs;
	std::vector<double> zs;
	for(int i=0; i< 500000; ++i){
		if (x0<0.001) x0=0;
		if (y0<0.001) y0=0;
		if (yc0<0.001) yc0=0;
		if (z0<0.001) z0=0;

		double x = x0 + f(x0, y0, yc0);
		double y = y0 + g(x0, y0, z0);
		double yc = yc0 +gc(x0, yc0);
		double z = z0 + h(y0, z0);

		xs.push_back(x);
		ys.push_back(y);
		ycs.push_back(yc);
		zs.push_back(z);

		x0 = x;
		y0 = y;
		yc0 = yc;
		z0 = z;
	}
	renderGraph(cr, width, height, xs, 0, 1, 0);
	renderGraph(cr, width, height, ys, 0, 0, 0);
	renderGraph(cr, width, height, ycs, 0.5, 0.5, 1);
	renderGraph(cr, width, height, zs, 1, 0, 0);
}

int main(int argc, char** argv)
{
	std::printf("Start rendering\n");
	std::flush(std::cout);
	{
		using namespace dolly;
		CameraBuilder b(640,480,"sim_test.mp4");
		std::unique_ptr<Camera> r ( b.build() );
		cairo_set_source_rgba(r->cairo(), 1, 1, 1, 1);
		cairo_paint(r->cairo());
		cairo_set_font_size (r->cairo(), 64.0);
		for( int i=0;i<100;++i ) {
			std::printf("Rendering frame: %d\n", i);
			std::flush(std::cout);
			cairo_set_source_rgba(r->cairo(), 1, 1, 1, 1);
			cairo_paint(r->cairo());
			double const max = 0.18 / 100;
			double const a2 = ((i/100.0) * max) + 0.02/100;
			double const a2c = 0.05/100;
			renderOne(r->cairo(), 640, 480, a2, a2c);
			r->shot();
			r->shot();
			r->shot();
		}
	}
	return 0;
}
