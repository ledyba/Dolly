include $(top_srcdir)/Makefile.am.common
lib_LTLIBRARIES=libdolly.la
libdolly_la_SOURCES=\
	CairoPtr.cpp\
	FFPtr.cpp\
	Dolly.cpp\
	Util.cpp

libdolly_la_CPPFLAGS = $(CAIRO_CFLAGS) $(SDL2_CFLAGS) $(FFMPEG_CFLAGS)
libdolly_la_LIBADD = $(CAIRO_LIBS) $(SDL2_LIBS) $(FFMPEG_LIBS)

#libdolly_la_LDFLAGS = -no-undefined \
#        -avoid-version \
#        -Wl,-o,.libs/dolly.dll \
#        -Wl,--output-def,.libs/dolly.def,--out-implib,.libs/dolly.dll.a