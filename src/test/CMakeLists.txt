# Locate GTest and include
set(GTEST_INCLUDE "../../googletest/googletest/include")
set(GTEST_LIBRARY "../../build/lib")
include_directories(${GTEST_INCLUDE})
link_directories(${GTEST_LIBRARY})

# Locate bdd package
set(BDD_INCLUDE "../")
set(BDD_LIBRARY "../../build/lib")

# Define application
set(TEST_APP_NAME bddTestSuite)
add_executable(${TEST_APP_NAME} Tests.cpp)

# Set app suffix to .bin
set_target_properties (
   ${TEST_APP_NAME}
   PROPERTIES
      VERSION ${TST_VERSION}
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "../../build/bin"
)

# Link with gtest and bdd lib
target_link_libraries(${TEST_APP_NAME} gtest gtest_main bddPkg)
