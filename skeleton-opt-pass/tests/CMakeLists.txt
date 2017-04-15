
include(FindPythonModule)

find_python_module(lit REQUIRED)

if(NOT EXISTS ${LLVM_TOOLS_BINARY_DIR}/FileCheck)
  message(WARNING "Could not find FileCheck")

  return()
endif()


