/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */
#pragma once
#include <unordered_map>
#include <cmath>

namespace clr {

#define DEFINE_MEMBER(rscope, wscope, type, name)\
private:\
	type name##_;\
rscope:\
	inline type const& name() const noexcept{return name##_;}\
wscope:\
	inline void name(type const& val){name##_ = val;}

class Color final {
private:
	DEFINE_MEMBER(public, private, bool, transparent);
	DEFINE_MEMBER(public, private, float, red);
	DEFINE_MEMBER(public, private, float, green);
	DEFINE_MEMBER(public, private, float, blue);
public:
	Color( float r, float g,float b )
	:transparent_(false)
	,red_(r)
	,green_(g)
	,blue_(b) {
	}
	Color( )
	:transparent_(true), red_(NAN), green_(NAN), blue_(NAN){}
	~Color() = default;
public:
	Color avg( Color const& c ) const {
		if( this->transparent() ) {
			return c;
		}else if( c.transparent() ){
			return *this;
		}else{
			return Color(
					//		static_cast<unsigned char>( (static_cast<unsigned int>(red())+static_cast<unsigned int>(c.red()) )/2 ),
					//		static_cast<unsigned char>( (static_cast<unsigned int>(green())+static_cast<unsigned int>(c.green()) )/2 ),
					//		static_cast<unsigned char>( (static_cast<unsigned int>(blue())+static_cast<unsigned int>(c.blue()) )/2 )
					( red  () + c.red  () )/2.0f,
					( green() + c.green() )/2.0f,
					( blue () + c.blue () )/2.0f
			);
		}
	}
};

class Field final{
private:
	uint32_t spr_;
	Color color_;
public:
	Field(uint32_t const& t, Color c):spr_(t), color_(c){};
	explicit Field(uint32_t const& t):spr_(t), color_(){};
	Field():spr_(0), color_(){};
	~Field() = default;
	Color color() const { return this->color_; }
public:
	Field& operator = ( Field const& t ) {
		spr_ = t.spr_;
		color_ = t.color_;
		return *this;
	}
	Field& operator *= ( Field const& t ) {
		spr_ *= t.spr_;
		color_ = color_.avg(t.color_);
		return *this;
	}
	Field& operator /= ( Field const& t ) {
		spr_ /= t.spr_;
		color_ = color_.avg(t.color_);
		return *this;
	}
	Field& operator += ( Field const& t ) {
		spr_ += t.spr_;
		color_ = color_.avg(t.color_);
		return *this;
	}
	Field& operator -= ( Field const& t ) {
		spr_ -= t.spr_;
		color_ = color_.avg(t.color_);
		return *this;
	}
	Field& operator &= ( Field const& t ) {
		spr_ &= t.spr_;
		color_ = color_.avg(t.color_);
		return *this;
	}
	Field operator + ( Field const& t ) const{
		return Field(spr_ + t.spr_, color_.avg(t.color_));
	}
	Field operator - ( Field const& t ) const{
		return Field(spr_ - t.spr_, color_.avg(t.color_));
	}
	Field operator / ( Field const& t ) const{
		return Field(spr_ / t.spr_, color_.avg(t.color_));
	}
	Field operator * ( Field const& t ) const{
		return Field(spr_ * t.spr_, color_.avg(t.color_));
	}
	Field operator ^ ( Field const& t ) const{
		return Field(spr_ ^ t.spr_, color_.avg(t.color_));
	}
	Field operator >> ( Field const& t ) const{
		return Field(spr_ >> t.spr_, color_.avg(t.color_));
	}
	Field operator << ( Field const& t ) const{
		return Field(spr_ << t.spr_, color_.avg(t.color_));
	}
	Field operator && ( Field const& t ) const{
		return Field(spr_ && t.spr_, color_.avg(t.color_));
	}
	Field operator & ( Field const& t ) const{
		return Field(spr_ & t.spr_, color_.avg(t.color_));
	}
	Field operator || ( Field const& t ) const{
		return Field(spr_ || t.spr_, color_.avg(t.color_));
	}
	Field operator | ( Field const& t ) const{
		return Field(spr_ | t.spr_, color_.avg(t.color_));
	}
	template <typename T>
	bool operator < ( T const& t ) const{
		return spr_ < t;
	}
	template <typename T>
	bool operator > ( T const& t ) const{
		return spr_ > t;
	}
	uint32_t value() const {
		return spr_;
	}
};

}
