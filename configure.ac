AC_PREREQ(2.60)
AC_INIT([axa], [1.0.0])
AC_CONFIG_SRCDIR([axa/axa.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability silent-rules subdir-objects])
AC_PROG_CC_STDC
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])
LT_INIT

my_CFLAGS=" \
-Wextra -Wall -Wbad-function-cast -Wcast-align -Wmissing-noreturn \
-Wnested-externs -Wpointer-arith -Wshadow -Wwrite-strings \
-Wdeclaration-after-statement -Wswitch-enum \
-Wunused \
-Wunreachable-code \
-Wuninitialized \
-Wno-unknown-pragmas \
-Wmissing-declarations -Wmissing-prototypes \
-Wsign-compare -Wchar-subscripts \
-Wstrict-prototypes -Wshadow \
-Wformat-security"
AC_SUBST([my_CFLAGS])

AC_CONFIG_HEADERS(config.h)

PKG_PROG_PKG_CONFIG
if test -n "$PKG_CONFIG"; then
    if $PKG_CONFIG --variable=pc_path pkg-config 2>/dev/null | grep -q /libdata/; then
        PKG_INSTALLDIR(['${prefix}/libdata/pkgconfig'])
    else
        PKG_INSTALLDIR
    fi
fi

MY_CHECK_LIBPCAP

MY_CHECK_LIBPROTOBUF_C

save_LIBS="$LIBS"
AC_SEARCH_LIBS([strlcpy],
               [bsd],
               [strlcpy_LIBS="$LIBS"],
               [AC_MSG_ERROR([could not find library containing strlcpy])]
              )
LIBS="$save_LIBS"
AC_SUBST(strlcpy_LIBS)

PKG_CHECK_MODULES([libedit], [libedit], [], [MY_CHECK_LIBEDIT])

PKG_CHECK_MODULES([libnmsg], [libnmsg >= 0.8.0])

PKG_CHECK_MODULES([libwdns], [libwdns])

AC_CHECK_HEADER([zlib.h], [], [ AC_MSG_ERROR([required header file not found]) ])
AC_CHECK_LIB([z], [deflate], [], [ AC_MSG_ERROR([required library not found]) ])

AC_PATH_PROG([DOXYGEN], [doxygen])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
if test -n "$DOXYGEN"; then
        DOC_HTML_MSG="yes, doxygen available: $DOXYGEN"
        AC_MSG_CHECKING(doxygen version)
        MY_CHECK_DOXYGEN
        AC_MSG_RESULT($DOXYGEN_version)
        if test $DOXYGEN_version == "oldish"; then
            AC_MSG_WARN(old Doxygen version found API documentation may not render as intended consider upgrading to version 1.8.3 or newer)
        fi
else
        DOC_HTML_MSG="no, doxygen not available"
fi

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$libnmsg_CFLAGS"
AC_CHECK_HEADERS([nmsg/base/dnsqr.pb-c.h nmsg/base/encode.pb-c.h nmsg/base/packet.pb-c.h nmsg/sie/newdomain.pb-c.h],
                 [],
                 [AC_MSG_ERROR([required header file not found])])
CPPFLAGS="$save_CPPFLAGS"

AX_DEFINE_DIR([CONFDIR], [sysconfdir/axa], [config dir for axa])
AC_DEFINE_UNQUOTED(FIELDS_FILE,"$CONFDIR/fields",[Fields file])

AC_DEFINE_UNQUOTED(AXA_PVERS_STR,"version $PACKAGE_VERSION", [Version])

AC_CONFIG_FILES([Makefile axa/libaxa.pc doc/doxygen/Doxyfile])
AC_OUTPUT
AC_MSG_RESULT([
    $PACKAGE $VERSION

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}
        libs:                   ${LIBS}

        prefix:                 ${prefix}
        sysconfdir:             ${sysconfdir}
        libdir:                 ${libdir}
        includedir:             ${includedir}
        pkgconfigdir:           ${pkgconfigdir}

        building html docs:     ${DOC_HTML_MSG} 
])