file(GLOB tests RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" test_*.input)
foreach(testcase ${tests})
    string(REGEX REPLACE "\\.input$" "" name "${testcase}")
    add_test(NAME ${name}
        COMMAND ${CMAKE_COMMAND}
        -DTARGET_FILE_FRIBIDI=$<TARGET_FILE:fribidi>
        -DWORKING_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR}
        -Dtestcase=${testcase}
        -DTARGET_FILE_LIBFRIBIDI=$<TARGET_FILE:libfribidi>
        -P run.tests.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()


