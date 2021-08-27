include(Headers.cmake)
install(FILES ${libfribidi_headers} DESTINATION include/fribidi)

if(FRIBIDI_CHARSETS)
    set(maybe_target_objects_libfridibi-char-sets $<TARGET_OBJECTS:libfribidi-char-sets>)
else()
    set(maybe_target_objects_libfridibi-char-sets "")
endif()

set(tab_files
    ${PROJECT_BINARY_DIR}/include/arabic-shaping.tab.i
    ${PROJECT_BINARY_DIR}/include/bidi-type.tab.i
    ${PROJECT_BINARY_DIR}/include/joining-type.tab.i
    ${PROJECT_BINARY_DIR}/include/mirroring.tab.i
)

set_source_files_properties(
    ${PROJECT_BINARY_DIR}/include/fribidi-unicode-version.h
    ${tab_files}
    PROPERTIES GENERATED ON)

add_library(libfribidi
# fribidi header files
  fribidi-arabic.h
  fribidi-begindecls.h
  fribidi-bidi.h
  fribidi-bidi-types.h
  fribidi-bidi-types-list.h
  fribidi-char-sets.h
  fribidi-char-sets-list.h
  fribidi-common.h
  fribidi-deprecated.h
  fribidi-enddecls.h
  fribidi-flags.h
  fribidi-joining.h
  fribidi-joining-types.h
  fribidi-joining-types-list.h
  fribidi-mirroring.h
  fribidi-brackets.h
  fribidi-shape.h
  fribidi-types.h
  fribidi-unicode.h
  fribidi.h
# fribidi source files
  fribidi.c
  fribidi-arabic.c
  fribidi-bidi.c
  fribidi-bidi-types.c
  fribidi-char-sets.c
  fribidi-char-sets-cap-rtl.c
  fribidi-char-sets-cp1255.c
  fribidi-char-sets-cp1256.c
  fribidi-char-sets-iso8859-6.c
  fribidi-char-sets-iso8859-8.c
  fribidi-char-sets-utf8.c
  fribidi-deprecated.c
  fribidi-joining.c
  fribidi-joining-types.c
  fribidi-mirroring.c
  fribidi-brackets.c
  fribidi-run.c
  fribidi-shape.c

       
        ${libfribidi_headers}
        ${PROJECT_BINARY_DIR}/include/fribidi-config.h
        ${tab_files}

)

add_dependencies(libfribidi build-gen-tab)

include(GenerateExportHeader)
generate_export_header(libfribidi
    BASE_NAME fribidi
    EXPORT_MACRO_NAME FRIBIDI_ENTRY
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/fribidi-export.h
)
install(FILES ${PROJECT_BINARY_DIR}/include/fribidi-export.h DESTINATION include/fribidi)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(libfribidi PUBLIC FRIBIDI_STATIC_DEFINE;FRIBIDI_BUILD)
endif()

if(UNIX OR BUILD_SHARED_LIBS)
    set_target_properties(libfribidi PROPERTIES OUTPUT_NAME fribidi)
endif()

if(UNIX)
    set_target_properties(libfribidi PROPERTIES
        VERSION ${FRIBIDI_MAJOR_VERSION}.${LT_VERSION_INFO}
        SOVERSION ${FRIBIDI_MAJOR_VERSION}
        )
else()
    set_target_properties(libfribidi PROPERTIES
        VERSION ${FRIBIDI_VERSION})
endif()

target_include_directories(libfribidi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/charset>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS libfribidi EXPORT fribidi-config
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(EXPORT fribidi-config
    FILE fribidi-config.cmake
    DESTINATION lib/cmake/fribidi
    NAMESPACE fribidi::)
