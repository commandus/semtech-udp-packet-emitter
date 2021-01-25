SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

commoncppflags = -Ithird_party -Igen

GEN_CPP = gen/ns/ns.pb.cpp gen/common/common.pb.cpp gen/gw/gw.pb.cpp gen/ns/profiles.pb.cpp \
	gen/ns/ns.grpc.pb.cpp gen/ns/profiles.grpc.pb.cpp \
	gen/as/external/api/application.pb.cpp gen/as/external/api/application.grpc.pb.cpp \
	gen/as/external/api/device.pb.cpp gen/as/external/api/device.grpc.pb.cpp \
	gen/as/external/api/frameLog.pb.cpp \
	gen/google/api/annotations.pb.cpp gen/google/api/http.pb.cpp

GEN_H = gen/ns/ns.pb.h gen/common/common.pb.h gen/gw/gw.pb.h gen/ns/profiles.pb.h \
	gen/as/external/api/application.pb.h \
	gen/as/external/api/device.pb.h \
	gen/as/external/api/frameLog.pb.h \
	gen/google/api/annotations.pb.h gen/google/api/http.pb.h

gengrpcs = $(GEN_CPP) $(GEN_H)
PROTOS = \
	proto/ns/ns.proto \
	proto/common/common.proto \
	proto/gw/gw.proto \
	ns/profiles.proto \
	proto/as/external/api/application.proto \
	proto/as/external/api/device.proto \
	proto/as/external/api/frameLog.proto \
	proto/google/api/annotations.proto proto/google/api/http.proto 

# generate Protobuf C++ sources
# rename .cc to .cpp
# generate gRPC C++ sources
$(gengrpcs): Makefile
	protoc -Iproto -I. --cpp_out gen $(PROTOS)
	mv gen/ns/ns.pb.cc gen/ns/ns.pb.cpp
	mv gen/common/common.pb.cc gen/common/common.pb.cpp
	mv gen/gw/gw.pb.cc gen/gw/gw.pb.cpp
	mv gen/ns/profiles.pb.cc gen/ns/profiles.pb.cpp
	mv gen/as/external/api/application.pb.cc gen/as/external/api/application.pb.cpp
	mv gen/as/external/api/device.pb.cc gen/as/external/api/device.pb.cpp
	mv gen/google/api/annotations.pb.cc gen/google/api/annotations.pb.cpp
	mv gen/google/api/http.pb.cc gen/google/api/http.pb.cpp
	mv gen/as/external/api/frameLog.pb.cc gen/as/external/api/frameLog.pb.cpp
	protoc -Iproto -I. --grpc_out gen --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(PROTOS)
	mv gen/ns/ns.grpc.pb.cc gen/ns/ns.grpc.pb.cpp
	mv gen/ns/profiles.grpc.pb.cc gen/ns/profiles.grpc.pb.cpp
	mv gen/as/external/api/application.grpc.pb.cc gen/as/external/api/application.grpc.pb.cpp
	mv gen/as/external/api/device.grpc.pb.cc gen/as/external/api/device.grpc.pb.cpp

GEN = $(gengrpcs)

AM_CXXFLAGS=-std=c++11 -O0
COMMON_CPP_FLAGS = -D_GLIBCXX_USE_CXX11_ABI=0
LIB_PROTOBUF = -L/usr/local/lib -lprotobuf

#
#	Exclude generated sources from distribution
#
#nodist_semtech_udp_packet_emitter_SOURCES = $(GEN_CPP)
BUILT_SOURCES = $(GEN)
CLEANFILES = $(GEN)

#
# Binaries
#
bin_PROGRAMS = semtech-udp-packet-emitter chirpstack-ns-log

nobase_dist_include_HEADERS = \
	platform.h utilstring.h utildate.h strptime.h \
	errlist.h daemonize.h aes-128.h \
	third_party/argtable3/argtable3.h \
	third_party/base64/base64.h

commonlibs =

semtech_udp_packet_emitter_SOURCES = \
	semtech-udp-packet-emitter.cpp errlist.cpp daemonize.cpp \
	utilstring.cpp utildate.cpp strptime.cpp utillora.cpp aes-128.c \
	third_party/argtable3/argtable3.c \
	third_party/base64/base64.cpp

semtech_udp_packet_emitter_LDADD = $(commonlibs)
semtech_udp_packet_emitter_CPPFLAGS = $(commoncppflags)

chirpstack_ns_log_SOURCES = \
	chirpstack-ns-log.cpp errlist.cpp daemonize.cpp \
	utilstring.cpp utildate.cpp utillora.cpp aes-128.c \
	chirpstack-ns-client.cpp strptime.cpp \
	$(GEN_CPP) \
	third_party/argtable3/argtable3.c \
	third_party/base64/base64.cpp
chirpstack_ns_log_LDADD = $(commonlibs) $(LIB_PROTOBUF) -lgrpc++
chirpstack_ns_log_CPPFLAGS = $(commoncppflags)
#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
   autogen.sh CMakeLists.txt CODE_OF_CONDUCT.md CONTRIBUTING.md COPYING HISTORY LICENSE README.md TODO \
   third_party/argtable3/README


test_frm_payload_SOURCES = \
	tests/test-frm-payload.cpp \
	utillora.cpp utildate.cpp utilstring.cpp aes-128.c \
	third_party/base64/base64.cpp
test_frm_payload_LDADD = $(commonlibs)
test_frm_payload_CPPFLAGS = $(commoncppflags)

check_PROGRAMS = test-frm-payload

TESTS = $(check_PROGRAMS)
