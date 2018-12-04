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
set(TOP_LEVEL ../)

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

# Print versions
message("${Green}BDD package version: ${PKG_VERSION}${ColourReset}")
message("${Green}BDD test version:    ${TST_VERSION}${ColourReset}")

# Generate version files
configure_file("build_versions.h.in" "build_versions.h" @ONLY)

# Add googletest
find_package(GTest)
if(NOT GTEST_FOUND)
    message("${Yellow}WARNING: GTest package was not found in your system!")
    message("Running 'git submodule init && git submodule update'${ColourReset}")
    execute_process(COMMAND "git submodule init"
                    COMMAND "git submodule update")
    add_subdirectory(../googletest/googletest gtestbuild)
endif()

# Add BDD package source and its tests
add_subdirectory(../src bddbuild)
add_subdirectory(../src/test testsbuild)

# Set source
set(LIB_HEADERS
  ManagerInterface.h
  Manager.h
)
set(LIB_SOURCES Manager.cpp)
set(APP_SOURCES main.cpp)

# Define library
set(LIBRARY_NAME bddPkg)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set lib properties
set_target_properties(
  ${LIBRARY_NAME}
  PROPERTIES
     VERSION ${PKG_VERSION}
     SUFFIX ".a"
     ARCHIVE_OUTPUT_DIRECTORY "../lib"
     LIBRARY_OUTPUT_DIRECTORY "../lib"
)

# Define application
set(APPLICATION_NAME bddPkgMain)
add_executable(${APPLICATION_NAME} ${APP_SOURCES})
# Set app suffix to .bin
set_target_properties (
   ${APPLICATION_NAME}
   PROPERTIES
      VERSION ${PKG_VERSION}
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "../bin"
)

# Locate BDD package
set(BDD_INCLUDE "../../src")
set(BDD_LIBRARY "../lib")
include_directories(${BDD_INCLUDE})
link_directories(${BDD_LIBRARY})

# Links target exe (bddpkgMain) with library (bddPkg)
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME})
