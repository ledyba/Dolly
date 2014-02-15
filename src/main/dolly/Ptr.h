/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, PSI
 */
#pragma once

#include <exception>
#include <stdexcept>

namespace dolly
{

template <typename T>
void ptrCloser(T* ptr);

template<typename T>
class Ptr;

template<typename T>
class PtrSpirit {
private:
	T* const ptr_;
	int cnt_;
	~PtrSpirit(){
		if( ptr_ ) {
			ptrCloser<T>(ptr_);
		}
	}
	PtrSpirit(PtrSpirit<T> const&) = delete;
	PtrSpirit(PtrSpirit<T>&&) = delete;
	PtrSpirit& operator=(PtrSpirit<T> const&) = delete;
	PtrSpirit& operator=(PtrSpirit<T>&&) = delete;
public:
	PtrSpirit(T* ptr):ptr_(ptr),cnt_(1){
		if( !ptr ) {
			throw std::logic_error("Pointer must not zero.");
		}
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
class Ptr {
private:
	PtrSpirit<T>* ptr_;
public:
	Ptr(T* ptr):ptr_(ptr ? new PtrSpirit<T>(ptr) : nullptr){
	}
	Ptr():ptr_(nullptr){}
	Ptr(Ptr<T> const& p):ptr_(p.ptr_){
		if(*ptr_){
			ptr_->incref();
		}
	}
	Ptr(Ptr<T>&& p):ptr_(p.ptr_){}
	Ptr& operator=(Ptr<T>&& ptr){
		if( this->ptr_ ) {
			ptr_->decref();
		}
		this->ptr_ = ptr.ptr_;
		return *this;
	}
	Ptr& operator=(Ptr<T> const& ptr){
		if( ptr.ptr_ ) {
			ptr.ptr_->incref();
		}
		if( this->ptr_ ) {
			ptr_->decref();
		}
		this->ptr_ = ptr.ptr_;
		return *this;
	}
	~Ptr(){
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
	Ptr& set(T* ptr){
		if( this->ptr_ ) {
			ptr_->decref();
		}
		if( ptr ) {
			this->ptr_ = new PtrSpirit<T>(ptr);
		}else{
			this->ptr_ = nullptr;
		}
		return *this;
	}
	T* operator->(){
		return this->ptr_ ? this->ptr_->get() : nullptr;
	}
};

}
