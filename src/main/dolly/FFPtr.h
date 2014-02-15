/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once

#include <exception>
#include <stdexcept>
extern "C" {
#include <libavformat/avformat.h>
}

namespace dolly
{

template <typename T>
void ffCloser(T* ptr);

template<typename T>
class FFPtr;

template<typename T>
class FFSpirit {
private:
	T* const ptr_;
	int cnt_;
	~FFSpirit(){
		if( ptr_ ) {
			ffCloser<T>(ptr_);
		}
	}
	FFSpirit(FFSpirit<T> const&) = delete;
	FFSpirit(FFSpirit<T>&&) = delete;
	FFSpirit& operator=(FFSpirit<T> const&) = delete;
	FFSpirit& operator=(FFSpirit<T>&&) = delete;
public:
	FFSpirit(T* ptr):ptr_(ptr),cnt_(1){
	}
	T* get(){
		return ptr_;
	}
	const T* get() const{
		return ptr_;
	}
	operator bool() const{
		return ptr_;
	}
	void incref(){
		if( cnt_ == 0 ) {
			throw std::logic_error("Pointer cnt must not zero.");
		}
		++cnt_;
	}
	void decref(){
		if( cnt_ == 0 ) {
			throw std::logic_error("Pointer cnt must not zero.");
		}else if( cnt_ == 1 ){
			delete this;
		}else{
			--cnt_;
		}
	}
};

template<typename T>
class FFPtr {
private:
	FFSpirit<T>* ptr_;
public:
	FFPtr(T* ptr):ptr_(ptr ? new FFSpirit<T>(ptr) : nullptr){
	}
	FFPtr():ptr_(nullptr){}
	FFPtr(FFPtr<T> const& p):ptr_(p.ptr_){
		if(*ptr_){
			ptr_->incref();
		}
	}
	FFPtr(FFPtr<T>&& p):ptr_(p.ptr_){}
	FFPtr& operator=(FFPtr<T>&& ptr){
		if( this->ptr_ ) {
			ptr_->decref();
		}
		this->ptr_ = ptr.ptr_;
		return *this;
	}
	~FFPtr(){
		if( *ptr_ ) {
			ptr_->decref();
		}
	}
	T* get(){
		return ptr_ ? ptr_->get() : nullptr;
	}
	const T* get() const{
		return ptr_ ? ptr_->get() : nullptr;
	}
	operator bool() const{
		return ptr_ && *ptr_;
	}
	FFPtr& operator=(FFPtr<T>& ptr){
		if( ptr.ptr_ ) {
			ptr.ptr_->incref();
		}
		if( this->ptr_ ) {
			ptr_->decref();
		}
		this->ptr_ = ptr.ptr_;
		return *this;
	}
	FFPtr& set(T* ptr){
		if( this->ptr_ ) {
			ptr_->decref();
		}
		if( ptr ) {
			this->ptr_ = new FFSpirit<T>(ptr);
		}else{
			this->ptr_ = nullptr;
		}
		return *this;
	}
	T* operator->(){
		return this->ptr_ ? this->ptr_->get() : nullptr;
	}
};

typedef FFPtr<AVCodecContext> FFCodecContext;
typedef FFPtr<AVFrame> FFFrame;
typedef FFPtr<AVFormatContext> FFFormatContext;

}
