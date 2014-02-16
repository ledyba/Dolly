AC_INIT([Dooly], [1.0], [nasda60___at___hotmail.com], [dolly], [http://ledyba.org/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_LANG([C++])
AC_CONFIG_FILES([Makefile])

AC_DEFUN([PKG_CHECK_MODULES_STATIC],
[
	AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
	_save_PKG_CONFIG=$PKG_CONFIG
	PKG_CONFIG="$PKG_CONFIG --static"
	PKG_CHECK_MODULES([$1], [$2], [$3], [$4], [$5])
	PKG_CONFIG=$_save_PKG_CONFIG[]dnl
])

PKG_CHECK_MODULES_STATIC([CAIRO], [cairo])
PKG_CHECK_MODULES_STATIC([SDL2], [sdl2])
PKG_CHECK_MODULES_STATIC([FFMPEG], [libavcodec libavdevice libavfilter libavformat libavutil libswresample libswscale])
AX_CXX_CHECK_LIB( gtest, [testing::TestInfo::name() const] )
AC_OUTPUT

