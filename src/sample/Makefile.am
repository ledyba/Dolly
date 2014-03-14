include $(top_srcdir)/Makefile.am.common
noinst_PROGRAMS = word_rotation sim mandelbrot runge_kutta

word_rotation_SOURCES = word_rotation.cpp
word_rotation_LDADD = $(LIBDOLLY)

sim_SOURCES = sim.cpp
sim_LDADD = $(LIBDOLLY)

mandelbrot_SOURCES = mandelbrot.cpp
mandelbrot_LDADD = $(LIBDOLLY)

runge_kutta_SOURCES = runge_kutta.cpp
runge_kutta_LDADD = $(LIBDOLLY)

if HAVE_GMP
noinst_PROGRAMS += 
endif

