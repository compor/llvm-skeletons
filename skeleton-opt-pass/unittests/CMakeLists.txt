# cmake file

find_package(GTest)

if(NOT GTEST_FOUND)
  message(WARNING "Could not find GTest; skipping unit tests")

  return()
endif()


