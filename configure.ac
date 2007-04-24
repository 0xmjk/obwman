 dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT()

AM_CONFIG_HEADER([autoconfig.h])

VERSION=`head ChangeLog -n 1 | cut -d \( -f 2 | cut -d \) -f 1 | cut -d '-' -f 1`

AM_INIT_AUTOMAKE([obwman], $VERSION)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_YACC
AM_PROG_LEX

exec_prefix=${prefix}

AC_DEFUN(EVALUATE_PREFIX, [
  for var in prefix exec_prefix bindir sbindir libexecdir mandir \
       sysconfdir datadir includedir localstatedir runtimedir \
       logfiledir libdir installbuilddir libsuffix; do
               eval "val=\"\$$var\""
	       eval "$var=$val"
   done
])
				    

EVALUATE_PREFIX

AC_CONFIG_FILES([Makefile config.h])

AC_OUTPUT
