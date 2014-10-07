AC_INIT([Tutorial Program], 1.0)
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX

AC_SEARCH_LIBS([dlopen], [dl dld], [], [
  AC_MSG_ERROR([unable to find the dlopen() function])
])
PKG_CHECK_MODULES(DEPS, gtkmm-2.4 gstreamer-1.0)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
