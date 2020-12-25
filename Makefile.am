SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS=-std=c++11 -O0

commoncppflags = -Ithird_party

#
#	Exclude generated sources from distribution
#
nodist_semtech_udp_packet_emitter_SOURCES = $(GEN)
BUILT_SOURCES = $(GEN)
CLEANFILES = $(GEN)

#
# Binaries
#
bin_PROGRAMS = semtech-udp-packet-emitter

nobase_dist_include_HEADERS = \
	platform.h utilstring.h utildate.h strptime.h \
	errlist.h daemonize.h aes-128.h \
	third_party/argtable3/argtable3.h \
	third_party/base64/base64.h

commonlibs = -L/usr/local/lib 

semtech_udp_packet_emitter_SOURCES = \
	semtech-udp-packet-emitter.cpp errlist.cpp daemonize.cpp \
	utilstring.cpp utildate.cpp strptime.cpp utillora.cpp aes-128.c \
	third_party/argtable3/argtable3.c \
	third_party/base64/base64.cpp

semtech_udp_packet_emitter_LDADD = $(commonlibs)
semtech_udp_packet_emitter_CPPFLAGS = $(commoncppflags)

#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
   autogen.sh CMakeLists.txt CODE_OF_CONDUCT.md CONTRIBUTING.md COPYING HISTORY LICENSE README.md TODO \
   third_party/argtable3/README

check_PROGRAMS =

TESTS = $(check_PROGRAMS)
