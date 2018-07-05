/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, psi
 */
#include "../config.h"
#ifndef HAVE_X11
#ifndef HAVE_WINDOWS_H

#include "Window.h"

namespace dolly {

class NopWindow final : public Window {
private:
public:
	NopWindow(const int width, const int height);
	virtual ~NopWindow() = default;
	virtual void showFrame(cairo_surface_t* const  surf) override;
	virtual bool handleWindowEvent(cairo_surface_t* const  surf) override;
	virtual void closeWindow() override;
};

void NopWindow::showFrame(cairo_surface_t* const  surf)
{
}

bool NopWindow::handleWindowEvent(cairo_surface_t* const surf){
	return false;
}

void NopWindow::closeWindow()
{
}

NopWindow::NopWindow(const int width, const int height)
:Window(width, height)
{
}

std::unique_ptr<Window> createWindow(const int width, const int height){
	return std::unique_ptr<Window>(new NopWindow(width, height));
}

}
#endif
#endif
