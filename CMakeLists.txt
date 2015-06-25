cmake_minimum_required(VERSION 3.0)
project(fribidi)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(MSVC)
    add_definitions(/wd4996)
endif()

include(CMakePrintHelpers)

set(start_bracket "\\[")
set(start_paren "\\(")
set(no_end_bracket "[^]]")
set(no_start_bracket "[^[]")
file(STRINGS configure.ac configure.ac)
string(CONCAT ac_init_regex
    "AC_INIT${start_paren}${start_bracket}(${no_end_bracket}*)]${no_start_bracket}*" # AC_INIT([GNU FriBidi],
    "${start_bracket}(${no_end_bracket}*)]${no_start_bracket}*" # [http://bugs.freedesktop.org/enter_bug.cgi?product=fribidi],
    "${start_bracket}(${no_end_bracket}*)]${no_start_bracket}*" # [fribidi],
    "${start_bracket}(${no_end_bracket}*)]${no_start_bracket}*\\)" # [http://fribidi.org/])
    )
string(REGEX MATCH "${ac_init_regex}" _ "${configure.ac}")

set(PACKAGE "${CMAKE_MATCH_3}")
set(PACKAGE_NAME "${CMAKE_MATCH_1}")
set(PACKAGE_BUGREPORT "${CMAKE_MATCH_2}")

if(NOT PACKAGE OR NOT PACKAGE_NAME OR NOT PACKAGE_BUGREPORT)
    message(FATAL_ERROR "Can't parse AC_INIT from configure.ac, check file and regexes.")
endif()

string(REGEX MATCH "m4_define\\(fribidi_major_version[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_MAJOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "m4_define\\(fribidi_minor_version[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_MINOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "m4_define\\(fribidi_micro_version[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_MICRO_VERSION "${CMAKE_MATCH_1}")
set(FRIBIDI_VERSION "${FRIBIDI_MAJOR_VERSION}.${FRIBIDI_MINOR_VERSION}.${FRIBIDI_MICRO_VERSION}")

string(REGEX MATCH "m4_define\\(fribidi_interface_version[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_INTERFACE_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "m4_define\\(fribidi_interface_age[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_INTERFACE_AGE "${CMAKE_MATCH_1}")

string(REGEX MATCH "m4_define\\(fribidi_binary_age[^0-9]+([0-9]*)" _ "${configure.ac}")
set(FRIBIDI_BINARY_AGE "${CMAKE_MATCH_1}")

if(FRIBIDI_MAJOR_VERSION STREQUAL "" OR FRIBIDI_MINOR_VERSION STREQUAL "" OR FRIBIDI_MICRO_VERSION STREQUAL "" OR
    FRIBIDI_INTERFACE_VERSION STREQUAL "" OR FRIBIDI_INTERFACE_AGE STREQUAL "" OR FRIBIDI_BINARY_AGE STREQUAL "")
    message(FATAL_ERROR "Can't parse versions from configure.ac, check file and regexes.")
endif()

set(LT_VERSION_INFO "${FRIBIDI_INTERFACE_VERSION}.${FRIBIDI_INTERFACE_AGE}.${FRIBIDI_BINARY_AGE}")

option(FRIBIDI_DISABLE_CHARSETS "exclude charset conversion routines from library" OFF)
if(FRIBIDI_DISABLE_CHARSETS)
    set(FRIBIDI_CHARSETS 0)
else()
    set(FRIBIDI_CHARSETS 1)
endif()

set(FRIBIDI_USE_GLIB 0)

include(CheckTypeSize)
check_type_size(int SIZEOF_INT)
check_type_size(wchar_t SIZEOF_WCHAR_T)


configure_file(lib/fribidi-config.h.in include/fribidi-config.h)
install(FILES ${PROJECT_BINARY_DIR}/include/fribidi-config.h DESTINATION include/fribidi)

option(FRIBIDI_ENABLE_DEBUG "turn on debugging [default=ON]" ON)
if(FRIBIDI_ENABLE_DEBUG)
    set(DEBUG 1)
else()
    set(DEBUG 0)
endif()

include(CheckIncludeFile)
foreach(h asm/page.h inttypes.h memory.h stdint.h stdlib.h strings.h string.h sys/times.h)
    string(TOUPPER ${h} H)
    string(MAKE_C_IDENTIFIER ${H} H)
    check_include_file(${h} HAVE_${H})
endforeach()

set(HAVE_STRINGIZE 1) # if you find a system that do not support this, add test
set(STDC_HEADERS 1) # if you find a system that do not support this, add test

option(FRIBIDI_ENABLE_MALLOC "use simple malloc instead of memory chunks" OFF)
if(FRIBIDI_ENABLE_MALLOC)
    set(USE_SIMPLE_MALLOC 1)
else()
    set(USE_SIMPLE_MALLOC 0)
endif()

configure_file(cmake/config.h.in include/config.h)

add_definitions(-DHAVE_CONFIG_H)


if(FRIBIDI_GEN_TAB)
    # The gen.tab subdirectory will be built in a separate build tree
    # with the default CMake-generator of the platform.
    # Mainly because cross-compiling is easier that way.
    if(NOT DEFINED FRIBIDI_GENERATED_INCLUDE_DIR)
        message(FATAL_ERROR "FRIBIDI_GENERATED_INCLUDE_DIR is not defined.")
    endif()
    add_subdirectory(gen.tab)
else()
    # configure external project dir
    set(fribidi_gen_tab_binary_dir ${PROJECT_BINARY_DIR}/fribidi-gen.tab)
    file(MAKE_DIRECTORY ${fribidi_gen_tab_binary_dir})
    execute_process(
        WORKING_DIRECTORY ${fribidi_gen_tab_binary_dir}
        COMMAND ${CMAKE_COMMAND}
            -DCMAKE_BUILD_TYPE=Release
            -DFRIBIDI_GENERATED_INCLUDE_DIR=${PROJECT_BINARY_DIR}/include
            -DFRIBIDI_GEN_TAB=1
            ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
    )

    if(result)
        message(FATAL_ERROR "Configuring fribidi-gen.tab external project failed.")
    else()
        message(STATUS "Successful configuration of the fribidi-gen.tab external project.")
    endif()

    add_custom_target(build-gen-tab
        COMMAND ${CMAKE_COMMAND}
            --build ${fribidi_gen_tab_binary_dir}
            --config Release
        COMMENT "Running custom target: build-gen-tab"
        VERBATIM
    )

    enable_testing()

    foreach(subdir charset lib bin test)
        add_subdirectory(${subdir})
    endforeach()
endif()
