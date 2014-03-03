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
	cairo_surface_t* img_;
	ATOM atom_;
public:
	Win32Window(const int width, const int height);
	virtual ~Win32Window();
	virtual void showFrame(cairo_surface_t* const  surf) override;
	virtual bool handleWindowEvent(cairo_surface_t* const  surf) override;
	virtual void closeWindow() override;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

void Win32Window::showFrame(cairo_surface_t* const  surf)
{
	img_ = surf;
	RedrawWindow(window_, nullptr, nullptr, RDW_INVALIDATE);
}

LRESULT CALLBACK Win32Window::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	Win32Window* const win = (Win32Window*)GetWindowLong(hwnd, GWL_USERDATA);
	switch (iMsg) {
	case WM_CREATE:
		static_assert(sizeof(LONG) == sizeof(LPVOID), "LONG != LPVOID");
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
		break;
	case WM_DESTROY: /* 終了処理 */
		PostQuitMessage(0);
		return 0;
	case WM_PAINT: {
		const int w = cairo_image_surface_get_width(win->img_);
		const int h = cairo_image_surface_get_height(win->img_);
		const int stride = cairo_image_surface_get_stride(win->img_);
		BITMAPINFO info;
		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = w;
		info.bmiHeader.biHeight = -h;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = 0;
		info.bmiHeader.biXPelsPerMeter = 72;
		info.bmiHeader.biYPelsPerMeter = 72;
		info.bmiHeader.biClrUsed = 0;
		info.bmiHeader.biClrImportant = 0;
		info.bmiHeader.biSizeImage = h * stride;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HDC memdc = CreateCompatibleDC(hdc);

		HBITMAP bmp = CreateCompatibleBitmap(hdc, w,h);
		SetDIBits(hdc, bmp, 0, h, cairo_image_surface_get_data(win->img_), &info, DIB_RGB_COLORS);

		HGDIOBJ old = SelectObject(memdc, bmp);
		BitBlt(hdc, 0, 0, w, h, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, old);
		DeleteObject(bmp);
		EndPaint(hwnd, &ps);
	}
		break;
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
,atom_(0)
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
	wndclass.lpszClassName = "Win32Dolly";   /* クラス名 */
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);

	atom_ = RegisterClassEx(&wndclass);
	if(atom_ == 0){
		throw std::invalid_argument("Failed to init window class.");
	}
	const DWORD style = (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE;
	RECT r {0,0,width,height};
	AdjustWindowRectEx(&r,style,false,WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
	window_ = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST, (LPCSTR)((int)atom_), TEXT("Dolly"), style, CW_USEDEFAULT, CW_USEDEFAULT, r.right-r.left, r.bottom-r.top, NULL, 0, mod, this);
}
Win32Window::~Win32Window()
{
	this->closeWindow();
	auto mod = GetModuleHandle(NULL);
	UnregisterClass((LPCSTR)((int)atom_), mod);
}

std::unique_ptr<Window> createWindow(const int width, const int height)
{
	return std::unique_ptr<Window>(new Win32Window(width, height));
}

}
#endif
