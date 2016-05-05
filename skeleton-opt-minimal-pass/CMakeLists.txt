# cmake file

# build tool configuration

cmake_minimum_required(VERSION 3.2.0)

project(HelloPass)

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

find_package(LLVM REQUIRED CONFIG)

if(${LLVM_FOUND})
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
else()
  message(FATAL_ERROR "Could not find LLVM")
endif()


# project configuration

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(NOT ${LLVM_ENABLE_RTTI})
  message(STATUS "RTTI support: disabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

if(NOT ${LLVM_ENABLE_EH})
  message(STATUS "EH support: disabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif()


#

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(LLVMHelloPass MODULE HelloPass.cpp)

