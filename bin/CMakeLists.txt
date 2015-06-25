set(getopt_SOURCES getopt.c getopt1.c getopt_int.h getopt.h gettext.h)

link_libraries(libfribidi)

add_executable(fribidi fribidi-main.c ${getopt_SOURCES})
add_executable(fribidi-benchmark fribidi-benchmark.c ${getopt_SOURCES})
add_executable(fribidi-bidi-types fribidi-bidi-types.c)

if(FRIBIDI_CHARSETS)
    add_executable(fribidi-caprtl2utf8 fribidi-caprtl2utf8.c)
endif()

install(TARGETS fribidi DESTINATION bin)

