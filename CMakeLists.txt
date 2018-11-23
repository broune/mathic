cmake_minimum_required(VERSION 3.12)

project(mathic VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mathic
  src/mathic/Action.cpp
  src/mathic/BitTriangle.cpp
  src/mathic/BoolParameter.cpp
  src/mathic/CliParameter.cpp
  src/mathic/CliParser.cpp
  src/mathic/ColumnPrinter.cpp
  src/mathic/DivMask.cpp
  src/mathic/HelpAction.cpp
  src/mathic/IntegerParameter.cpp
  src/mathic/StringParameter.cpp
  src/mathic/Timer.cpp
  src/mathic/display.cpp
  src/mathic/error.cpp
  src/mathic/BinaryKDTree.h
  )

target_link_libraries(mathic memtailor)
target_include_directories(mathic
  PUBLIC
    src
    )
  
install(TARGETS mathic ARCHIVE DESTINATION lib)
install(FILES include/mathic.h DESTINATION include)
install(DIRECTORY src/mathic/
  DESTINATION include/mathic
  FILES_MATCHING PATTERN "*.h"
  PATTERN stdinc.h EXCLUDE
  )

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
  enable_testing()
  add_executable(mathic-gtests
    src/test/BitTriangle.cpp
    src/test/DivFinder.cpp
    src/test/HashTable.cpp
    src/test/PairQueue.cpp
    src/test/gtestInclude.cpp
    src/test/testMain.cpp
    )
  target_link_libraries(mathic-gtests mathic)
  endif()

################################
# add gtest testing ############
################################

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
endif()

target_include_directories(mathic-gtests
  PRIVATE
  ${googletest_SOURCE_DIR}/googletest/include
  ${googletest_SOURCE_DIR}/googletest/src
  ${googletest_SOURCE_DIR}/googletest
  )

add_test(NAME mathic-gtests
      COMMAND mathic-gtests
  )

