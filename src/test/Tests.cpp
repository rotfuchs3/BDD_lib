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
    ASSERT_EQ(4, manager.createVar("varC"));
}
/**
* \brief Test for uniqueTableSize()
*/
TEST(uniqueTableSize, tableSize) {
    Manager manager;
    // initial table size should be 2 due to the false and true nodes.
    ASSERT_EQ(2, manager.uniqueTableSize());
    // insert something, size of 5 after this
    manager.createVar("varA");
    manager.createVar("varB");
    manager.createVar("varC");
    ASSERT_EQ(5, manager.uniqueTableSize());
}
/**
* \brief Test for topVar, should return the top variable for a given BDD_ID
*/
TEST(topVar, retTopVariable) {
    Manager manager;
    ASSERT_EQ(0, manager.topVar(0));
    ASSERT_EQ(1, manager.topVar(1));
}

/**
 * \brief Test for ITE, terminal case, already constained,
 */
TEST(ite,terminalCases){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(a,manager.ite(1,a,b));
	ASSERT_EQ(b,manager.ite(0,a,b));
	ASSERT_EQ(a,manager.ite(c,a,a));
}

/**
 * \brief Test for ITE, terminal case, already constained,
 */
TEST(ite,iteOnConstantError){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.ite(a,b,-10));
	ASSERT_EQ(2,manager.ite(a,1,0));
}

TEST(ite,InternalNode){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(c+1,manager.ite(a,1,b));
	//already the node contained
	ASSERT_EQ(c+1,manager.ite(a,1,b));
	ASSERT_EQ(c+1,manager.ite(a,a,b));
	ASSERT_EQ(c+5,manager.ite(c,a,b));
	ASSERT_EQ(c+8,manager.ite(c,b,a));
}

TEST(coFactorTrueX,retCofacTrueX){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorTrue(-10,a));
	ASSERT_EQ(0,manager.coFactorTrue(0,b));
	ASSERT_EQ(b,manager.coFactorTrue(b,a));
	ASSERT_EQ(1,manager.coFactorTrue(a,a));
}
TEST(coFactorTrue,retCofacTrue){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorTrue(-10));
	ASSERT_EQ(1,manager.coFactorTrue(b));
	ASSERT_EQ(0,manager.coFactorTrue(0));
}

TEST(coFactorFalseX,retCofacFalseX){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorFalse(-10,a));
	ASSERT_EQ(1,manager.coFactorFalse(1,b));
	ASSERT_EQ(b,manager.coFactorFalse(b,a));
	ASSERT_EQ(0,manager.coFactorFalse(a,a));
}

TEST(coFactorFalse,retCofacFalse){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorFalse(-10));
	ASSERT_EQ(0,manager.coFactorFalse(b));
	ASSERT_EQ(1,manager.coFactorFalse(1));
}

/// main
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
