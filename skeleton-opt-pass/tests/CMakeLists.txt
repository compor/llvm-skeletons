
include(FindPythonModule)

find_python_module(lit)

if(NOT PY_LIT_FOUND)
  message(WARNING "Could not find lit; skipping tests")

  return()
endif()

if(NOT EXISTS ${LLVM_TOOLS_BINARY_DIR}/FileCheck)
  message(WARNING "Could not find FileCheck; skipping tests")

  return()
endif()

set(PRJ_TEST_CONFIG_FILE "lit.cfg")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PRJ_TEST_CONFIG_FILE}.in"
  ${PRJ_TEST_CONFIG_FILE})

add_custom_target(tests
    COMMAND ${PYTHON_EXECUTABLE} -m lit.main "${CMAKE_CURRENT_BINARY_DIR}" -v
    DEPENDS ${TESTEE_LIB})

add_custom_command(TARGET tests PRE_BUILD COMMAND 
  ${CMAKE_COMMAND} -E copy_directory 
  "${CMAKE_CURRENT_SOURCE_DIR}/data/" ${CMAKE_CURRENT_BINARY_DIR})

