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
#include "matrix.h"
#pragma once

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

typedef dolly::Matrix<4,4,float> Matrix4;
typedef dolly::Vector<4,float> Vector4;


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
		SaveMatrix(SuperPX& px);
		~SaveMatrix() noexcept;
	};
};
