# this script must be started from the tests directory
# options
#
# - testcase: the file name of the input test file (e.g. "test_CapRTL_explicit.input")
# - TARGET_FILE_FRIBIDI: location of the fribidi executable
# - WORKING_DIRECTORY: the absolute path to the current directory which must be the tests directory

if(TARGET_FILE_LIBFRIBIDI MATCHES ".dll$")
  get_filename_component(libfribidi_dll_dir "${TARGET_FILE_LIBFRIBIDI}" DIRECTORY)
  file(TO_CMAKE_PATH "$ENV{PATH}" env_path)
  list(INSERT env_path 0 "${libfribidi_dll_dir}")
  file(TO_NATIVE_PATH "${env_path}" env_path)
  set(ENV{PATH} "${env_path}")
endif()

get_filename_component(test "${testcase}" NAME_WE)
string(REGEX MATCH "^test_([^_]+)_" _ "${test}")
if(NOT CMAKE_MATCH_1)
  message(FATAL_ERROR "test file name has invalid format: ${testcase}, can't parse charset.")
endif()
set(charset "${CMAKE_MATCH_1}")
message(STATUS "=== ${test} ===")

file(WRITE .run.tests.cmake.empty_file "")
execute_process(COMMAND ${TARGET_FILE_FRIBIDI} --charset "${charset}"
  INPUT_FILE .run.tests.cmake.empty_file
  RESULT_VARIABLE result)
file(REMOVE .run.tests.cmake.empty_file)
if(result)
  message(STATUS "[Character set not supported]")
else()
  execute_process(COMMAND ${TARGET_FILE_FRIBIDI} --test --charset "${charset}" "${testcase}"
    OUTPUT_FILE .run.tests.cmake.output_file
    RESULT_VARIABLE result)

  string(REGEX REPLACE "\\.input$" ".reference" reference "${testcase}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files .run.tests.cmake.output_file "${reference}"
    RESULT_VARIABLE result)
  file(REMOVE .run.tests.cmake.output_file)
  if(result)
    message(FATAL_ERROR "[Failed]")
  else()
    message(STATUS "[Passed]")
  endif()
endif()

