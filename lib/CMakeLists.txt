set(LIB_SOURCE_PATH ${CMAKE_SOURCE_DIR}/lib/src)

add_library(disfslib ${LIB_SOURCE_PATH}/connection.c
  ${LIB_SOURCE_PATH}/udp_discovery.c
  ${LIB_SOURCE_PATH}/file_menager.c)

target_include_directories(disfslib PUBLIC include/)

option(BUILD_TESTS ON)

if(BUILD_TESTS)
find_package(cmocka REQUIRED)

add_executable(simple_test tests/simple_test.c)
target_link_libraries(simple_test cmocka::cmocka disfslib)

add_test(NAME simple_test COMMAND simple_test)

add_executable(udp_packet_test tests/udp_packet_test.c)
target_link_libraries(udp_packet_test cmocka::cmocka disfslib)

add_test(NAME udp_packet_test COMMAND udp_packet_test)

endif()
