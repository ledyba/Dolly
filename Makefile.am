include $(top_srcdir)/Makefile.am.common
SUBDIRS = src/include src/dolly src/sample
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# pkg-config stuff, note that libdolly.pc is always required in order
# to get the correct library
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = src/libdolly.pc
