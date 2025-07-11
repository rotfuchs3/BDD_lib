cmake_minimum_required(VERSION 3.0.2)

project(globalPrj)

# Fancy stuff
if(NOT WIN32)
   string(ASCII 27 Esc)
   set(ColourReset "${Esc}[m")
   set(Red "${Esc}[31m")
   set(Green "${Esc}[32m")
   set(Yellow "${Esc}[33m")
   set(Blue "${Esc}[34m")
   set(Magenta "${Esc}[35m")
   set(Cyan "${Esc}[36m")
   set(White "${Esc}[37m")
endif()

# C++11
set(CMAKE_CXX_STANDARD 11)
# Compile/Linker flags
#set(CMAKE_CXX_FLAGS "-Wall -g -pg")
#set(CMAKE_EXE_LINKER_FLAGS "-g -pg")
#set(CMAKE_SHARED_LINKER_FLAGS "-g -pg")

# Set top level path
set(TOP_LEVEL ..)

# Set the bdd pkg version number
# VERSION_MAJOR: Overall code version, both bdd and tests have the same major
set(VERSION_MAJOR "0")
set(PKG_VERSION_MAJOR "0")
set(PKG_VERSION_MINOR "1")
set(PKG_VERSION ${VERSION_MAJOR}.${PKG_VERSION_MAJOR}.${PKG_VERSION_MINOR})

# Set the bdd tests version number
set(TST_VERSION_MAJOR "0")
set(TST_VERSION_MINOR "18")
set(TST_VERSION ${VERSION_MAJOR}.${TST_VERSION_MAJOR}.${TST_VERSION_MINOR})

# Generate version files
configure_file("build_versions.h.in" "build_versions.h" @ONLY)

#######################################
# ADD GTEST OR BUILD FROM SUBMODULE
#######################################
set(COLOR_GTEST "${Blue}G${Red}T${Yellow}e${Green}s${Red}t${ColourReset}")
message("${Yellow}Looking for ${COLOR_GTEST}...")
find_package(GTest)
if(NOT GTEST_FOUND)
    message("${Yellow}WARNING: ${COLOR_GTEST} ${Yellow}package was not found in your system!")
    message("Running 'git submodule init && git submodule update' ...${ColourReset}")
    execute_process(COMMAND git submodule init
                    COMMAND git submodule update)
    add_subdirectory(${TOP_LEVEL}/googletest/googletest gtestbuild)
    set(GTEST_INCLUDE "{TOP_LEVEL}/googletest/googletest/include")
    set(GTEST_LIB "{TOP_LEVEL}/build/lib")
 else()
    message("${Green}Found ${COLOR_GTEST}${Green} libraries in ${GTEST_BOTH_LIBRARIES}${ColourReset}")
    set(GTEST_INCLUDE "${GTEST_INCLUDE_DIRS}")
    set(GTEST_LIB "${GTEST_BOTH_LIBRARIES}")
endif()

##################################
# BUILD THE BDD LIBRARY AND MAIN
##################################
message("${Cyan}BDD PACKAGE v${PKG_VERSION}${ColourReset}")

# Set sources
set(LIB_HEADERS "${TOP_LEVEL}/src/ManagerInterface.h"  "${TOP_LEVEL}/src/Manager.h")
set(LIB_SOURCES "${TOP_LEVEL}/src/Manager.cpp")
set(APP_SOURCES "${TOP_LEVEL}/src/main.cpp")

# Define library
set(LIBRARY_NAME bddPkg)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set lib properties
set_target_properties(
  ${LIBRARY_NAME}
  PROPERTIES
     SUFFIX "_v${VERSION_MAJOR}_${PKG_VERSION_MAJOR}_${PKG_VERSION_MINOR}.so"
     ARCHIVE_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
     LIBRARY_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
)

# Define application - main.cpp
set(APPLICATION_NAME bddPkgMain)
add_executable(${APPLICATION_NAME} ${APP_SOURCES})
# Set app suffix to .bin
set_target_properties (
   ${APPLICATION_NAME}
   PROPERTIES
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

# Locate BDD package and link
set(BDD_INCLUDE "${TOP_LEVEL}/src")
set(BDD_LIBRARY "${TOP_LEVEL}/build/lib")
include_directories(${BDD_INCLUDE})
link_directories(${BDD_LIBRARY})

# Links target exe (bddpkgMain) with library (bddPkg)
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} bddPkg pthread)

##################################
# BUILD THE BDD Reachability
##################################

message("${Cyan}BDD Rechablity v${PKG_VERSION}${ColourReset}")

# Set sources
set(LIB_HEADERS "${TOP_LEVEL}/src/reachability/ReachableInterface.h"  "${TOP_LEVEL}/src/reachability/Reachable.h")
set(LIB_SOURCES "${TOP_LEVEL}/src/reachability/Reachable.cpp")
set(APP_SOURCES "${TOP_LEVEL}/src/reachability/main.cpp")

