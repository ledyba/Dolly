AC_PREREQ([2.63])
AC_INIT([Dooly], [1.0], [nasda60___at___hotmail.com], [dolly], [http://ledyba.org/])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
AC_LANG([C++])

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([win32-dll])

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


AC_CONFIG_FILES([Makefile
                 src/include/Makefile
                 src/dolly/Makefile
                 src/sample/Makefile])
AC_OUTPUT

