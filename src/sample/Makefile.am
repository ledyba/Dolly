include $(top_srcdir)/Makefile.am.common
noinst_PROGRAMS = \
  word_rotation \
  mandelbrot \
  runge_kutta \
  3d_texture

word_rotation_SOURCES = word_rotation.cpp
word_rotation_LDADD = $(LIBDOLLY)

mandelbrot_SOURCES = mandelbrot.cpp
mandelbrot_LDADD = $(LIBDOLLY)

runge_kutta_SOURCES = runge_kutta.cpp
runge_kutta_LDADD = $(LIBDOLLY)

3d_texture_SOURCES = 3d_texture.cpp super_px.cpp
3d_texture_LDADD = $(LIBDOLLY)

if HAVE_GMP
noinst_PROGRAMS += 
endif

