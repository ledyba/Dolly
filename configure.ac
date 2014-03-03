AC_PREREQ([2.68])
AC_INIT([Dooly], [1.0], [nasda60___at___hotmail.com], [dolly], [http://ledyba.org/])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CC
CXXFLAGS=
CFLAGS=
AM_PROG_AS
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AX_CHECK_COMPILE_FLAG([-std=gnu++11], [CXXFLAGS="$CXXFLAGS -std=gnu++11"])
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

dnl Initialize libtool
dnl LT_PATH_LD
dnl LT_LIB_DLLOAD
dnl LT_PREREQ([2.4])
dnl LT_INIT([win32-dll disable-fast-install])
dnl LT_LANG([C++])

AC_DEFUN([PKG_CHECK_MODULES_STATIC],
[
	AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
	_save_PKG_CONFIG=$PKG_CONFIG
	PKG_CONFIG="$PKG_CONFIG --static"
	PKG_CHECK_MODULES([$1], [$2], [$3], [$4])
	PKG_CONFIG=$_save_PKG_CONFIG[]dnl
])

PKG_CHECK_MODULES_STATIC([CAIRO], [cairo])
PKG_CHECK_MODULES_STATIC([X11], [x11], [AC_DEFINE([HAVE_X11], [1], [Use X11])], [echo -n])
PKG_CHECK_MODULES_STATIC([FFMPEG], [libavcodec libavdevice libavfilter libavformat libavutil libswresample libswscale])
AX_CXX_CHECK_LIB( gtest, [testing::TestInfo::name() const] )
AC_CHECK_LIB(gmp, __gmpz_init, [
	AC_CHECK_HEADER([gmp.h],
					[AC_DEFINE([HAVE_GMP_H], [1],[HAVE GMP])
					 AC_DEFINE([HAVE_GMP], [1],[HAVE GMP])
					 AM_CONDITIONAL(HAVE_GMP, true)],
					[AC_MSG_ERROR([GNU MP not found, see http://gmplib.org/])])],
					[AC_MSG_WARN([GNU MP not found, see http://gmplib.org/])
					 AM_CONDITIONAL(HAVE_GMP, false)])
AC_CHECK_HEADER([Windows.h],
				[AC_DEFINE([HAVE_WINDOWS_H], [1],[Windows])
				 AM_CONDITIONAL(HAVE_WINDOWS, true)]
				 LIBS="-lgdi32 $LIBS")

AC_CONFIG_FILES([Makefile
				 src/libdolly.pc
				 src/include/Makefile
				 src/dolly/Makefile
				 src/sample/Makefile])
AC_OUTPUT

