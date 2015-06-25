cmake_minimum_required(VERSION 3.0)
project(cmake-testbuild)

find_package(fribidi REQUIRED)

add_executable(cmake-test main.c)
set_target_properties(cmake-test PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(cmake-test fribidi::libfribidi)

target_compile_definitions(cmake-test PRIVATE
    -DFRIBIDI_CMAKE_TEST_CONFIG=$<CONFIG>
    -DFRIBIDI_CMAKE_TEST_TARGET_FILE_NAME=$<TARGET_FILE_NAME:fribidi::libfribidi>
    -DFRIBIDI_CMAKE_TEST_TARGET_TYPE=$<TARGET_PROPERTY:fribidi::libfribidi,TYPE>
    )

install(TARGETS cmake-test DESTINATION bin)
