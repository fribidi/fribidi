math(EXPR last_arg_idx "${CMAKE_ARGC}-1")
foreach(i RANGE 1 ${last_arg_idx})
    if("${CMAKE_ARGV${i}}" STREQUAL "-P")
        math(EXPR first_arg_idx "${i}+2")
        break()
    endif()
endforeach()

# parse args
if(first_arg_idx GREATER last_arg_idx) # no args after cmake ... -P <path>
    message(FATAL_ERROR "Missing command: -P <script-name> must be followed by the command to execute and optional args for the command.")
endif()

set(COMMAND "")
foreach(i RANGE ${first_arg_idx} ${last_arg_idx})
    list(APPEND COMMAND "${CMAKE_ARGV${i}}")
endforeach()

if(DEFINED OUTPUT_FILE)
    set(maybe_output_file_args OUTPUT_FILE "${OUTPUT_FILE}")
endif()

execute_process(COMMAND ${COMMAND}
    RESULT_VARIABLE result
    ${maybe_output_file_args})

if(result)
    message(FATAL_ERROR "RESULT_VARIABLE: ${result}")
endif()

