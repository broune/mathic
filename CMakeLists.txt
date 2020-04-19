cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mathic VERSION 1.0 LANGUAGES CXX)

find_package(Threads 2.1 REQUIRED QUIET)
find_package(Memtailor   REQUIRED)



include(CheckIncludeFiles)
check_include_files("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)
check_include_files(sys/types.h	HAVE_SYS_TYPES_H)
check_include_files(sys/stat.h	HAVE_SYS_STAT_H)
check_include_files(stdlib.h	HAVE_STDLIB_H)
check_include_files(string.h	HAVE_STRING_H)
check_include_files(memory.h	HAVE_MEMORY_H)
check_include_files(strings.h	HAVE_STRINGS_H)
check_include_files(inttypes.h	HAVE_INTTYPES_H)
check_include_files(stdint.h	HAVE_STDINT_H)
check_include_files(unistd.h	HAVE_UNISTD_H)
check_include_files(dlfcn.h	HAVE_DLFCN_H)

# set_property(TARGET mathic PROPERTY CXX_STANDARD 11)
add_compile_options(
  -g -O2
  -DPACKAGE_NAME="${PROJECT_NAME}"
  -DPACKAGE_TARNAME="${PROJECT_NAME}"
  -DPACKAGE_VERSION="${PROJECT_VERSION}"
  -DPACKAGE_STRING="${PROJECT_NAME} ${PROJECT_VERSION}"
  -DPACKAGE_BUGREPORT=""
  -DPACKAGE_URL=""
  -DPACKAGE="${PROJECT_NAME}"
  -DVERSION="${PROJECT_VERSION}"
  # TODO
  -DLT_OBJDIR=".libs/"
)

add_library(mathic STATIC
  src/mathic.h
  src/mathic.cpp
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
install(FILES README.md lgpl-2.0.txt lgpl-2.1.txt lgpl-3.0.txt
  DESTINATION licenses/mathic
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

option(enable_divsim "Build divsim" OFF)
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

option(BUILD_TESTING "Build the tests" ON)
if(BUILD_TESTING)
  enable_testing()
  add_executable(mathic-gtests
    src/test/BitTriangle.cpp
    src/test/DivFinder.cpp
    src/test/HashTable.cpp
    src/test/PairQueue.cpp
    src/test/testMain.cpp
    )

  ################################
  # add gtest testing ############
  ################################
  find_package(GTest)
  include(GoogleTest)
  gtest_discover_tests(mathic-gtests)
  add_test(NAME mathic-gtests COMMAND mathic-gtests)

  if(FALSE AND GTEST_FOUND)
    target_link_libraries(mathic-gtests mathic GTest::GTest GTest::Main)
    target_include_directories(mathic-gtests PRIVATE ${GTEST_INCLUDE_DIR})
  else()
    include(FetchContent)
    FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.10.0
      )
    FetchContent_MakeAvailable(googletest)

    target_link_libraries(mathic-gtests mathic gtest)
    target_include_directories(mathic-gtests PRIVATE
      ${googletest_SOURCE_DIR}/googletest/include
      ${googletest_SOURCE_DIR}/googletest/src
      ${googletest_SOURCE_DIR}/googletest
      )
  endif()
endif()
