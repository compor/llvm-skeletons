# cmake file

# build tool configuration

cmake_minimum_required(VERSION 3.2.0)

enable_testing()

# add project cmake module path
set(PRJ_CMAKE_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH ${PRJ_CMAKE_MODULE_DIR})

include(CMakePackageConfigHelpers)
include(Util)
include(HandleLLVMDependency)

option(PRJ_USE_LLVM_INTERNAL_MODULE TRUE)

get_version(PRJ_VERSION)

set(PRJ_NAME SkeletonOpt)

# part of LLVM or standalone
set(PRJ_STANDALONE_BUILD FALSE)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(${PRJ_NAME} C CXX)

  set(PRJ_STANDALONE_BUILD TRUE)

  msg(STATUS "configuring as external LLVM project")
endif()


# CMP0056 is required for any libc++abi dependency of the try_compile() command
# this bumps required version to 3.2
# otherwise LD_LIBRARY_DIR needs to be set
set_policies(CMP0056 NEW)


# requirements

handle_llvm_dependency(${PRJ_STANDALONE_BUILD})


# project configuration

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

msg(STATUS "RTTI support: ${LLVM_ENABLE_RTTI}")

if(NOT LLVM_ENABLE_EH)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif()

msg(STATUS "EH support: ${LLVM_ENABLE_EH}")


#

set(LIB_NAME "LLVM${PRJ_NAME}Pass")
set(LIB_SOURCES "lib/SkeletonOptPass.cpp")

if(NOT PRJ_USE_LLVM_INTERNAL_MODULE)
  add_library(${LIB_NAME} MODULE ${LIB_SOURCES})
else()
  # this follows the suggestion documented in
  # http://llvm.org/docs/CMake.html#cmake-out-of-source-pass
  # for allowing future inclusion of the plugin in the LLVM source tree
  # by using LLVM's internal cmake functions and utilities
  set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin)
  set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib)

  add_llvm_loadable_module(${LIB_NAME} ${LIB_SOURCES})
endif()

# according to http://llvm.org/docs/CMake.html
# we do not need the below library dependencies since the plugin will be loaded
# via opt or clang which already have those libs in their dependencies
# comment out the following two statements to test against static lib LLVM build

#llvm_map_components_to_libnames(llvm_libs core support analysis ipo passes)
#target_link_libraries(${LIB_NAME} PUBLIC ${llvm_libs})

target_compile_definitions(${LIB_NAME} PUBLIC ${LLVM_DEFINITIONS})
target_compile_definitions(${LIB_NAME} PUBLIC VERSION_STRING=${PRJ_VERSION})

target_include_directories(${LIB_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
target_include_directories(${LIB_NAME} PUBLIC "include")

#

set(TESTEE_LIB ${LIB_NAME})
get_property(TESTEE_PREFIX TARGET ${TESTEE_LIB} PROPERTY PREFIX)
get_property(TESTEE_SUFFIX TARGET ${TESTEE_LIB} PROPERTY SUFFIX)

if(NOT TESTEE_PREFIX)
  set(TESTEE_PREFIX ${CMAKE_SHARED_MODULE_PREFIX})
endif()

if(NOT TESTEE_SUFFIX)
  set(TESTEE_SUFFIX ${CMAKE_SHARED_MODULE_SUFFIX})
endif()

add_subdirectory(unittests)
add_subdirectory(tests)
add_subdirectory(docs)


attach_compilation_db_command(${LIB_NAME})


#

configure_file(
  ${PRJ_CMAKE_MODULE_DIR}/SkeletonOptPassConfig.cmake.in
  "share/${PRJ_NAME}/cmake/${PRJ_NAME}Config.cmake"
  @ONLY)

# TODO parse project version from vcs
write_basic_package_version_file(
  "share/${PRJ_NAME}/cmake/${PRJ_NAME}ConfigVersion.cmake"
  VERSION "0.0.0"
  COMPATIBILITY SameMajorVersion)

# installation

set(PRJ_INSTALL_INCLUDE_DIR "include")
set(PRJ_INSTALL_SHARE_DIR "share")

if(NOT PRJ_STANDALONE_BUILD)
  set(PRJ_INSTALL_INCLUDE_DIR ${PRJ_NAME}${PRJ_INSTALL_INCLUDE_DIR})
  set(PRJ_INSTALL_SHARE_DIR ${PRJ_NAME}${PRJ_INSTALL_SHARE_DIR})
endif()

if(PRJ_STANDALONE_BUILD)
  install(TARGETS ${LIB_NAME} LIBRARY DESTINATION "lib")
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/share" DESTINATION share)

