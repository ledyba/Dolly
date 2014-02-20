/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, psi
 */
#include "../config.h"
#if HAVE_X11

#include "Window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace dolly {

class X11Window final : public Window {
private:
	Display* display_;
	int screen_;
	::Window window_;
	Visual* visual_;
	GC gc_;
	Atom delWindow_;
public:
	X11Window(const int width, const int height);
	virtual ~X11Window();
	virtual void showFrame(cairo_surface_t* const  surf) override;
	virtual bool handleWindowEvent(cairo_surface_t* const  surf) override;
	virtual void closeWindow() override;
};

void X11Window::showFrame(cairo_surface_t* const  surf)
{
	if(!this->window_){
		return;
	}
	XImage* img = XCreateImage(
			this->display_, DefaultVisual( this->display_, this->screen_ ), 32,
			ZPixmap,
			0,
			(char*)cairo_image_surface_get_data(surf),
			cairo_image_surface_get_width(surf),
			cairo_image_surface_get_height(surf),
			32,
			cairo_image_surface_get_stride(surf)
	);
	XPutImage(
			this->display_,
			this->window_,
			gc_,
			img,
			0, 0, 0,0,
			cairo_image_surface_get_width(surf),
			cairo_image_surface_get_height(surf));
	img->data = nullptr;
	XDestroyImage( img );
	XFlush(display_);
}

bool X11Window::handleWindowEvent(cairo_surface_t* const surf){
	if(!display_){
		return false;
	}else if(XPending(display_) <= 0 ){
		return false;
	}
	XEvent e;
	XNextEvent(display_, &e);
	if (e.type == Expose) {
		showFrame(surf);
	}else if (e.type == ClientMessage){
		closeWindow();
		return true;
	}
	return false;
}

void X11Window::closeWindow()
{
	if(this->display_){
		XDestroyWindow(this->display_, this->window_);
		XFreeGC(display_, gc_);
		XCloseDisplay(this->display_);
		display_ = nullptr;
	}
}

X11Window::X11Window(const int width, const int height)
:Window(width, height)
,display_(nullptr)
,screen_(0)
,window_()
,visual_()
,gc_()
,delWindow_()
{
	this->display_ = XOpenDisplay(nullptr);
	this->screen_ = DefaultScreen(this->display_);
	XVisualInfo vis_info;
	if(!XMatchVisualInfo(display_, screen_, 32, TrueColor, &vis_info)) {
		return;
	}
	this->visual_ = vis_info.visual;
	XSetWindowAttributes win_attr;
	win_attr.colormap = XCreateColormap(display_, RootWindow(display_, screen_), vis_info.visual, AllocNone);
	win_attr.background_pixel = 0;
	win_attr.border_pixel = 0;
	this->window_ = XCreateWindow(this->display_, RootWindow(display_, screen_), 0, 0, width, height, 1,32, InputOutput,vis_info.visual, CWBackPixel | CWColormap | CWBorderPixel, &win_attr);
	gc_ = XCreateGC(display_, window_, 0, 0);
	delWindow_ = XInternAtom( display_, "WM_DELETE_WINDOW", 0 );
	XSetWMProtocols(display_ , window_, &delWindow_, 1);
	XSelectInput(this->display_, this->window_, ExposureMask);
	XMapWindow(this->display_, this->window_);
	{
		XSizeHints *sizehints = XAllocSizeHints();
		long userhints;

		XGetWMNormalHints(display_, window_, sizehints, &userhints);

		sizehints->min_width = sizehints->max_width = width;
		sizehints->min_height = sizehints->max_height = height;
		sizehints->flags |= PMinSize | PMaxSize;

		XSetWMNormalHints(display_, window_, sizehints);

		XFree(sizehints);
	}
	{
		char * title = (char*)"Dolly";
		XTextProperty titleprop;
		XStringListToTextProperty(&title, 1, &titleprop);
		XSetWMName(display_, window_, &titleprop);
		XFree(titleprop.value);
	}
	XFlush(this->display_);
}
X11Window::~X11Window()
{
	this->closeWindow();
}

std::unique_ptr<Window> createWindow(const int width, const int height){
	return std::unique_ptr<Window>(new X11Window(width, height));
}

}
#endif
