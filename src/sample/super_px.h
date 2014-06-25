/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include <dolly/Dolly.h>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <vector>
class Color final{
	float r_;
	float g_;
	float b_;
	float a_;
public:
	inline Color(const float r,const float g,const float b, const float a = 1.0f)
	:r_(r), g_(g), b_(b), a_(a){
	}
	inline Color()
	:r_(0), g_(0), b_(0), a_(0){
	}

	Color(Color const&) = default;
	Color(Color&&) = default;
	Color& operator=(Color const&) = default;
	Color& operator=(Color&&) = default;
	~Color() = default;
	inline float r() const { return this->r_; }
	inline float r(float v) { return this->r_ = v; }
	inline float g() const { return this->g_; }
	inline float g(float v) { return this->g_ = v; }
	inline float b() const { return this->b_; }
	inline float b(float v) { return this->b_ = v; }
	inline float a() const { return this->a_; }
	inline float a(float v) { return this->a_ = v; }
};

class Matrix4 final {
	float *val_;
public:
	inline float operator()(int row, int column) const { return this->val_[row*4 + column]; };
	inline float& operator()(int row, int column) { return this->val_[row*4 + column]; };
public:
	Matrix4();
	Matrix4(
			float const a11, float const a12, float const a13, float const a14,
			float const a21, float const a22, float const a23, float const a24,
			float const a31, float const a32, float const a33, float const a34,
			float const a41, float const a42, float const a43, float const a44
	);
	static Matrix4 zero();
	static Matrix4 ident();
	Matrix4& toIdent();
	Matrix4& toZero();
	Matrix4& toTrans();
	Matrix4 trans() const;
	Matrix4(float (val)[16]);
	Matrix4(Matrix4 const& o);
	Matrix4(Matrix4&& o);
	Matrix4& operator=(Matrix4 const& o);
	Matrix4& operator=(Matrix4&& o);
	~Matrix4();
public:
	Matrix4 operator*(float const f) const;
	Matrix4 operator/(float const f) const;
	Matrix4 operator*(Matrix4 const& m) const;
	Matrix4& operator*=(Matrix4 const& m);
	Matrix4& operator*=(float const f);
	Matrix4& operator/=(float const f);
};

Matrix4 operator*(float f, Matrix4 const& v);

class Vector4 final{
	float v_[4];
public:
	inline float x() const { return this->v_[0]; };
	inline float y() const { return this->v_[1]; };
	inline float z() const { return this->v_[2]; };
	inline float w() const { return this->v_[3]; };
	inline float& x() { return this->v_[0]; };
	inline float& y() { return this->v_[1]; };
	inline float& z() { return this->v_[2]; };
	inline float& w() { return this->v_[3]; };
public:
	Vector4();
	Vector4(float const x,float const y,float const z,float const w);
	Vector4(float const (&v)[4]);
	Vector4(Vector4 const& o);
	Vector4& operator=(Vector4 const& o);
	Vector4 operator*(float const f) const;
	Vector4& operator*=(float const f);
	Vector4 operator/(float const f) const;
	Vector4& operator/=(float const f);
	Vector4 operator-() const;
	Vector4 operator+(Vector4 const& o);
	Vector4 operator-(Vector4 const& o);
	Vector4& operator+=(Vector4 const& o);
	Vector4& operator-=(Vector4 const& o);
	~Vector4() = default;
	inline float const& operator()(int i) const{ return v_[i]; }
	inline float const& operator[](int i) const{ return v_[i]; }
	inline float& operator()(int i){ return v_[i]; }
	inline float& operator[](int i){ return v_[i]; }
public:
	float dot(Vector4 const& v);
	Vector4 cross(Vector4 const& v){
		//FIXME
		Vector4 s;
		for(int i=0;i<4;++i){
			s(i) = 0;
		}
		return std::move(s);
	}
};
Vector4 operator*(Matrix4 const& mat, Vector4 const& vec);
Vector4 operator*(float const f, Vector4 const& vec);

class SuperPX final{
private:
	cairo_t* cr_;
	cairo_surface_t* surf_;
	Color clearColor_;
private:
	Matrix4 proj_;
	Matrix4 model_;
	std::vector<Matrix4> savedMatrix_;
public:
	SuperPX(cairo_t* const& cr, cairo_surface_t* surf);
	~SuperPX() = default;
	SuperPX(SuperPX const&) = delete;
	SuperPX(SuperPX&&) = delete;
	SuperPX& operator=(SuperPX const&) = delete;
	SuperPX& operator=(SuperPX&&) = delete;
public:
	Color const& clearColor(Color const& c);
	Color const& clearColor() const;
public:
	void pushMatrix();
	void popMatrix();
	void color(Color const& c);
	void clear();
	void dot(const int x, const int y, Color const& c);
	Vector4 toScreen(Vector4 const& f);
	void line( Vector4 const& from, Vector4 const& to, Color const& c);
	void renderTex(int const x, int const y, cairo_surface_t* img, Vector4 const& texPt_);
	void triangle(
			Vector4 const& a_, Vector4 const& b_, Vector4 const& c_,
			cairo_surface_t* img,
			Vector4 const& u_, Vector4 const& v_, Vector4 const& w_);
	void move(float const tx, float const ty, float const tz = 0.0f);
	void scale(float const sx, float const sy, float const sz = 1.0f);
	void frustum(float l,float r,float b,float t,float n,float f);
	void rotate(float x, float y, float z, float angle);
public:
	class SaveMatrix final{
	private:
		SuperPX& px_;
		SaveMatrix(SaveMatrix const&) = delete;
		SaveMatrix(SaveMatrix&&) = delete;
		SaveMatrix& operator=(SaveMatrix const&) = delete;
		SaveMatrix& operator=(SaveMatrix&&) = delete;
	public:
		SaveMatrix(SuperPX& px):px_(px) {
			px_.pushMatrix();
		}
		~SaveMatrix() noexcept {
			px_.popMatrix();
		}
	};
};
