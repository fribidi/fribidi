if(NOT FRIBIDI_CHARSETS)
    return()
endif()

install(
    FILES
        fribidi-char-sets.h
        fribidi-char-sets-list.h
    DESTINATION
        include/fribidi)

set_source_files_properties(
    ${PROJECT_BINARY_DIR}/include/fribidi-unicode-version.h
    PROPERTIES GENERATED ON)

add_library(libfribidi-char-sets OBJECT
    fribidi-char-sets.c
    fribidi-char-sets-cap-rtl.c
    fribidi-char-sets-cp1255.c
    fribidi-char-sets-cp1256.c
    fribidi-char-sets-iso8859-6.c
    fribidi-char-sets-iso8859-8.c
    fribidi-char-sets-utf8.c
    fribidi-char-sets-cap-rtl.h
    fribidi-char-sets-cp1255.h
    fribidi-char-sets-cp1256.h
    fribidi-char-sets-iso8859-6.h
    fribidi-char-sets-iso8859-8.h
    fribidi-char-sets-utf8.h
    ${PROJECT_BINARY_DIR}/include/fribidi-config.h
    ${PROJECT_BINARY_DIR}/include/fribidi-unicode-version.h
)

if(BUILD_SHARED_LIBS)
    # object libs doesn't get the proper value
    set_target_properties(libfribidi-char-sets PROPERTIES
        POSITION_INDEPENDENT_CODE True)
endif()

add_dependencies(libfribidi-char-sets build-gen-tab)

target_include_directories(libfribidi-char-sets
    PRIVATE
        ${PROJECT_SOURCE_DIR}/lib
        ${PROJECT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR})

# this object lib will be built as part of libfribidi
# so we need to set libfribidi_EXPORTS flag here
target_compile_definitions(libfribidi-char-sets PRIVATE libfribidi_EXPORTS)
