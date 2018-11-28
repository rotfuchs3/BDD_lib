cmake_minimum_required(VERSION 2.8)

project(globalPrj)

# C++11
set(CMAKE_CXX_STANDARD 11)

# Create output directories
file(MAKE_DIRECTORY "bin")
file(MAKE_DIRECTORY "lib")

# Set the bdd pkg version number
# VERSION_MAJOR: Overall code version, both bdd and tests have the same major
set(VERSION_MAJOR "0")
set(PKG_VERSION_MAJOR "0")
set(PKG_VERSION_MINOR "1")
set(PKG_VERSION ${VERSION_MAJOR}.${PKG_VERSION_MAJOR}.${PKG_VERSION_MINOR})

# Set the bdd tests version number
set(TST_VERSION_MAJOR "0")
set(TST_VERSION_MINOR "4")
set(TST_VERSION ${VERSION_MAJOR}.${TST_VERSION_MAJOR}.${TST_VERSION_MINOR})

# Generate version files
configure_file("build_versions.h.in" "build_versions.h" @ONLY)

# Add googletest
add_subdirectory(../googletest/googletest gtestbuild)

# Add BDD package source and its tests
add_subdirectory(../src bddbuild)
add_subdirectory(../src/test testsbuild)
