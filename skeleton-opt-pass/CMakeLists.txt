# cmake file

# build tool configuration

cmake_minimum_required(VERSION 3.2.0)

project(SkeletonOptPass)

# TODO need to move this to top-level cmake list file
# CMP0056 is required for any libc++abi dependency of the try_compile() command
# this bumps required version to 3.2
# otherwise LD_LIBRARY_DIR needs to be set
set(PROJECT_POLICIES CMP0056)

foreach(PROJECT_POLICY ${PROJECT_POLICIES})
  if(POLICY ${PROJECT_POLICY})
    cmake_policy(GET ${PROJECT_POLICY} POLICY_VALUE)
    message("policy ${PROJECT_POLICY}: ${POLICY_VALUE}")
  endif()
endforeach()


# requirements

message(STATUS "Detecting Doxygen dependency")

find_package(Doxygen)

if(${DOXYGEN_FOUND})
  message(STATUS "Found Doxygen ${DOXYGEN_VERSION}")

  set(DOXYGEN_CFG_FILE_TEMPLATE
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen.cfg.in")
  set(DOXYGEN_CFG_FILE "${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen.cfg")

  configure_file(${DOXYGEN_CFG_FILE_TEMPLATE} ${DOXYGEN_CFG_FILE} @ONLY)

  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CFG_FILE}
    WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation" VERBATIM)
else()
  message(STATUS "Could not find Doxygen")
endif()

#

message(STATUS "Detecting LLVM dependency")

find_package(LLVM REQUIRED CONFIG)

if(${LLVM_FOUND})
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
else()
  message(FATAL_ERROR "Could not find LLVM")
endif()

#

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


# project configuration

#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(NOT ${LLVM_ENABLE_RTTI})
  message(STATUS "RTTI support: disabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

if(NOT ${LLVM_ENABLE_EH})
  message(STATUS "EH support: disabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif()

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


#

include(CMakePackageConfigHelpers)

string(TOUPPER ${PROJECT_NAME} PROJECT_CMAKE_MODULE_PREFIX)



# TODO better way to deduce library name, especially when using llvm cmake
# functions
get_target_property(PROJECT_LIB_NAMES ${LIB_NAME} NAME)
set(PROJECT_LIB_NAMES "${PROJECT_LIB_NAMES} ${LIB_NAME}.so lib${LIB_NAME}.so")

set(PROJECT_CMAKE_MODULE_SUBDIR "cmake/modules")
set(PROJECT_CMAKE_MODULE_SOURCE_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CMAKE_MODULE_SUBDIR}")

configure_file(${PROJECT_CMAKE_MODULE_SOURCE_DIR}/SkeletonOptPassConfig.cmake.in
  "share/${PROJECT_NAME}/cmake/${PROJECT_NAME}Config.cmake"
  @ONLY)

write_basic_package_version_file(
  "share/${PROJECT_NAME}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "0.0.0"
  COMPATIBILITY SameMajorVersion)

# installation

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION "lib")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "include")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/share" DESTINATION .)

