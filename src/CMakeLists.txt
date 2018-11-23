cmake_minimum_required(VERSION 2.8)
 
project(TEST CXX C)
 
# Locate GTest
set(GTEST_INCLUDE_DIRS "../gtest/include")
set(GTEST_LIBRARY "../gtest/lib")
# g++ flag for C++11
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 ${LLVM_CXX_FLAGS} ")

# include GTEST libs/includes
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBRARY})

# Link TEST with what we want to test and the GTest and pthread library
add_executable(TEST main.cpp)
target_link_libraries(TEST gtest gtest_main pthread) 
# Set suffix to .out
set_target_properties (
	TEST
	PROPERTIES
		SUFFIX ".out"
)