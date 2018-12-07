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

	BDD_ID root=manager.ite(c,a,b);
	ASSERT_EQ(a,manager.coFactorTrue(root,c));
}
TEST(coFactorTrue,retCofacTrue){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorTrue(-10));
	ASSERT_EQ(1,manager.coFactorTrue(b));
	ASSERT_EQ(0,manager.coFactorTrue(0));

	BDD_ID root=manager.ite(c,a,b);
	ASSERT_EQ(5,manager.coFactorTrue(root));
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

	BDD_ID root=manager.ite(c,a,b);
	ASSERT_EQ(b,manager.coFactorFalse(root,c));
}

TEST(coFactorFalse,retCofacFalse){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	ASSERT_EQ(-2,manager.coFactorFalse(-10));
	ASSERT_EQ(0,manager.coFactorFalse(b));
	ASSERT_EQ(1,manager.coFactorFalse(1));

	BDD_ID root=manager.ite(c,a,b);
	ASSERT_EQ(7,manager.coFactorFalse(root));
}


TEST(findNodes,checkSetOfNodes){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	std::set<BDD_ID> nodes_of_root=std::set<BDD_ID>();
	const std::set<BDD_ID> empty=std::set<BDD_ID>();
	std::set<BDD_ID> One=std::set<BDD_ID>();
	One.insert(1);
	//5 is root node of ite(b,1,c)
	int content[]={0,1,5,4};
	std::set<BDD_ID> complex=std::set<BDD_ID>(content,content+4);
	BDD_ID root=manager.ite(c,a,b);


	manager.findNodes(-10,nodes_of_root);
	ASSERT_EQ(empty,nodes_of_root);
	manager.findNodes(1,nodes_of_root);
	ASSERT_EQ(One,nodes_of_root);
	nodes_of_root=std::set<BDD_ID>();
	manager.findNodes(5,nodes_of_root);
	ASSERT_EQ(complex,nodes_of_root);
}

TEST(findVars,checkSetOfNodes){
	Manager manager;

	const BDD_ID a=manager.createVar("a");
	const BDD_ID b=manager.createVar("b");
	const BDD_ID c=manager.createVar("c");

	std::set<BDD_ID> vars_of_root=std::set<BDD_ID>();
	const std::set<BDD_ID> empty=std::set<BDD_ID>();
	std::set<BDD_ID> One=std::set<BDD_ID>();
	One.insert(1);
	//5 is root node of ite(b,1,c)
	int content[]={0,1,2,4};
	std::set<BDD_ID> complex=std::set<BDD_ID>(content,content+4);
	BDD_ID root=manager.ite(a,c,1);


	manager.findVars(-10,vars_of_root);
	ASSERT_EQ(empty,vars_of_root);

	manager.findVars(1,vars_of_root);
	ASSERT_EQ(One,vars_of_root);

	vars_of_root=std::set<BDD_ID>();
	manager.findVars(root,vars_of_root);
	ASSERT_EQ(complex,vars_of_root);
}

TEST(neg,retNegNot){
	Manager manager;

	const BDD_ID a = manager.createVar("a");
	const BDD_ID b = manager.createVar("b");

	ASSERT_EQ(-2,manager.neg(-20));

	ASSERT_EQ(0,manager.neg(1));

	BDD_ID notB = manager.neg(b);
	ASSERT_EQ(0,manager.coFactorTrue(notB));
	ASSERT_EQ(1,manager.coFactorFalse(notB));

	//the BDD for a and b
	BDD_ID AandB = manager.ite(a,b,0);
	BDD_ID notAandB = manager.ite(a,notB,1);
	ASSERT_EQ(notAandB,manager.neg(AandB));

}

TEST(and2,Constants){
	Manager manager;

	const BDD_ID a = manager.createVar("a");
	const BDD_ID b = manager.createVar("b");

	ASSERT_EQ(-2,manager.and2(-20,1));

	ASSERT_EQ(0,manager.and2(1,0));
	ASSERT_EQ(1,manager.and2(1,1));

	ASSERT_EQ(0,manager.and2(0,a));
}

TEST(and2,assoitiv){
	Manager manager;

	const BDD_ID a = manager.createVar("a");
	const BDD_ID b = manager.createVar("b");

	BDD_ID AandB=manager.and2(a,b);
	BDD_ID BandA=manager.and2(b,a);
	ASSERT_EQ(AandB,BandA);
	ASSERT_EQ(a,manager.and2(1,a));

	BDD_ID notB = manager.neg(b);
	ASSERT_EQ(0,manager.and2(b,notB));
}

TEST(and2,distributive){
	Manager manager;

	const BDD_ID a = manager.createVar("a");
	const BDD_ID b = manager.createVar("b");
	const BDD_ID c = manager.createVar("c");

	BDD_ID AorB=manager.ite(a,1,b);
	BDD_ID AorC=manager.ite(a,1,c);
	BDD_ID BandC=manager.ite(b,c,0);
	BDD_ID erg=manager.ite(a,1,BandC);

	ASSERT_EQ(erg,manager.and2(AorB,AorC));
}



/// main
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
