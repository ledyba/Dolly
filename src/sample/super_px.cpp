/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include "super_px.h"

/***********************************************************************
 *  Matrix4
 ***********************************************************************/

Matrix4::Matrix4():val_(new float[16]){
	for( int i=0;i<16;++i ) {
		val_[i] = 0.0f;
	}
}

Matrix4::Matrix4(
		float const a11, float const a12, float const a13, float const a14,
		float const a21, float const a22, float const a23, float const a24,
		float const a31, float const a32, float const a33, float const a34,
		float const a41, float const a42, float const a43, float const a44
):val_(new float[16]){
	(*this)(0,0) = a11; (*this)(0,1) = a12; (*this)(0,2) = a13; (*this)(0,3) = a14;
	(*this)(1,0) = a21; (*this)(1,1) = a22; (*this)(1,2) = a23; (*this)(1,3) = a24;
	(*this)(2,0) = a31; (*this)(2,1) = a32; (*this)(2,2) = a33; (*this)(2,3) = a34;
	(*this)(3,0) = a41; (*this)(3,1) = a42; (*this)(3,2) = a43; (*this)(3,3) = a44;
}

Matrix4 Matrix4::zero(){
	return std::move(Matrix4());
}
Matrix4 Matrix4::ident(){
	Matrix4 m;
	for( int i=0;i < 4; ++i ) {
		m(i,i) = 1;
	}
	return std::move(m);
}

Matrix4& Matrix4::toIdent(){
	for( int i=0;i < 4; ++i ) {
		(*this)(i,i) = 1;
	}
	return *this;
}
Matrix4& Matrix4::toZero(){
	for( int i=0;i<16;++i ) {
		val_[i] = 0.0f;
	}
	return *this;
}
Matrix4& Matrix4::toTrans(){
	for( int i=0;i<3;++i ) {
		for( int j=0;j<3;++j ) {
			std::swap((*this)(i,j), (*this)(j,i));
		}
	}
	return *this;
}
Matrix4 Matrix4::trans() const{
	Matrix4 m;
	for( int i=0;i<3;++i ) {
		for( int j=0;j<3;++j ) {
			m(i,j) = (*this)(j,i);
		}
	}
	return std::move(m);
}
Matrix4::Matrix4(float (val)[16]):val_(new float[16]){
	std::copy(val, val+16, val_);
}
Matrix4::Matrix4(Matrix4 const& o):val_(new float[16]){
	std::copy(o.val_, o.val_+16, val_);
}

Matrix4::Matrix4(Matrix4&& o):val_(o.val_){
	o.val_ = nullptr;
}
Matrix4& Matrix4::operator=(Matrix4 const& o){
	std::copy(o.val_, o.val_+16, val_);
	return *this;
}
Matrix4& Matrix4::operator=(Matrix4&& o) {
	this->val_ = o.val_;
	o.val_ = nullptr;
	return *this;
}
Matrix4::~Matrix4(){
	if(this->val_){
		delete [] this->val_;
		this->val_ = nullptr;
	}
}

Matrix4 Matrix4::operator*(float const f) const{
	float v[16];
	for( int i=0;i<16;++i ) {
		v[i] = val_[i] * f;
	}
	return std::move(Matrix4(v));
}
Matrix4 Matrix4::operator/(float const f) const{
	float v[16];
	for( int i=0;i<16;++i ) {
		v[i] = val_[i] / f;
	}
	return std::move(Matrix4(v));
}
Matrix4 Matrix4::operator*(Matrix4 const& m) const{
	Matrix4 mat;
	for( int i=0;i<4;++i ) {
		for( int j=0;j<4;++j ) {
			float s = 0;
			for( int k=0;k<4;++k ) {
				s += (*this)(i,k) * m(k,j);
			}
			mat(i,j) = s;
		}
	}
	return std::move(mat);
}
Matrix4& Matrix4::operator*=(Matrix4 const& m){
	return ((*this) = std::move( (*this) * m ));
}
Matrix4& Matrix4::operator*=(float const f){
	for( int i=0;i<16;++i ) {
		val_[i] *= f;
	}
	return *this;
}
Matrix4& Matrix4::operator/=(float const f){
	for( int i=0;i<16;++i ) {
		val_[i] /= f;
	}
	return *this;
}

Matrix4 operator*(float f, Matrix4 const& v){
	return std::move(v * f);
}

/***********************************************************************
 *  Vector4
 ***********************************************************************/
Vector4::Vector4(){
	v_[0]=
	v_[1]=
	v_[2]=
	v_[3]=0;
}
Vector4::Vector4(float const x,float const y,float const z,float const w)
{
	v_[0]=x;
	v_[1]=y;
	v_[2]=z;
	v_[3]=w;
}
Vector4::Vector4(float const (&v)[4])
{
	v_[0] = v[0];
	v_[1] = v[1];
	v_[2] = v[2];
	v_[3] = v[3];
}
Vector4::Vector4(Vector4 const& o){
	v_[0] = o.v_[0];
	v_[1] = o.v_[1];
	v_[2] = o.v_[2];
	v_[3] = o.v_[3];
}
Vector4& Vector4::operator=(Vector4 const& o){
	v_[0] = o.v_[0];
	v_[1] = o.v_[1];
	v_[2] = o.v_[2];
	v_[3] = o.v_[3];
	return *this;
}
Vector4 Vector4::operator*(float const f) const{
	Vector4 v(*this);
	for(int i=0;i<4;++i){
		v(i) *= f;
	}
	return std::move(v);
}
Vector4& Vector4::operator*=(float const f) {
	for(int i=0;i<4;++i){
		(*this)(i) *= f;
	}
	return *this;
}
Vector4 Vector4::operator/(float const f) const{
	Vector4 v(*this);
	for(int i=0;i<4;++i){
		v(i) /= f;
	}
	return std::move(v);
}
Vector4& Vector4::operator/=(float const f) {
	for(int i=0;i<4;++i){
		(*this)(i) *= f;
	}
	return *this;
}
Vector4 Vector4::operator-() const{
	Vector4 vec;
	for(int i=0;i<4;++i){
		vec(i) = -(*this)(i);
	}
	return std::move(vec);
}
Vector4 Vector4::operator+(Vector4 const& o){
	Vector4 vec;
	for(int i=0;i<4;++i){
		vec(i) = (*this)(i) + o(i);
	}
	return std::move(vec);
}
Vector4 Vector4::operator-(Vector4 const& o){
	Vector4 vec;
	for(int i=0;i<4;++i){
		vec(i) = (*this)(i) - o(i);
	}
	return std::move(vec);
}
Vector4& Vector4::operator+=(Vector4 const& o){
	for(int i=0;i<4;++i){
		(*this)(i) += o(i);
	}
	return *this;
}
Vector4& Vector4::operator-=(Vector4 const& o){
	for(int i=0;i<4;++i){
		(*this)(i) -= o(i);
	}
	return *this;
}

float Vector4::dot(Vector4 const& v){
	float s = 0;
	for(int i=0;i<4;++i){
		s += v(i) * (*this)(i);
	}
	return s;
}

Vector4 operator*(Matrix4 const& mat, Vector4 const& vec)
{
	float v[4];
	for( int i=0; i<4;++i ) {
		float s = 0;
		for( int j=0; j<4;++j ) {
			s += mat(i,j) * vec(j);
		}
		v[i] = s;
	}
	return std::move(Vector4(v));
}

Vector4 operator*(float const f, Vector4 const& vec)
{
	return std::move(vec * f);
}

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
		x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
		y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0,
		x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c, 0,
		0, 0, 0, 1) * this->model_  );
}

