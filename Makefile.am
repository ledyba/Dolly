include $(top_srcdir)/am.conf
ACLOCAL_AMFLAGS = -I m4 
SUBDIRS = src/include src/dolly src/sample

AM_CXXFLAGS = -std=gnu++11
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = sample
sample_SOURCES =\
	src/entrypoint/main.cpp\
	src/main/Field.cpp\
	src/main/Field.h\
	src/main/sha256.cpp\
	src/main/sha256.h\
	src/main/dolly/CairoPtr.h\
	src/main/dolly/CairoPtr.cpp\
	src/main/dolly/FFPtr.h\
	src/main/dolly/FFPtr.cpp\
	src/main/dolly/Dolly.h\
	src/main/dolly/Dolly.cpp\
	src/main/dolly/Util.h\
	src/main/dolly/Util.cpp

AM_CPPFLAGS = $(CAIRO_CFLAGS) $(SDL2_CFLAGS) $(FFMPEG_CFLAGS)
sample_LDADD = $(CAIRO_LIBS) $(SDL2_LIBS) $(FFMPEG_LIBS)
