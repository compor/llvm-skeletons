# cmake file

# build tool configuration

cmake_minimum_required(VERSION 3.2)

project(DependentMinimal)


# requirements

find_package(SkeletonOptPass CONFIG REQUIRED)

if(NOT SkeletonOptPass_FOUND)
  message(ERROR "SkeletonOptPass could not be found")
endif()


# project configuration

add_executable(dependent-minimal
  dependent-minimal.cpp)

target_link_libraries(dependent-minimal ${SKELETONOPTPASS_LIBRARY})


