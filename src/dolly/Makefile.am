include $(top_srcdir)/Makefile.am.common
lib_LIBRARIES=libdolly.a
libdolly_a_SOURCES=\
	CairoPtr.cpp\
	FFPtr.h\
	FFPtr.cpp\
	Dolly.cpp\
	Util.cpp\
	Window.h\
	WindowX11.cpp\
	WindowWin32.cpp\
	WindowNop.cpp\
	Film.cpp\
	Film.h

libdolly_a_CPPFLAGS = $(CAIRO_CFLAGS) $(X11_CFLAGS) $(FFMPEG_CFLAGS) $(LIBAV_CFLAGS)
#libdolly_a_LIBADD = $(CAIRO_LIBS) $(X11_LIBS) $(FFMPEG_LIBS) $(LIBAV_LIBS)

#libdolly_la_LDFLAGS = -no-undefined \
#        -avoid-version \
#        -Wl,-o,.libs/dolly.dll \
#        -Wl,--output-def,.libs/dolly.def,--out-implib,.libs/dolly.dll.a

