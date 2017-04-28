# cmake file

# requirements

find_package(GTest)

if(NOT GTEST_FOUND)
  message(WARNING "Could not find GTest; skipping unit tests")

  return()
endif()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

# configuration

# TODO better native filename deduction
get_property(lib_prefix TARGET ${TESTEE_LIB} PROPERTY PREFIX)
get_property(lib_suffix TARGET ${TESTEE_LIB} PROPERTY SUFFIX)

if(NOT lib_prefix)
  set(lib_prefix "lib")
endif()

if(NOT lib_suffix)
  set(lib_suffix ".so")
endif()

set(TESTEE_LIB_FILENAME "${lib_prefix}${TESTEE_LIB}${lib_suffix}")

# aggregate unit test targets under a pseudo-target
add_custom_target(unittests)

set(PRJ_TEST_NAMES TestDummy)

foreach(prj_test_name ${PRJ_TEST_NAMES})
  set(TEST_SOURCES "${prj_test_name}.cpp")

  add_executable(${prj_test_name} ${TEST_SOURCES})

  target_compile_definitions(${prj_test_name} PUBLIC ${LLVM_DEFINITIONS})

  target_include_directories(${prj_test_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${prj_test_name} PUBLIC ${GTEST_INCLUDE_DIRS})
  target_include_directories(${prj_test_name} PUBLIC ${LLVM_INCLUDE_DIRS})
  target_include_directories(${prj_test_name} PUBLIC ${Boost_INCLUDE_DIRS})
  target_include_directories(${prj_test_name} PUBLIC 
    "${CMAKE_SOURCE_DIR}/include")

  target_link_libraries(${prj_test_name} PUBLIC ${GTEST_BOTH_LIBRARIES})  
  target_link_libraries(${prj_test_name} PUBLIC ${CMAKE_THREAD_LIBS_INIT})  

  llvm_map_components_to_libnames(llvm_libs 
    core support asmparser analysis passes)

  target_link_libraries(${prj_test_name} PUBLIC ${llvm_libs})

  set_property(TARGET ${prj_test_name} APPEND_STRING PROPERTY 
    LINK_FLAGS " -Wl,--warn-unresolved-symbols")

  # setup dependency between unit test targets and their production code
  add_dependencies(${prj_test_name} ${TESTEE_LIB})

  # exclude unit test targets from main build
  set_target_properties(${prj_test_name} PROPERTIES EXCLUDE_FROM_ALL TRUE)

  add_dependencies(unittests ${prj_test_name})

  get_property(PRJ_UNIT_TESTS_EXE TARGET ${prj_test_name} PROPERTY NAME)
endforeach()


set(PRJ_UNIT_TESTS_SCRIPT "run_unit_tests.sh")
get_property(PRJ_UNIT_TESTS_TARGET TARGET unittests PROPERTY NAME)

configure_file("${CMAKE_SOURCE_DIR}/utils/scripts/${PRJ_UNIT_TESTS_SCRIPT}.in" 
    "${CMAKE_BINARY_DIR}/${PRJ_UNIT_TESTS_SCRIPT}"
    @ONLY)

