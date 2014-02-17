AC_PREREQ([2.68])
AC_INIT([Dooly], [1.0], [nasda60___at___hotmail.com], [dolly], [http://ledyba.org/])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_PATH_LD

# Initialize libtool
LT_PREREQ([2.4])
LT_INIT([win32-dll])


AC_DEFUN([PKG_CHECK_MODULES_STATIC],
[
	AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
	_save_PKG_CONFIG=$PKG_CONFIG
	PKG_CONFIG="$PKG_CONFIG --static"
	PKG_CHECK_MODULES([$1], [$2], [$3], [$4], [$5])
	PKG_CONFIG=$_save_PKG_CONFIG[]dnl
])

PKG_CHECK_MODULES([CAIRO], [cairo])
PKG_CHECK_MODULES([SDL2], [sdl2])
PKG_CHECK_MODULES([FFMPEG], [libavcodec libavdevice libavfilter libavformat libavutil libswresample libswscale])
AX_CXX_CHECK_LIB( gtest, [testing::TestInfo::name() const] )


AC_CONFIG_FILES([Makefile
                 src/include/Makefile
                 src/dolly/Makefile
                 src/sample/Makefile])
AC_OUTPUT

