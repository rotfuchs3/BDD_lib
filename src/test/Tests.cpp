/**
 * \defgroup TestSuite
 * \brief Source for BDD package test suite
 * \file Tests.cpp
 * \author vdscp_8
*/
#include <gtest/gtest.h>
#include "Manager.h"
Manager manager;
/** 
 * \brief At init, BDD_ID for true nodes is set to '1'
 */
TEST(True, trueId) { 
  ASSERT_EQ(1, True());
}
/**
 * \brief At init, BDD_ID for false nodes is set to '0'
 */
TEST(False, falseId) {
  ASSERT_EQ(0, False());
}
/**
 * \brief Should return true if provided a leaf node, false and true nodes are terminal by default
 */
TEST(isConstant, isNodeLeaf) {
  ASSERT_EQ(true, isConstant(0));
  ASSERT_EQ(true, isConstsnt(1));
}
/**
 * \brief Should return true if provided a variable node, 
 */
TEST(isVariable, isNodeVariable) {
  Node var1(2, 0, 1, 3, "var1");
  Node var2(3, 0, 1, 4, "var2");
  ASSERT_EQ(true, isVariable(var1.myId));
  ASSERT_EQ(true, isVariable(var2.myId));
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
