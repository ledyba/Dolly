/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <cmath>
#include "super_px.h"

/***********************************************************************
 *  SuperPX
 ***********************************************************************/

SuperPX::SuperPX(cairo_t* const& cr, cairo_surface_t* surf)
:cr_(cr)
,surf_(surf)
,clearColor_(0,0,0,1)
,proj_(Matrix4::ident())
,model_(Matrix4::ident())
{
}
Color const& SuperPX::clearColor(Color const& c) {
	return this->clearColor_ = c;
}
Color const& SuperPX::clearColor() const {
	return this->clearColor_;
}
void SuperPX::pushMatrix()
{
	this->savedMatrix_.push_back( this->model_ );
}
void SuperPX::popMatrix()
{
	this->model_ = this->savedMatrix_.back();
	this->savedMatrix_.pop_back();
}

void SuperPX::color(Color const& c){
	cairo_set_source_rgba(cr_, c.r(), c.g(), c.b(), c.a());
}
void SuperPX::clear(){
	color(clearColor_);
	cairo_paint(cr_);
}
void SuperPX::dot(const int x, const int y, Color const& c){
	color(c);
	cairo_rectangle(cr_, x,y, 1, 1);
	cairo_fill(cr_);
}
Vector4 SuperPX::toScreen(Vector4 const& f)
{
	float const w = cairo_image_surface_get_width(surf_)/2;
	float const h = cairo_image_surface_get_height(surf_)/2;
	Vector4 v(f / f.w());
	v.x() *= w;
	v.x() += w;
	v.y() *= h;
	v.y() += h;
	return std::move( v );
}
void SuperPX::line( Vector4 const& from, Vector4 const& to, Color const& c) {
	Vector4 f ( toScreen( proj_ * this->model_ * from ) );
	Vector4 t ( toScreen ( proj_ * this->model_ * to ) );
	color(c);
	cairo_move_to(cr_, f.x() / f.w(), f.y() / f.w());
	cairo_line_to(cr_, t.x() / t.w(), t.y() / t.w());
	cairo_stroke(cr_);
}
void SuperPX::renderTex(int const x, int const y, cairo_surface_t* img, Vector4 const& texPt_)
{
	Vector4 const texPt( texPt_ / texPt_.w() );
	const int texX = static_cast<int>(texPt.x());
	const int texY = static_cast<int>(texPt.y());
	unsigned char const * const dat = cairo_image_surface_get_data(img);
	int const stride = cairo_image_surface_get_stride(img);
	const unsigned int col = *reinterpret_cast<const unsigned int*>( &dat[texY * stride + texX * 4] );
	unsigned int const alphai = (col >> 24) & 0xff;
	float const alpha = alphai == 0 ? 255.0 : alphai;
	Color const c(
			((col & 0xff0000) >> 16) / alpha,
			((col & 0x00ff00) >>  8) / alpha,
			((col & 0x0000ff) >>  0) / alpha, alpha / 255.0);
	dot(x,y,c);
}
void SuperPX::triangle(
	Vector4 const& a_, Vector4 const& b_, Vector4 const& c_,
	cairo_surface_t* img,
	Vector4 const& u_, Vector4 const& v_, Vector4 const& w_)
{
	Vector4 a(proj_ * model_ * a_);
	Vector4 b(proj_ * model_ * b_);
	Vector4 c(proj_ * model_ * c_);
	Vector4 u(u_ / a.w());
	Vector4 v(v_ / b.w());
	Vector4 w(w_ / c.w());
//	Vector4 u(u_);
//	Vector4 v(v_);
//	Vector4 w(w_);
	a = toScreen(a);
	b = toScreen(b);
	c = toScreen(c);
	using std::swap;
	if( a.y() > b.y() ) { swap(a,b); swap(u,v); }
	if( a.y() > c.y() ) { swap(a,c); swap(u,w); }
	if( b.y() > c.y() ) { swap(b,c); swap(v,w); }
	if((b.y() - a.y()) < 1) {
		const bool isAvecLeft = a.x() < b.x();
		const Vector4 lp = (isAvecLeft) ? a : b;
		const Vector4 rp = (isAvecLeft) ? b : a;
		const int startY = round(a.y());
		const Vector4 lpt = (isAvecLeft) ? u : v;
		const Vector4 rpt = (isAvecLeft) ? v : u;
		for( int dy = 0, maxY=round( std::max((c-a).y(), (c-b).y()) ); dy<=maxY; ++dy) {
			const Vector4 l ( (lp * (maxY-dy) + c * dy) / maxY );
			const Vector4 r ( (rp * (maxY-dy) + c * dy) / maxY );
			const Vector4 lt ( (lpt * (maxY-dy) + w * dy) / maxY );
			const Vector4 rt ( (rpt * (maxY-dy) + w * dy) / maxY );
			const int startX = round(l.x());
			for( int dx=0, maxX=round(r.x() - l.x()); dx<=maxX; ++dx ) {
				const Vector4 texPt ( ( (lt * (maxX-dx)) + (rt * dx) ) / maxX );
				renderTex(startX+dx, startY+dy, img, texPt);
			}
		}
	}else{
		const int startY = round(a.y());
		const int maxDY = round(c.y() - a.y());
		const int middleDY = round(b.y() - a.y());
		if( (c.x()-a.x())/maxDY < (b.x()-a.x())/middleDY ) { //cが左
			for( int dy = 0; dy<middleDY; ++dy) {
				const Vector4 l ( (a *    (maxDY-dy) + c * dy) / maxDY );
				const Vector4 r ( (a * (middleDY-dy) + b * dy) / middleDY );
				const Vector4 lt ( (u *    (maxDY-dy) + w * dy) / maxDY );
				const Vector4 rt ( (u * (middleDY-dy) + v * dy) / middleDY );
				const int startX = round(l.x());
				for( int dx=0, maxX=round(r.x() - l.x()); dx<=maxX; ++dx ) {
					const Vector4 texPt ( ( (lt * (maxX-dx)) + (rt * dx) ) / maxX );
					renderTex(startX+dx, startY+dy, img, texPt);
				}
			}
			for( int dy = middleDY; dy<=maxDY; ++dy) {
				const Vector4 l ( (a * (maxDY-dy) + c * dy) / maxDY );
				const Vector4 r ( (b * ((maxDY-middleDY) - (dy-middleDY)) + c * (dy-middleDY)) / (maxDY-middleDY) );
					const Vector4 lt ( (u * (maxDY-dy) + w * dy) / maxDY );
				const Vector4 rt ( (v * ((maxDY-middleDY) - (dy-middleDY)) + w * (dy-middleDY)) / (maxDY-middleDY) );
				const int startX = round(l.x());
				for( int dx=0, maxX=round(r.x() - l.x()); dx<=maxX; ++dx ) {
					const Vector4 texPt ( ( (lt * (maxX-dx)) + (rt * dx) ) / maxX );
					renderTex(startX+dx, startY+dy, img, texPt);
				}
			}
		}else{
			for( int dy = 0; dy<middleDY; ++dy) {
				const Vector4 r ( (a * (maxDY-dy) + c * dy) / maxDY );
				const Vector4 l ( (a * (middleDY-dy) + b * dy) / middleDY );
				const Vector4 rt ( (u * (maxDY-dy) + w * dy) / maxDY );
				const Vector4 lt ( (u * (middleDY-dy) + v * dy) / middleDY );
				const int startX = round(l.x());
				for( int dx=0, maxX=round(r.x() - l.x()); dx<=maxX; ++dx ) {
					const Vector4 texPt ( ( (lt * (maxX-dx)) + (rt * dx) ) / maxX );
					renderTex(startX+dx, startY+dy, img, texPt);
				}
			}
			for( int dy = middleDY; dy<=maxDY; ++dy) {
				const Vector4 r ( (a * (maxDY-dy) + c * dy) / maxDY );
				const Vector4 l ( (b * ((maxDY-middleDY) - (dy-middleDY)) + c * (dy-middleDY)) / (maxDY-middleDY) );

				const Vector4 rt ( (u * (maxDY-dy) + w * dy) / maxDY );
				const Vector4 lt ( (v * ((maxDY-middleDY) - (dy-middleDY)) + w * (dy-middleDY)) / (maxDY-middleDY) );
				const int startX = round(l.x());
				for( int dx=0, maxX=round(r.x() - l.x()); dx<=maxX; ++dx ) {
					const Vector4 texPt ( ( (lt * (maxX-dx)) + (rt * dx) ) / maxX );
					renderTex(startX+dx, startY+dy, img, texPt);
				}
			}
		}
	}
}
void SuperPX::move(float const tx, float const ty, float const tz){
	this->model_ = std::move(Matrix4(
		1,      0,       0,  tx,
		0,      1,       0,  ty,
		0,      0,       1,  tz,
		0,      0,       0,  1
	) * this->model_);
}
void SuperPX::scale(float const sx, float const sy, float const sz){
	this->model_ = std::move( Matrix4(
		sx,     0,       0,  0,
		0,      sy,      0,  0,
		0,      0,       sz, 0,
		0,      0,       0,  1
	) * this->model_ );
}
void SuperPX::frustum(float l,float r,float b,float t,float n,float f){
	const float rl = r-l;
	const float tb = t-b;
	const float fn = f-n;
	this->proj_ = std::move(Matrix4(
		2*n/rl, 0,       (r+l)/rl,  0,
		0,      2*n/tb,  (t+b)/tb,  0,
		0,      0,      -(f+n)/fn,  2*f*n/fn,
		0,      0,      -1,         0
	));
}
void SuperPX::rotate(float x, float y, float z, float angle)
{
	const float c = cos(angle);
	const float s = sin(angle);
	this->model_ = std::move ( Matrix4(
		x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0.0f,
		y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0.0f,
		x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f) * this->model_  );
}


SuperPX::SaveMatrix::SaveMatrix(SuperPX& px):px_(px) {
	px_.pushMatrix();
}
SuperPX::SaveMatrix::~SaveMatrix() noexcept {
	px_.popMatrix();
}

