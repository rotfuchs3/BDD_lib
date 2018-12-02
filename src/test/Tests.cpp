/**
 * \defgroup TestSuite
 * \brief Source for BDD package test suite
 * \file Tests.cpp
 * \author vdscp_8
*/
/// GoogleTest
#include <gtest/gtest.h>
/// Lib header
#include "Manager.h"

/** 
* \brief Test for trueId = 1
*/
TEST(True, trueIdTest) {
    Manager manager;
    ASSERT_EQ(1, manager.True());
}
/**
* \brief Test falseId = 0
*/
TEST(False, falseIdTest) {
    Manager manager;
    ASSERT_EQ(0, manager.False());
}
/**
* \brief Should return true if provided a leaf node, false and true nodes are terminal by default
*/
TEST(isConstant, isNodeLeaf) {
    Manager manager;
    ASSERT_EQ(true, manager.isConstant(0));
    ASSERT_EQ(true, manager.isConstant(1));
}
/**
* \brief Should return true if provided a variable BDD_ID
*/
TEST(isVariable, isNodeVariable) {
    Manager manager;
    ASSERT_FALSE(manager.isVariable(0));
    ASSERT_EQ(true, manager.isVariable(2));
    ASSERT_EQ(true, manager.isVariable(9000));
}
/**
* \brief Test for createVar from label, return BDD_ID, 0 and 1 are false and true BDD_IDs, so next should be 2, 3, 4, etc...
*/
TEST(createVar, createVarRetBDD) {
    Manager manager;
    ASSERT_EQ(2, manager.createVar("varA"));
    ASSERT_EQ(3, manager.createVar("varB"));
    ASSERT_EQ(4, manager.createVar("varC"));
}

/// main
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
