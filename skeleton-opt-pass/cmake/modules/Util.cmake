# cmake file

function(fatal message_txt)
  message(FATAL_ERROR "${message_txt}")
endfunction()


function(debug message_txt)
  message(STATUS "[DEBUG] ${message_txt}")
endfunction()


macro(msg mode)
  message(${mode} "${PRJ_NAME} ${ARGN}")
endmacro()


function(get_version version)
  execute_process(COMMAND git describe --tags --long --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE ver
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(${version} "${ver}" PARENT_SCOPE)
endfunction()


function(attach_compilation_db_command trgt)
  if(NOT TARGET ${trgt})
    fatal("cannot attach custom command to non-target: ${trgt}")
  endif()

  set(file "compile_commands.json")

  add_custom_command(TARGET ${trgt} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different ${file} "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM)
endfunction()


function(set_policies)
  math(EXPR is_even "${ARGC} % 2")
  math(EXPR upper "${ARGC} - 1")

  if(NOT is_even EQUAL 0)
    fatal("set_policies requires an even number of arguments")
  endif()

  foreach(idx RANGE 0 ${upper} 2)
    set(plc "${ARGV${idx}}")

    math(EXPR nxt_idx "${idx} + 1")
    set(newval "${ARGV${nxt_idx}}")

    if(POLICY ${plc})
      cmake_policy(GET ${plc} oldval)

      if(NOT oldval EQUAL newval)
        cmake_policy(SET "${plc}" "${newval}")

        msg(STATUS "policy ${plc}: ${newval}")
      endif()
    else()
      msg(WARNING "policy ${plc} is not defined")
    endif()
  endforeach()
endfunction()

