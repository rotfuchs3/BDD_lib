/**
 * \defgroup TestSuite
 * \brief Source for BDD package test suite
 * \file Tests.cpp
 * \author vdscp_8
*/
#include <gtest/gtest.h>
#include "Manager.h"
/// Global manager instance used for tests
Manager g_manager;
/** 
 * \brief Test for trueId = 1
 */
TEST(True, trueIdTest) {
  ASSERT_EQ(1, g_manager.True());
}
/**
 * \brief Test falseId = 0
 */
TEST(False, falseIdTest) {
  ASSERT_EQ(0, g_manager.False());
}
/**
 * \brief Should return true if provided a leaf node, false and true nodes are terminal by default
 */
TEST(isConstant, isNodeLeaf) {
  ASSERT_EQ(true, g_manager.isConstant(0));
  ASSERT_EQ(true, g_manager.isConstant(1));
}
/**
 * \brief Should return true if provided a variable BDD_ID
 */
TEST(isVariable, isNodeVariable) {
  ASSERT_EQ(true, g_manager.isVariable(2));
  ASSERT_EQ(true, g_manager.isVariable(9000));
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
