set(gen-unicode-version_SOURCES gen-unicode-version.c)
set(gen-bidi-type-tab_SOURCES gen-bidi-type-tab.c packtab.c packtab.h)
set(gen-joining-type-tab_SOURCES gen-joining-type-tab.c packtab.c packtab.h)
set(gen-arabic-shaping-tab_SOURCES gen-arabic-shaping-tab.c)
set(gen-mirroring-tab_SOURCES gen-mirroring-tab.c packtab.c packtab.h)

include_directories(
    ../lib
    ${PROJECT_BINARY_DIR}/include # this must be before FRIBIDI_GENERATED_INCLUDE_DIR
    # because when cross-compiling, the first one contains the generated
    # config files which correspond to the host
    ${FRIBIDI_GENERATED_INCLUDE_DIR})

foreach(t gen-bidi-type-tab gen-joining-type-tab gen-arabic-shaping-tab gen-mirroring-tab)
    list(APPEND ${t}_SOURCES ${FRIBIDI_GENERATED_INCLUDE_DIR}/fribidi-unicode-version.h)
endforeach()
foreach(t gen-unicode-version gen-bidi-type-tab gen-joining-type-tab gen-arabic-shaping-tab gen-mirroring-tab)
    add_executable(${t} ${${t}_SOURCES})
endforeach()

set(COMPRESSION 2)

# generate bidi-type.tab.i

set(output ${FRIBIDI_GENERATED_INCLUDE_DIR}/bidi-type.tab.i)
set(depends
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/UnicodeData.txt)
add_custom_command(
    OUTPUT ${output}
    COMMAND gen-bidi-type-tab 
        ${COMPRESSION}
        ${depends}
        > ${output}
    DEPENDS ${depends}
    COMMENT "Generate bidi-type.tab.i"
    VERBATIM
)

# generate joining-type.tab.i

set(output ${FRIBIDI_GENERATED_INCLUDE_DIR}/joining-type.tab.i)
set(depends
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/UnicodeData.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/ArabicShaping.txt)
add_custom_command(
    OUTPUT ${output}
    COMMAND gen-joining-type-tab
        ${COMPRESSION}
        ${depends}
        > ${output}
    DEPENDS ${depends}
    COMMENT "Generate joining-type.tab.i"
    VERBATIM
)

# generate arabic-shaping.tab.i

set(output ${FRIBIDI_GENERATED_INCLUDE_DIR}/arabic-shaping.tab.i)
set(depends
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/UnicodeData.txt)
add_custom_command(
    OUTPUT ${output}
    COMMAND gen-arabic-shaping-tab
        ${COMPRESSION}
        ${depends}
        > ${output}
    DEPENDS ${depends}
    COMMENT "Generate arabic-shaping.tab.i"
    VERBATIM
)

# generate mirroring.tab.i

set(output ${FRIBIDI_GENERATED_INCLUDE_DIR}/mirroring.tab.i)
set(depends
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/BidiMirroring.txt)
add_custom_command(
    OUTPUT ${output}
    COMMAND gen-mirroring-tab
        ${COMPRESSION}
        ${depends}
        > ${output}
    DEPENDS ${depends}
    COMMENT "Generate mirroring.tab.i"
    VERBATIM
)

# generate fribidi-unicode-version.h

set(output ${FRIBIDI_GENERATED_INCLUDE_DIR}/fribidi-unicode-version.h)
set(depends
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/ReadMe.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/unidata/BidiMirroring.txt)
add_custom_command(
    OUTPUT ${output}
    COMMAND gen-unicode-version
        ${depends}
        > ${output}
    DEPENDS ${depends}
    COMMENT "Running gen.tabs custom command to generate fribidi-unicode-version.h"
    VERBATIM
)

add_custom_target(gen-unicode-version-target ALL
    DEPENDS
        ${FRIBIDI_GENERATED_INCLUDE_DIR}/fribidi-unicode-version.h
    COMMENT "Running custom target: gen.tabs/gen-unicode-version-target"
    VERBATIM
    )

add_custom_target(gen-tabs-target ALL
    DEPENDS
        ${FRIBIDI_GENERATED_INCLUDE_DIR}/bidi-type.tab.i
        ${FRIBIDI_GENERATED_INCLUDE_DIR}/joining-type.tab.i
        ${FRIBIDI_GENERATED_INCLUDE_DIR}/arabic-shaping.tab.i
        ${FRIBIDI_GENERATED_INCLUDE_DIR}/mirroring.tab.i
    COMMENT "Running custom target: gen.tabs/gen-tabs-target"
    VERBATIM
    )
