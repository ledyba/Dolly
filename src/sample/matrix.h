/*
 * matrix.h
 *
 *  Created on: 2014/07/30
 *      Author: psi
 */

#pragma once
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <array>

namespace dolly {

template <int N, int M, typename FT=double>
class Matrix final{
private:
	std::array<FT, N*M> vs_;
	template <int N_, int M_, typename FT_> friend class Matrix;
public:
	template<typename ...E>
	constexpr Matrix(FT const& e, E&&...init)
	:vs_{{e, static_cast<FT>(std::forward<E>(init))...}}
	{
	}
	Matrix() {
		std::fill(std::begin(vs_), std::end(vs_), 0);
	}
	Matrix(Matrix<N,M,FT> const& other){
		std::copy(std::begin(other.vs_), std::end(other.vs_), std::begin(vs_));
	}
	Matrix(Matrix<N,M,FT>&& other){
		std::copy(std::begin(other.vs_), std::end(other.vs_), std::begin(vs_));
	}
	Matrix<N,M,FT>& operator=(Matrix<N,M,FT> const& other){
		std::copy(std::begin(other.vs_), std::end(other.vs_), std::begin(vs_));
		return *this;
	}
	Matrix<N,M,FT>& operator=(Matrix<N,M,FT>&& other){
		std::copy(std::begin(other.vs_), std::end(other.vs_), std::begin(vs_));
		return *this;
	}
	~Matrix() = default;
public:
	constexpr FT const& operator()(int const n, int const m) const {
		return vs_.at(n*M + m);
	}
	FT& operator()(int const n, int const m) {
		return vs_.at(n*M + m);
	}
public:
	Matrix<N,M,FT> operator *(FT const& f) const {
		Matrix<N,M,FT> r(*this);
		for(auto& v : r.vs_) {
			v *= f;
		}
		return std::move(r);
	}
	Matrix<N,M,FT> operator /(FT const& f) const {
		Matrix<N,M,FT> r(*this);
		for(auto& v : r.vs_) {
			v /= f;
		}
		return std::move(r);
	}
	Matrix<N,M,FT>& operator *=(FT const& f) {
		for(auto& v : vs_) {
			v *= f;
		}
		return *this;
	}
	Matrix<N,M,FT>& operator /=(FT const& f) {
		for(auto& v : vs_) {
			v /= f;
		}
		return *this;
	}
public:
	template <int L>
	Matrix<N,L,FT> operator*(Matrix<M,L,FT> const& m) const{
		Matrix<N,M,FT> mat;
		for( int i=0;i<N;++i ) {
			for( int j=0;j<L;++j ) {
				FT s = 0;
				for( int k=0;k<M;++k ) {
					s += (*this)(i,k) * m(k,j);
				}
				mat(i,j) = s;
			}
		}
		return std::move(mat);
	}
	typename std::enable_if<N==M, Matrix<N,M,FT>& >::type operator*=(Matrix<N,M,FT> const& m) {
		return ((*this) = std::move( (*this) * m ));
	}
public:
	Matrix<M,N> trans() const{
		Matrix<M,N> m;
		for( int i=0;i<N;++i ) {
			for( int j=0;j<M;++j ) {
				m(i,j) = (*this)(j,i);
			}
		}
		return std::move(m);
	}
	typename std::enable_if<N==M, Matrix<N,M,FT>& >::type toTrans() {
		for( int i=0;i<N;++i ) {
			for( int j=0;j<M;++j ) {
				std::swap((*this)(i,j), (*this)(j,i));
			}
		}
		return *this;
	}
	static Matrix<N,M,FT> zero() {
		return std::move(Matrix<N,M,FT>());
	}
	static typename std::enable_if<N==M, Matrix<N,M,FT> >::type ident(){
		Matrix<N,M,FT> m;
		for( int i=0;i < N; ++i ) {
			m(i,i) = 1;
		}
		return std::move(m);
	}

