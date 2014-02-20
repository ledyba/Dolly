/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, psi
 */
#pragma once

#include <cairo/cairo.h>
#include <memory>
namespace dolly {

class Window {
private:
	const int width_;
	const int height_;
public:
	Window(const int width, const int height):width_(width), height_(height){};
	virtual ~Window() = default;
public:
	virtual void showFrame(cairo_surface_t* const surf) = 0;
	virtual bool handleWindowEvent(cairo_surface_t* const surf) = 0;
	virtual void closeWindow() = 0;
};

std::unique_ptr<Window> createWindow(const int width, const int height);

}
