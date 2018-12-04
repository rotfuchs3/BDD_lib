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
set(TST_VERSION_MINOR "7")
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
set(LIB_HEADERS  "${TOP_LEVEL}/src/ManagerInterface.h"  "${TOP_LEVEL}/src/Manager.h")
set(LIB_SOURCES "${TOP_LEVEL}/src/Manager.cpp")
set(APP_SOURCES "${TOP_LEVEL}/src/main.cpp")

# Define library
set(LIBRARY_NAME bddPkg)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set lib properties
set_target_properties(
  ${LIBRARY_NAME}
  PROPERTIES
     SUFFIX "_v${VERSION_MAJOR}_${PKG_VERSION_MAJOR}_${PKG_VERSION_MINOR}.a"
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
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} pthread)

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