	typename std::enable_if<N==M, Matrix<N,M,FT>& >::type toIdent(){
		for( int i=0;i < N; ++i ) {
			(*this)(i,i) = 1;
		}
		return *this;
	}
	Matrix<N,M,FT>& toZero(){
		std::fill(std::begin(vs_), std::end(vs_), 0.0);
		return *this;
	}
};


template <int N, typename FT=double>
class Vector final{
	std::array<FT, N> v_;
public:
	inline typename std::enable_if<(N > 0), FT>::type x() const { return this->v_[0]; };
	inline typename std::enable_if<(N > 1), FT>::type y() const { return this->v_[1]; };
	inline typename std::enable_if<(N > 2), FT>::type z() const { return this->v_[2]; };
	inline typename std::enable_if<(N > 3), FT>::type w() const { return this->v_[3]; };
	inline typename std::enable_if<(N > 0), FT>::type& x() { return this->v_[0]; };
	inline typename std::enable_if<(N > 1), FT>::type& y() { return this->v_[1]; };
	inline typename std::enable_if<(N > 2), FT>::type& z() { return this->v_[2]; };
	inline typename std::enable_if<(N > 3), FT>::type& w() { return this->v_[3]; };
public:
	Vector(){
		std::fill(std::begin(v_), std::end(v_), 0);
	}
	template<typename ...E>
	constexpr Vector(FT const& e, E&&...init)
	:v_{{e, static_cast<FT>(std::forward<E>(init))...}} {
	}
	Vector(FT const (&v)[N]) {
		std::copy(std::begin(v), std::end(v), std::begin(v_));
	}
	Vector(Vector const& o) {
		std::copy(std::begin(o.v_), std::end(o.v_), std::begin(v_));
	}
	Vector<N,FT>& operator=(Vector<N,FT> const& o) {
		std::copy(std::begin(o.v_), std::end(o.v_), std::begin(v_));
		return *this;
	}
	Vector<N,FT> operator*(FT const f) const {
		Vector<N,FT> v(*this);
		for(int i=0;i<N;++i){
			v(i) *= f;
		}
		return std::move(v);
	}
	Vector<N,FT>& operator*=(FT const f) {
		for(int i=0;i<N;++i){
			(*this)(i) *= f;
		}
		return *this;
	}
	Vector<N,FT> operator/(FT const f) const {
		Vector<N,FT> v(*this);
		for(int i=0;i<N;++i){
			v(i) /= f;
		}
		return std::move(v);
	}
	Vector<N,FT>& operator/=(FT const f) {
		for(int i=0;i<N;++i){
			(*this)(i) /= f;
		}
		return *this;
	}
	Vector<N,FT> operator-() const {
		Vector<N,FT> vec;
		for(int i=0;i<4;++i){
			vec(i) = -(*this)(i);
		}
		return std::move(vec);
	}
	Vector<N,FT> operator+(Vector<N,FT> const& o) const {
		Vector<N,FT> vec;
		for(int i=0;i<N;++i){
			vec(i) = (*this)(i) + o(i);
		}
		return std::move(vec);
	}
	Vector<N,FT> operator-(Vector<N,FT> const& o) const {
		Vector<N,FT> vec;
		for(int i=0;i<N;++i){
			vec(i) = (*this)(i) - o(i);
		}
		return std::move(vec);
	}
	Vector<N,FT>& operator+=(Vector<N,FT> const& o) {
		for(int i=0;i<N;++i){
			(*this)(i) += o(i);
		}
		return *this;
	}
	Vector<N,FT>& operator-=(Vector<N,FT> const& o) {
		for(int i=0;i<N;++i){
			(*this)(i) += o(i);
		}
		return *this;
	}
	~Vector() = default;
	inline FT const& operator()(int i) const{ return v_[i]; }
	inline FT const& operator[](int i) const{ return v_[i]; }
	inline FT& operator()(int i){ return v_[i]; }
	inline FT& operator[](int i){ return v_[i]; }
	FT length() const {
		FT s = 0;
		for(int i=0;i<N;++i){
			s += (*this)(i) * (*this)(i);
		}
		return s;
	}
public:
	FT dot(Vector<N,FT> const& v) {
		FT s = 0;
		for(int i=0;i<N;++i){
			s += v(i) * (*this)(i);
		}
		return s;
	}
	typename std::enable_if<N==4, Vector<N,FT> >::type cross(Vector<N,FT> const& v){
		return std::move(Vector<N,FT>(
				(*this)(1)*v(2) - (*this)(2)*v(1),
				(*this)(2)*v(0) - (*this)(0)*v(2),
				(*this)(0)*v(1) - (*this)(1)*v(0),
				0
			));
	}
};
template <int N, typename FT>
Vector<N,FT> operator*(Matrix<N,N,FT> const& mat, Vector<N,FT> const& vec)
{
	Vector<N,FT> v;
	for( int i=0; i<N;++i ) {
		FT s = 0;
		for( int j=0; j<N;++j ) {
			s += mat(i,j) * vec(j);
		}
		v(i) = s;
	}
	return std::move(v);

}

template<int N, typename FT, typename FT_>
Vector<N,FT> operator*(FT_ const f, Vector<N,FT> const& vec) {
	return std::move(vec*f);
}


}

