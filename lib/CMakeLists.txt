set(LIB_SOURCE_PATH ${CMAKE_SOURCE_DIR}/lib/src)

add_library(disfslib ${LIB_SOURCE_PATH}/connection.c)

target_include_directories(disfslib PUBLIC include/)

find_package(cmocka REQUIRED)

add_executable(simple_test tests/simple_test.c)
target_link_libraries(simple_test cmocka::cmocka disfslib)

add_test(NAME simple_test COMMAND simple_test)