# Define library
set(LIBRARY_NAME bddReach)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set lib properties
set_target_properties(
        ${LIBRARY_NAME}
        PROPERTIES
        SUFFIX "_v${VERSION_MAJOR}_${PKG_VERSION_MAJOR}_${PKG_VERSION_MINOR}.so"
        ARCHIVE_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
        LIBRARY_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
)

# Define application - main.cpp
set(APPLICATION_NAME bddReachMain)
add_executable(${APPLICATION_NAME} ${APP_SOURCES})
# Set app suffix to .bin
set_target_properties (
        ${APPLICATION_NAME}
        PROPERTIES
        SUFFIX ".bin"
        RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

# Locate BDD reachability and link
set(BDD_reach_INCLUDE "${TOP_LEVEL}/src/reachability")
set(BDD_reach_LIBRARY "${TOP_LEVEL}/build/lib")
include_directories(${BDD_reach_INCLUDE})
link_directories(${BDD_reach_LIBRARY})

# Links target exe (bddreachMain) with library (bddPkg) and (Reachability)
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} bddReach bddPkg pthread)

############################
# BUILD THE BDD TEST SUITE
############################
message("${Cyan}BDD TEST SUITE v${TST_VERSION}${ColourReset}")

# Locate GTest and include
include_directories(${GTEST_INCLUDE})
link_directories(${GTEST_LIB})

# Define application
set(TEST_APP_NAME bddTestSuite)
add_executable(${TEST_APP_NAME} ${TOP_LEVEL}/src/test/Tests.cpp)

# Set app suffix to .bin
set_target_properties (
   ${TEST_APP_NAME}
   PROPERTIES
      SUFFIX "_v${VERSION_MAJOR}_${TST_VERSION_MAJOR}_${TST_VERSION_MINOR}.bin"
      RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

# Link with gtest and bdd lib
target_link_libraries(${TEST_APP_NAME} gtest gtest_main bddPkg pthread)

############################
# BUILD THE BDD Reach TEST SUITE
############################

message("${Cyan}BDD Reach TEST SUITE v${TST_VERSION}${ColourReset}")

# Define application
set(TEST_APP_NAME bddReachTestSuite)
add_executable(${TEST_APP_NAME} ${TOP_LEVEL}/src/test/reachability/main_test.cpp)

# Set app suffix to .bin
set_target_properties (
        ${TEST_APP_NAME}
        PROPERTIES
        SUFFIX "_v${VERSION_MAJOR}_${TST_VERSION_MAJOR}_${TST_VERSION_MINOR}.bin"
        RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

# Link with gtest and bdd lib
target_link_libraries(${TEST_APP_NAME} gtest gtest_main bddReach bddPkg pthread)

############################
# BUILD THE BENCHMARK TOOL
############################
message("${Cyan}BENCHMARK TOOL${ColourReset}")

# Build the benchmark library
# Set sources
set(BENCH_HEADERS 
	"${TOP_LEVEL}/src/bench/bench_grammar.hpp" 
	"${TOP_LEVEL}/src/bench/skip_parser.hpp")
set(BENCH_SOURCES 
	"${TOP_LEVEL}/src/bench/bench_circuit_manager.cpp"
	"${TOP_LEVEL}/src/bench/BenchmarkLib.cpp"
	"${TOP_LEVEL}/src/bench/circuit_to_BDD_manager.cpp"
	"${TOP_LEVEL}/src/bench/Dumper.cpp"
)

# Define library
set(BENCHMARK_LIB Benchmark)
add_library(${BENCHMARK_LIB} SHARED ${BENCH_SOURCES})

# Set lib properties
set_target_properties(
  ${BENCHMARK_LIB}
  PROPERTIES
     SUFFIX ".so"
     ARCHIVE_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
     LIBRARY_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/lib"
)

# Boost package find
find_package(Boost COMPONENTS system filesystem REQUIRED)
if(NOT Boost_FOUND)
    message("${Yellow}WARNING: Boost package was not found in your system!")
    message("Please install libboost-all-dev...${ColourReset}")
    return()
 else()
    message("${Green}Found Boost${Green} libraries in: ${Boost_LIBRARIES}${ColourReset}")
endif()

# Locate Boost lib and includes
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES})

# Define application
set(BENCH_APP "VDSProject_bench")
add_executable(${BENCH_APP} ${TOP_LEVEL}/src/bench/main_bench.cpp)

# Set app suffix to .bin
set_target_properties (
   ${BENCH_APP}
   PROPERTIES
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

# Link with benchmark tool, boost, and bdd lib
target_link_libraries(${BENCH_APP} bddPkg Benchmark ${Boost_LIBRARIES})
############################
# BUILD THE VERIFY TOOL
############################
message("${Cyan}VERIFY TOOL${ColourReset}")

# Exectuable
add_executable(VDSProject_verify ${TOP_LEVEL}/src/verify/main_verify.cpp)

# Set app suffix to .bin
set_target_properties (
   VDSProject_verify
   PROPERTIES
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "${TOP_LEVEL}/build/bin"
)

