include $(top_srcdir)/common.ac
lib_LTLIBRARIES=libdolly.la
libdolly_la_SOURCES=\
	CairoPtr.cpp\
	FFPtr.cpp\
	Dolly.cpp\
	Util.cpp

libdolly_la_CPPFLAGS = $(CAIRO_CFLAGS) $(SDL2_CFLAGS) $(FFMPEG_CFLAGS)
libdolly_la_LIBADD = $(CAIRO_LIBS) $(SDL2_LIBS) $(FFMPEG_LIBS)
