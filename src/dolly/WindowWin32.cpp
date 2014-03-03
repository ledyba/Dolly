/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2014, psi
 */
#include "../config.h"

#if HAVE_WINDOWS_H
#include "Window.h"
#include <Windows.h>
#include <cstdio>

namespace dolly {

class Win32Window final : public Window {
	HWND window_;
public:
	Win32Window(const int width, const int height);
	virtual ~Win32Window();
	virtual void showFrame(cairo_surface_t* const  surf) override;
	virtual bool handleWindowEvent(cairo_surface_t* const  surf) override;
	virtual void closeWindow() override;
};

void Win32Window::showFrame(cairo_surface_t* const  surf)
{
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
		case WM_DESTROY : /* 終了処理 */
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;

}
bool Win32Window::handleWindowEvent(cairo_surface_t* const surf)
{
	if( !window_ ) {
		return false;
	}
	MSG msg = { 0 };
	while (PeekMessage (&msg,NULL,0,0,PM_NOREMOVE))
	{
		const int r = GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if( msg.message == WM_QUIT ) {
			closeWindow();
			return true;
		}else{
		}
	}
	return false;
}

void Win32Window::closeWindow()
{
	if(window_){
		CloseWindow(window_);
		window_=nullptr;
	}
}

Win32Window::Win32Window(const int width, const int height)
:Window(width, height)
,window_(NULL)
{
	auto mod = GetModuleHandle(NULL);
	WNDCLASSEX  wndclass ;

	wndclass.cbSize        = sizeof(wndclass);        /* 構造体の大きさ */
	wndclass.style         = CS_HREDRAW | CS_VREDRAW; /* スタイル */
	wndclass.lpfnWndProc   = WndProc;                /* メッセージ処理関数 */
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = mod;             /* プログラムのハンドル */
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); /* アイコン */
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);     /* カーソル */
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); /* ブラシ */
	wndclass.lpszMenuName  = NULL;              /* メニュー */
	wndclass.lpszClassName = "Test Window";   /* クラス名 */
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);

	ATOM atom = RegisterClassEx(&wndclass);
	if(atom == 0){
		throw std::invalid_argument("Failed to init window class.");
	}
	window_ = CreateWindowExA(0, TEXT("Test Window"), "Program activation", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, 0, mod, NULL);
}
Win32Window::~Win32Window()
{
	this->closeWindow();
}

std::unique_ptr<Window> createWindow(const int width, const int height)
{
	return std::unique_ptr<Window>(new Win32Window(width, height));
}

}
#endif
