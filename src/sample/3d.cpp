/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>

class Color final{
	float r_;
	float g_;
	float b_;
	float a_;
public:
	Color(const float r,const float g,const float b, const float a = 1.0f)
	:r_(r), g_(g), b_(b), a_(a){
	}
	Color()
	:r_(0), g_(0), b_(0), a_(0){
	}

	Color(Color const&) = default;
	Color(Color&&) = default;
	Color& operator=(Color const&) = default;
	Color& operator=(Color&&) = default;
	~Color() = default;
	float r() const { return this->a_; }
	float r(float v) { return this->r_ = v; }
	float g() const { return this->g_; }
	float g(float v) { return this->g_ = v; }
	float b() const { return this->b_; }
	float b(float v) { return this->b_ = v; }
	float a() const { return this->a_; }
	float a(float v) { return this->a_ = v; }
};

class Matrix4 final {
	float *val_;
public:
	float operator()(int row, int column) const { return this->val_[row*4 + column]; };
	float& operator()(int row, int column) { return this->val_[row*4 + column]; };
public:
	Matrix4():val_(new float[16]){
		for( int i=0;i<16;++i ) {
			val_[i] = 0.0f;
		}
	}
	Matrix4(
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
	static Matrix4&& zero(){
		return std::move(Matrix4());
	}
	static Matrix4&& ident(){
		Matrix4 m;
		for( int i=0;i < 4; ++i ) {
			m(i,i) = 1;
		}
		return std::move(m);
	}
	Matrix4& toIdent(){
		for( int i=0;i < 4; ++i ) {
			*this(i,i) = 1;
		}
		return *this;
	}
	Matrix4& toZero(){
		for( int i=0;i<16;++i ) {
			val_[i] = 0.0f;
		}
		return *this;
	}
	Matrix4& toTrans(){
		for( int i=0;i<3;++i ) {
			for( int j=0;j<3;++j ) {
				std::swap((*this)(i,j), (*this)(j,i));
			}
		}
		return *this;
	}
	Matrix4&& trans(){
		Matrix4 m;
		for( int i=0;i<3;++i ) {
			for( int j=0;j<3;++j ) {
				m(i,j) = *this(j,i);
			}
		}
		return std::move(m);
	}
	Matrix4(float (val)[16]):val_(new float[16]){
		memcpy(val_, val, sizeof(val_));
	}
	Matrix4(Matrix4 const& o):val_(new float[16]){
		memcpy(val_, o.val_, sizeof(val_));
	}
	Matrix4(Matrix4&& o):val_(o.val_){
		o.val_ = nullptr;
	}
	Matrix4& operator=(Matrix4 const& o){
		memcpy(val_, o.val_, sizeof(val_));
		return *this;
	}
	Matrix4& operator=(Matrix4&& o) {
		this->val_ = o.val_;
		o.val_ = nullptr;
		return *this;
	}
	~Matrix4(){
		if(this->val_){
			delete [] this->val_;
			this->val_ = nullptr;
		}
	}
public:
	Matrix4&& operator*(float const f) const{
		float v[16];
		for( int i=0;i<16;++i ) {
			v[i] = val_[i] * f;
		}
		return std::move(Matrix4(v));
	}
	Matrix4&& operator/(float const f) const{
		float v[16];
		for( int i=0;i<16;++i ) {
			v[i] = val_[i] / f;
		}
		return std::move(Matrix4(v));
	}
	Matrix4&& operator*(Matrix4 const& m) const{
		Matrix4 mat;
		for( int i=0;i<3;++i ) {
			for( int j=0;j<3;++j ) {
				float s = 0;
				for( int k=0;k<3;++k ) {
					s += (*this)(i,k) * m(k,j);
				}
				mat(i,j) = s;
			}
		}
		return std::move(mat);
	}
	Matrix4& operator*=(Matrix4 const& m) const{
		Matrix4 mat;
		for( int i=0;i<3;++i ) {
			for( int j=0;j<3;++j ) {
				float s = 0;
				for( int k=0;k<3;++k ) {
					s += (*this)(i,k) * m(k,j);
				}
				mat(i,j) = s;
			}
		}
		*this = std::move(mat);
		return *this;
	}
	Matrix4& operator*=(float const f){
		for( int i=0;i<16;++i ) {
			val_[i] *= f;
		}
		return *this;
	}
	Matrix4& operator/=(float const f){
		for( int i=0;i<16;++i ) {
			val_[i] /= f;
		}
		return *this;
	}
};

Matrix4&& operator*(float f, Matrix4 const& v){
	return std::move(v * f);
}
class Vector4 final{
	float v_[4];
public:
	float x() const { return this->v_[0]; };
	float y() const { return this->v_[1]; };
	float z() const { return this->v_[2]; };
	float w() const { return this->v_[3]; };
public:
	Vector4(float const x,float const y,float const z,float const w)
	{
		v_[0]=x;
		v_[1]=y;
		v_[2]=z;
		v_[3]=w;
	}
	Vector4(float const (&v)[4])
	{
		memcpy(v_, v, sizeof(v_));
	}
	Vector4(Vector4 const& o){
		memcpy(v_, o.v_, sizeof(v_));
	}
	Vector4(Vector4&& o){
		memcpy(v_, o.v_, sizeof(v_));
	}
	Vector4& operator=(Vector4 const& o){
		memcpy(v_, o.v_, sizeof(v_));
		return *this;
	}
	Vector4& operator=(Vector4&& o){
		memcpy(v_, o.v_, sizeof(v_));
		return *this;
	}
	Vector4&& operator*(float const f) {
		Vector4 v(*this);
		for(int i=0;i<4;++i){
			v(i) *= f;
		}
		return std::move(v);
	}
	Vector4& operator*=(float const f) {
		for(int i=0;i<4;++i){
			*this(i) *= f;
		}
		return *this;
	}
	Vector4&& operator-(){
		Vector4 vec;
		for(int i=0;i<4;++i){
			vec(i) = -*this(i);
		}
		return std::move(vec);
	}
	Vector4&& operator+(Vector4 const& o){
		Vector4 vec;
		for(int i=0;i<4;++i){
			vec(i) = *this(i) + o(i);
		}
		return std::move(vec);
	}
	Vector4&& operator-(Vector4 const& o){
		Vector4 vec;
		for(int i=0;i<4;++i){
			vec(i) = *this(i) - o(i);
		}
		return std::move(vec);
	}
	Vector4& operator+=(Vector4 const& o){
		for(int i=0;i<4;++i){
			*this(i) += o(i);
		}
		return *this;
	}
	Vector4& operator-=(Vector4 const& o){
		for(int i=0;i<4;++i){
			*this(i) -= o(i);
		}
		return *this;
	}
	~Vector4() = default;
	float const& operator()(int i) const{
		return v_[i];
	}
	float const& operator[](int i) const{
		return v_[i];
	}
	float& operator()(int i){
		return v_[i];
	}
	float& operator[](int i){
		return v_[i];
	}
public:
	float dot(Vector4 const& v){
		float s = 0;
		for(int i=0;i<4;++i){
			s += v(i) * *this(i);
		}
		return s;
	}
	Vector4&& cross(Vector4 const& v){
		Vector4 s = 0;
		for(int i=0;i<4;++i){
			s(i) = 0;
		}
		return std::move(s);
	}
};
Vector4&& operator*(Matrix4 const& mat, Vector4 const& vec)
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
Vector4&& operator*(float const f, Vector4 const& vec)
{
	return std::move(vec * f);
}

class SuperPX final{
private:
	cairo_t* cr_;
	Color clearColor_;
private:
	Matrix4 proj_;
public:
	SuperPX(cairo_t* const& cr)
	:cr_(cr)
	{
	}
	~SuperPX() = default;
	SuperPX(SuperPX const&) = delete;
	SuperPX(SuperPX&&) = delete;
	SuperPX& operator=(SuperPX const&) = delete;
	SuperPX& operator=(SuperPX&&) = delete;
public:
	Color const& clearColor(Color const& c) { return this->clearColor_ = c; }
	Color const& clearColor() const { return this->clearColor_; }
public:
	void clear(){
		cairo_set_source_rgba(cr_, clearColor_.r(), clearColor_.g(), clearColor_.b(), clearColor_.a());
		cairo_paint(cr_);
	}
	void dot(const int x, const int y, Color const& c){
		cairo_set_source_rgba(cr_, c.r(), c.g(), c.b(), c.a());
		cairo_rectangle(cr_, x,y, 1, 1);
		cairo_fill(cr_);
	}
	void frustum(float l,float r,float b,float t,float n,float f){
		const float rl = r-l;
		const float tb = t-b;
		const float fn = f-n;
		this->proj_ = std::move(Matrix4(
			2*n/rl, 0,      (r+l)/rl,   0,
			0,      2*n/tb, (t+b)/tb,   0,
			0,      0,      -(f+n)/fn, -2*f*n/fn,
			0,      0,      -1,         0
		));
	}
};

int main(int argc, char** argv)
{
	{
		using namespace dolly;
		CameraBuilder b(640,480,"test.mp4");
		std::unique_ptr<Camera> cam(b.build());
		SuperPX px(cam->cairo());
		px.clear();
		for( int i=0;i<7*30;++i ) {
			cam->shot();
		}
	}
	return 0;
}
