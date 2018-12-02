/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.cpp
 * \author vdscp_8
*/
/// Header
#include "Manager.h"
//! Constructor
/*!
    Initializes uniqueTable
*/
Manager::Manager(void)
{
    uniqueTable = std::unordered_map<BDD_ID, Node*>();
    // Insert 0 and configure
    createVar("0");
    uniqueTable[0]->highId = 0;
    uniqueTable[0]->lowId  = 0;
    uniqueTable[0]->topVar = 0;
    // Insert 1 and configure
    createVar("1");
    uniqueTable[1]->highId = 1;
    uniqueTable[1]->lowId  = 1;
    uniqueTable[1]->topVar = 1;
}
//! True
/*!
    Returns the ID of the node representing True
*/
const BDD_ID &Manager::True(void)
{
    return trueId;
}
//! False
/*!
    Returns the ID of the node representing False
*/
const BDD_ID &Manager::False(void)
{
    return falseId;
}
//! isConstant
/*!
    Returns true if x is a leaf node
*/
bool Manager::isConstant(const BDD_ID x)
{
    if(x == trueId || x == falseId) {
        return true;
    }
    return false;
}
//! isVariable
/*!
    Returns true if x is a variable
*/
bool Manager::isVariable(const BDD_ID x)
{
    if(x == trueId || x == falseId) {
        return false;
    }
    return true;
}
/*! createVar
    Creates a new variable for the BDD
*/
BDD_ID Manager::createVar(const std::string &label)
{
    Node *newNode = new Node(currentId, 0, 0, 0, label);
    // Insert new node to map
    uniqueTable.insert({currentId, newNode});
    return currentId++;
}
/*! uniqueTableSize
    Returns the number of the nodes currently exist in the unique table
*/
std::size_t Manager::uniqueTableSize(void)
{
  return 0;
}
//! topVar
    /*!
        Returns the ID of top variable of the BDD node f
    */
BDD_ID Manager::topVar(const BDD_ID f)
{
    return -1;
}
