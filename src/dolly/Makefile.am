include $(top_srcdir)/Makefile.am.common
lib_LIBRARIES=libdolly.a
libdolly_a_SOURCES=\
	CairoPtr.cpp\
	FFPtr.cpp\
	Dolly.cpp\
	Util.cpp\
	Window.h\
	WindowX11.cpp

libdolly_a_CPPFLAGS = $(CAIRO_CFLAGS) $(SDL2_CFLAGS) $(FFMPEG_CFLAGS)
#libdolly_a_LIBADD = $(CAIRO_LIBS) $(SDL2_LIBS) $(FFMPEG_LIBS)

#libdolly_la_LDFLAGS = -no-undefined \
#        -avoid-version \
#        -Wl,-o,.libs/dolly.dll \
#        -Wl,--output-def,.libs/dolly.def,--out-implib,.libs/dolly.dll.a

