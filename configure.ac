##############################################################################
## $Id: configure.ac,v 1.5 2004-05-05 21:47:34 behdad Exp $
## $Auther: behdad $
## $Date: 2004-05-05 21:47:34 $
## $Revision: 1.5 $
## $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/configure.ac,v $
##############################################################################
##                                               -*- Autoconf -*-
## Process this file with autoconf to produce a configure script.
## Use autoupdate to update this file for newer versions of autoconf.
## Use autoscan to check if you need to add something to this file.

AC_PREREQ(2.56)

dnl Define different version variables.
dnl
dnl Making releases:
dnl   FRIBIDI_MICRO_VERSION++;
dnl   FRIBIDI_INTERFACE_AGE++;
dnl   FRIBIDI_RPM_RELEASE = 1;
dnl
dnl If any functions have been added:
dnl   FRIBIDI_INTERFACE_AGE = 0;
dnl   FRIBIDI_INTERFACE_VERSION++;
dnl   FRIBIDI_BINARY_AGE++;
dnl
dnl If binary backwards compatibility has been broken:
dnl   FRIBIDI_BINARY_AGE = FRIBIDI_INTERFACE_AGE = 0;
dnl
dnl Building RPMs:
dnl   FRIBIDI_RPM_RELEASE++;
dnl
m4_define(fribidi_major_version, 0)dnl
m4_define(fribidi_minor_version, 19)dnl
m4_define(fribidi_micro_version, 0)dnl
m4_define(fribidi_version,fribidi_major_version.fribidi_minor_version.fribidi_micro_version)dnl
m4_define(fribidi_interface_version, 3)dnl
m4_define(fribidi_interface_age, 0)dnl
m4_define(fribidi_binary_age, 3)dnl
m4_define(fribidi_rpm_release, 1)dnl
dnl

AC_INIT([GNU FriBidi],fribidi_version(),[http://freedesktop.org/cgi-bin/bugzilla/enter_bug.cgi?product=FriBidi])
AC_REVISION([$Id: configure.ac,v 1.5 2004-05-05 21:47:34 behdad Exp $])
AC_CONFIG_SRCDIR(lib/fribidi.h)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([gnits])

FRIBIDI_MAJOR_VERSION=fribidi_major_version()
FRIBIDI_MINOR_VERSION=fribidi_minor_version()
FRIBIDI_MICRO_VERSION=fribidi_micro_version()
FRIBIDI_VERSION=fribidi_version()
FRIBIDI_INTERFACE_VERSION=fribidi_interface_version()
FRIBIDI_INTERFACE_AGE=fribidi_interface_age()
FRIBIDI_BINARY_AGE=fribidi_binary_age()
FRIBIDI_RPM_RELEASE=fribidi_rpm_release

AC_SUBST(FRIBIDI_MAJOR_VERSION)
AC_SUBST(FRIBIDI_MINOR_VERSION)
AC_SUBST(FRIBIDI_MICRO_VERSION)
AC_SUBST(FRIBIDI_VERSION)
AC_SUBST(FRIBIDI_INTERFACE_VERSION)
AC_SUBST(FRIBIDI_INTERFACE_AGE)
AC_SUBST(FRIBIDI_BINARY_AGE)
AC_SUBST(FRIBIDI_RPM_RELEASE)

dnl libtool versioning.
m4_define(lt_current, fribidi_interface_version())dnl
m4_define(lt_revision, fribidi_interface_age())dnl
m4_define(lt_age, fribidi_binary_age())dnl
LT_VERSION_INFO=lt_current():lt_revision():lt_age()
LT_CURRENT_MINUS_AGE=m4_eval(lt_current - lt_age)

AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_CURRENT_MINUS_AGE)

dnl ==========================================================================

# Checks for Win32.
AC_MSG_CHECKING([for native Win32])
  case "$host" in
    *-*-mingw*) os_win32=yes ;;
    *) pango_os_win32=no ;;
  esac
AC_MSG_RESULT([$os_win32])
AM_CONDITIONAL(OS_WIN32, test "$os_win32" = "yes")
AC_MSG_CHECKING([for some Win32 platform])
  case "$host" in
    *-*-mingw*|*-*-cygwin*) platform_win32=yes ;;
    *) platform_win32=no ;;
  esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([stdlib.h string.h strings.h inttypes.h stdbool.h wchar.h sys/times.h asm/page.h])

# Checks for typedefs and structures.
AC_C_CONST
AC_C_INLINE
AC_C_STRINGIZE
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(wchar_t)

# Checks for compiler characteristics.
changequote(,)dnl
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -Wall " ;;
  esac
  case " $CFLAGS " in
  *[\ \	]-pedantic[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -pedantic " ;;
  esac
fi
changequote([,])dnl

# Checks for library functions.
AC_CHECK_FUNCS([memmove memset strdup])
AC_TYPE_SIGNAL

# Checks for configure options

m4_define(debug_default,
          m4_if(m4_eval(fribidi_minor_version() % 2),1,yes,minimum))

# --enable-debug
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],
                             [turn on debugging @<:@default=debug_default()@:>@]),,
              enableval=debug_default())
if test x$enableval = xyes; then
	AC_DEFINE(DEBUG,1,
                  [Define to 1 if you want to include debug code in the library])
fi

# --enable-malloc
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-malloc],
                             [use simple malloc instead of memory chunks]))
if test x$enable_glib = xyes; then
	AC_DEFINE(USE_SIMPLE_MALLOC,1,
                  [Define to 1 if you want to use simple mallocs instead of memory chunks])
fi

# --disable-charsets
AC_ARG_ENABLE(charsets,
              AS_HELP_STRING([--disable-charsets],
                             [exclude charset conversion codes from the library]))
if test x$enable_charsets = xno; then
	FRIBIDI_CHARSETS=0
else
	FRIBIDI_CHARSETS=1
fi
AC_SUBST(FRIBIDI_CHARSETS)
AM_CONDITIONAL(FRIBIDI_CHARSETS, test x$FRIBIDI_CHARSETS = x1)

# --without-glib
AC_ARG_WITH(glib,
              AS_HELP_STRING([--without-glib],
                             [do not use Glib]))
GLIB_PACKAGE=glib-2.0
GLIB_LIBS=
GLIB_CFLAGS=
if test x$with_glib = xyes; then
	PKG_CHECK_MODULES(GLIB,$GLIB_PACKAGE)
	FRIBIDI_USE_GLIB=1
else
if test x$with_glib = xno; then
	FRIBIDI_USE_GLIB=0
else
	PKG_CHECK_MODULES(GLIB,$GLIB_PACKAGE,
		FRIBIDI_USE_GLIB=1,
		FRIBIDI_USE_GLIB=0)
fi
fi
if test x$FRIBIDI_USER_GLIB = x0; then
	GLIB_PACKAGE=
fi
AC_SUBST(GLIB_PACKAGE)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(FRIBIDI_USE_GLIB)
AM_CONDITIONAL(FRIBIDI_USE_GLIB, test "$FRIBIDI_USE_GLIB" = 1)

# Generate output
AC_CONFIG_FILES([fribidi.pc
                 fribidi.spec
                 lib/fribidi-config.h
                 Makefile
		 charset/Makefile
		 gentable/Makefile
                 lib/Makefile
                 bin/Makefile
		 doc/Makefile
                 test/Makefile])
AC_OUTPUT
