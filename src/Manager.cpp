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
    lookUpTable = std::unordered_map<std::string,BDD_ID>();
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
	std::unordered_map<std::string,BDD_ID>::const_iterator item=lookUpTable.find(label);
	//when label is not in the hashmap
	if(item == lookUpTable.end()){
		Node *newNode = new Node(currentId, 0, 0, 0, label);
		// Insert new node to map
		uniqueTable.insert({currentId, newNode});
		lookUpTable.insert({label,currentId});
		return currentId++;
	}else{
		return item->second;
	}
}
/*! uniqueTableSize
    Returns the number of the nodes currently exist in the unique table
*/
std::size_t Manager::uniqueTableSize(void)
{
    return uniqueTable.size();
}
//! topVar
    /*!
        Returns the ID of top variable of the BDD node f
    */
BDD_ID Manager::topVar(const BDD_ID f)
{
    return uniqueTable[f]->topVar;
}
