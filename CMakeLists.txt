cmake_minimum_required(VERSION 3.9)
project(semtech-udp-packet-emitter VERSION 1.0)

INCLUDE_DIRECTORIES("third_party")

set(SRC 
	semtech-udp-packet-emitter.cpp errlist.cpp daemonize.cpp utilstring.cpp utillora.cpp aes-128.c
	third_party/argtable3/argtable3.c
)
add_executable(semtech-udp-packet-emitter ${SRC})
# target_link_libraries(semtech-udp-packet-emitter ev)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/semtech-udp-packet-emitter DESTINATION cmake)
INSTALL(FILES README.md LICENSE HISTORY COPYING TODO CODE_OF_CONDUCT.md CONTRIBUTING.md DESTINATION doc)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${semtech_udp_packet_emitter_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${semtech_udp_packet_emitter_VERSION_MINOR}")
include(CPack)
