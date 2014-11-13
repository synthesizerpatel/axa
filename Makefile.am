bin_PROGRAMS =
check_PROGRAMS =
TESTS =
EXTRA_DIST =
CLEANFILES =
DISTCLEANFILES =
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

AM_CPPFLAGS = \
        -include $(top_builddir)/config.h \
        -I${top_srcdir}/axa
AM_CFLAGS = \
	${my_CFLAGS}
AM_LDFLAGS = \
	${my_LDFLAGS}

configdir = ${CONFDIR}
dist_config_DATA = etc/fields

#
##
### documentation 
##
#

MAN_PROCESS = $(AM_V_GEN) soelim -I. -I`dirname $<` -r $< | sed -e "s/^\.Dd.*/.Dd `date '+%B %d, %Y'`/" >$@
.man.1:
	    $(MAN_PROCESS)

sratool/sratool.1: sratool/sratool.man \
	fwd.man watch.man srawatch.man sratool/options.man sratool/cmds.man
sratool/radtool.1: sratool/radtool.man \
	fwd.man watch.man radwatch.man sratool/options.man sratool/cmds.man
sratunnel/sratunnel.1: sratunnel/sratunnel.man fwd.man srawatch.man watch.man

EXTRA_DIST += \
	sratool/sratool.man \
	sratool/radtool.man \
	sratunnel/sratunnel.man \
	fwd.man \
	watch.man \
	srawatch.man \
	radwatch.man \
	sratool/options.man \
	sratool/cmds.man \
	README.md \
	doc/doxygen/doxygen-input-filter

dist_man_MANS =  \
	sratool/sratool.1 \
	sratool/radtool.1 \
	sratunnel/sratunnel.1
CLEANFILES += \
	sratool/sratool.1 \
	sratool/radtool.1 \
	sratunnel/sratunnel.1

#
##
### library
##
#

LIBAXA_CURRENT=0
LIBAXA_REVISION=0
LIBAXA_AGE=0

nobase_include_HEADERS = \
	axa/axa.h \
	axa/bits.h \
	axa/client.h \
	axa/dns_walk.h \
	axa/axa_endian.h \
	axa/fields.h \
	axa/open_nmsg_out.h \
	axa/protocol.h \
	axa/rad_mod.h \
	axa/socket.h \
	axa/trie.h \
	axa/wire.h
lib_LTLIBRARIES = axa/libaxa.la

axa_libaxa_la_SOURCES = \
	axa/axa.h \
	axa/bits.h \
	axa/client.h \
	axa/dns_walk.h \
	axa/axa_endian.h \
	axa/fields.h \
	axa/open_nmsg_out.h \
	axa/protocol.h \
	axa/rad_mod.h \
	axa/socket.h \
	axa/trie.h \
	axa/wire.h \
	axalib/client.c \
	axalib/dns_walk.c \
	axalib/domain_to_str.c \
	axalib/emsg.c \
	axalib/fields.c \
	axalib/get_field_name.c \
	axalib/hash_divisor.c \
	axalib/open_nmsg_out.c \
	axalib/parse_ch.c \
	axalib/socket.c \
	axalib/time.c \
	axalib/trie.c \
	axalib/wdns_res.c \
	axalib/wdns_rtype.c \
	axalib/whit2nmsg.c \
	axalib/wire.c

axa_libaxa_la_LDFLAGS = $(AM_LDFLAGS) \
	-version-info $(LIBAXA_CURRENT):$(LIBAXA_REVISION):$(LIBAXA_AGE) \
	-export-symbols-regex "^(axa_[a-z].*)" \
	${libpcap_LIBS} \
	${libwdns_LIBS} \
	${libnmsg_LIBS} \
	${strlcpy_LIBS}
axa_libaxa_la_CFLAGS = \
	${AM_CFLAGS} \
	${libpcap_CFLAGS} \
	${libwdns_CFLAGS} \
	${libnmsg_CFLAGS} \
	${strlcpy_CFLAGS}

pkgconfig_DATA = axa/libaxa.pc
EXTRA_DIST += axa/libaxa.pc.in
DISTCLEANFILES += axa/libaxa.pc

#
##
### tests
##
#

# there are no tests

#
##
### programs 
##
#

bin_PROGRAMS += sratool/sratool
sratool_sratool_SOURCES = \
	sratool/main.c \
	sratool/print.c \
	sratool/sratool.h
sratool_sratool_LDADD = \
	axa/libaxa.la \
	${libprotobuf_c_LIBS} \
	${libpcap_LIBS} \
	${libnmsg_LIBS} \
	${libz_LIBS} \
	${libwdns_LIBS} \
	${libedit_LIBS}
sratool_sratool_CFLAGS = \
	${AM_CFLAGS} \
	${libprotobuf_c_CFLAGS} \
	${libpcap_CFLAGS} \
	${libnmsg_CFLAGS} \
	${libz_CFLAGS} \
	${libwdns_CFLAGS} \
	${libedit_CFLAGS}

bin_PROGRAMS += sratool/radtool
sratool_radtool_SOURCES = ${sratool_sratool_SOURCES}
sratool_radtool_LDADD = ${sratool_sratool_LDADD}
sratool_radtool_CFLAGS = ${sratool_sratool_CFLAGS}

bin_PROGRAMS += sratunnel/sratunnel
sratunnel_sratunnel_SOURCES = \
	sratunnel/main.c
sratunnel_sratunnel_LDADD = \
	axa/libaxa.la \
	${libnmsg_LIBS} \
	${libpcap_LIBS}
sratunnel_sratunnel_CFLAGS = \
	${AM_CFLAGS} \
	${libnmsg_CFLAGS} \
	${libpcap_CFLAGS}

#
##
### documentation 
##
#

exampledir = ${docdir}/examples
nobase_example_DATA = \
	sratool/main.c \
	sratool/print.c \
	sratool/sratool.h \
	sratool/README \
	sratunnel/main.c \
	sratunnel/README

EXTRA_DIST += \
	autogen.sh \
	copyright \
	axalib/README \
	axa/README \
	sratool/README \
	sratunnel/README

if HAVE_DOXYGEN
html-local: doc/doxygen/Doxyfile
	$(AM_V_GEN) cd doc/doxygen && $(DOXYGEN)
endif

doc: html-local

spotless: distclean
	rm -fr ar-lib aclocal.m4 autom4te.cache configure config.* Makefile.in \
	autoscan.log config.h compile depcomp configure.scan       			   \
	doc/doxygen/Doxyfile missing install-sh ltmain.sh
