# cmake file

cmake_minimum_required(VERSION 3.2.0)

project(SkeletonOptPass)


# build tool configuration
# TODO need to move this to top-level cmake list file

set(PROJECT_POLICIES CMP0056)

# CMP0056 is required for any libc++abi dependency of the try_compile() command

foreach(PROJECT_POLICY ${PROJECT_POLICIES})
  if(POLICY ${PROJECT_POLICY})
    cmake_policy(GET ${PROJECT_POLICY} POLICY_VALUE)
    message("policy ${PROJECT_POLICY}: ${POLICY_VALUE}")
  endif()
endforeach()


# requirements

find_package(LLVM REQUIRED CONFIG)

if(${LLVM_FOUND})
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
else()
    message(FATAL_ERROR "Could not find LLVM")
endif()


#message(STATUS "Detecting external project dependency: ExtDepProject")

#if(IS_DIRECTORY ${EXTDEP_ROOT_DIR})
  #message(STATUS "Found ExtDepProject root directory: ${EXTDEP_ROOT_DIR}")
  #set(EXTDEP_FOUND ON)
  #set($EXTDEP_INCLUDE_DIR "${EXTDEP_ROOT_DIR}/include/")
  #set(EXTDEP_LIB_DIR "${EXTDEP_ROOT_DIR}/lib/")
  #set(EXTDEP_LIB "dswp")
#else()
  #message(WARNING "Could not find: ExtDepProject")
#endif()


# configuration


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

#if(${EXTDEP_FOUND})
#include_directories(${EXTDEP_INCLUDE_DIR})
  #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -L${EXTDEP_LIB_DIR}")

  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEXTDEP")
#endif()


add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

set(LIB_NAME "LLVMSkeletonOptPass")
set(LIB_SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/SkeletonOptPass.cpp")

if(NOT ${USE_LLVM_INTERNAL_MODULE_FUNCTION})
  add_library(${LIB_NAME} MODULE ${LIB_SOURCES})
else()
  # this follows the suggestion documented in
  # http://llvm.org/docs/CMake.html#cmake-out-of-source-pass
  # for allowing future inclusion of the plugin in the LLVM source tree
  # by using LLVM's internal cmake functions and utilities
  set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INT_DIR}/bin)
  set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INT_DIR}/lib)

  list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
  include(HandleLLVMOptions)
  include(AddLLVM)

  add_llvm_loadable_module(${LIB_NAME} ${LIB_SOURCES})
endif()

# according to
# http://llvm.org/docs/CMake.html
# we do not need the below library dependencies since the plugin will be loaded
# via opt or clang which already have those libs in their dependencies
# comment out the following two statements
# to test against static lib LLVM build
#llvm_map_components_to_libnames(llvm_libs core support passes)
#target_link_libraries(${LIB_NAME} ${llvm_libs})

#if(${EXTDEP_FOUND})
  #target_link_libraries(${LIB_NAME} ${EXTDEP_LIB})
#endif()


# installation

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION "lib")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "include")

