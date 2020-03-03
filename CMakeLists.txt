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
  src/mathic/stdinc.h
  )

find_package(Threads 2.1 REQUIRED QUIET)
find_package(Memtailor   REQUIRED)

target_link_libraries(mathic ${MEMTAILOR_LIBRARIES} Threads::Threads)
target_include_directories(mathic
  PUBLIC
    src
    ${MEMTAILOR_INCLUDE_DIR}
  )
  
install(TARGETS mathic ARCHIVE DESTINATION lib)
install(FILES src/mathic.h DESTINATION include)
install(DIRECTORY src/mathic/
  DESTINATION include/mathic
  FILES_MATCHING PATTERN "*.h"
#  PATTERN stdinc.h EXCLUDE
  )

option(enable_pqsim "Build pqsim" OFF)
if(enable_pqsim)
add_executable(pqsim
  src/pqsim/pqMain.h         src/pqsim/pqMain.cpp
  src/pqsim/Item.h           src/pqsim/Item.cpp
  src/pqsim/Model.h          src/pqsim/Model.cpp
  src/pqsim/Simulator.h      src/pqsim/Simulator.cpp
  src/pqsim/GeobucketModel.h
  src/pqsim/HeapModel.h
  src/pqsim/StlSetModel.h
  src/pqsim/TourTreeModel.h
  src/pqsim/stdinc.h
  )
target_link_libraries(pqsim mathic)
endif()

option(enable_divsim "Build pqsim" OFF)
if(enable_divsim)
add_executable(divsim
  src/divsim/divMain.h       src/divsim/divMain.cpp 
  src/divsim/Simulation.h    src/divsim/Simulation.cpp
  src/divsim/DivListModel.h
  src/divsim/KDTreeModel.h
  src/divsim/Monomial.h
  src/divsim/stdinc.h
  )
target_link_libraries(divsim mathic)
endif()

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
endif()
