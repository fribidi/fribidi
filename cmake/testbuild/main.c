#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fribidi/fribidi.h"

#define S(x) SS(x)
#define SS(x) #x

int main(int argc, char* argv[]) {

    printf("Fribidi CMake test, %s build\n",
        S(FRIBIDI_CMAKE_TEST_CONFIG));
    printf("-- linked to %s which is a %s\n",
        S(FRIBIDI_CMAKE_TEST_TARGET_FILE_NAME),
        S(FRIBIDI_CMAKE_TEST_TARGET_TYPE));

    const char* np = "<null-pointer>";
    printf("-- fribidi_unicode_version: %s\n", fribidi_unicode_version ? fribidi_unicode_version : np);
    const char* atn = fribidi_get_bidi_type_name(fribidi_get_bidi_type('A'));
    printf("-- bidi type name of 'A': %s\n", atn ? atn : np);
    if(fribidi_version_info && fribidi_unicode_version && atn && strcmp(atn, "LTR") == 0) {
        printf("-- test passed.\n");
        return EXIT_SUCCESS;
    } else {
        printf("-- test failed.\n");
        return EXIT_FAILURE;
    }
}
