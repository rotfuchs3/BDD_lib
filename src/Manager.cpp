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
ClassProject::Manager::Manager()
{
  uniqueTable 	= uniqueTable_t();
#ifdef redundantCheck
  lookUpTable 	= lookUpTable_t();
#endif
  computeTable 	= computeTable_t();

  // Insert 0 and configure
  createVar("0");
  uniqueTable[0]->highId = 0;

  // Insert 1 and configure
  createVar("1");
  uniqueTable[1]->lowId = 1;
}
//! True
/*!
    Returns the ID of the node representing True
*/
const BDD_ID &ClassProject::Manager::True(void)
{
  return trueId;
}
//! False
/*!
    Returns the ID of the node representing False
*/
const BDD_ID &ClassProject::Manager::False(void)
{
  return falseId;
}
//! isConstant
/*!
    Returns true if x is a leaf node
*/
bool ClassProject::Manager::isConstant(const BDD_ID x)
{
  if (x == trueId || x == falseId)
  {
    return true;
  }
  return false;
}
//! isVariable
/*!
    Returns true if x is a variable
*/
bool ClassProject::Manager::isVariable(const BDD_ID x)
{
#ifdef inputErrorCheck
  // check if BDD_ID exists and if it's not 0 or 1
  if (x<0 || x > currentId  || isConstant(x))
  {
   return false;
  }
#endif

    // check highId == 1 and  check lowId == 0
    if ((uniqueTable[x]->highId == True()) && (uniqueTable[x]->lowId == False()))
    {
      return true;
    }

}
/*! createVar
    Creates a new variable for the BDD
*/
BDD_ID ClassProject::Manager::createVar(const std::string &label)
{
#ifdef redundantCheck
    lookUpTable_t::const_iterator item = lookUpTable.find(label);
  //when label is not in the hashmap
  if (item == lookUpTable.end())
  {
#endif
    return insertInUniquetable(True(), False(), currentId, label);
#ifdef redundantCheck
  }
  else
  {
    return item->second;
  }
#endif
}
/*! uniqueTableSize
    Returns the number of the nodes currently exist in the unique table
*/
std::size_t ClassProject::Manager::uniqueTableSize(void)
{
  return uniqueTable.size()-2;
}
//! topVar
/*!
   Returns the ID of top variable of the BDD node f
*/
BDD_ID ClassProject::Manager::topVar(const BDD_ID f)
{
  return uniqueTable[f]->topVar;
}
//! getTopVarName
/*!
  Returns the label of top variable of the BDD node f
*/
std::string ClassProject::Manager::getTopVarName(const BDD_ID f)
{
  if (!isValidID(f))
  {
    return "";
  }
  return uniqueTable[topVar(f)]->label;
}
//! ite
/*!
 Implements the if-then-else algorithm.
 Returns the new node that represents the ITE.
*/
BDD_ID ClassProject::Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
{
#ifdef inputErrorCheck
    if (!isValidID(i, t, e))
    {
        return MANAGER_FAIL;
    }
#endif
    //terminal cases
    if (t == e || i == 1)
    {
        return t;
    }
    if (i == 0)
    {
        return e;
    }
    BDD_ID alreadyExist = searchForNode(t, e, i);
    if (alreadyExist != MANAGER_FAIL)
        return alreadyExist;

    //termine highest topvariable depending on the variable order
    BDD_ID topVarI = MANAGER_FAIL, topVarT = MANAGER_FAIL, topVarE = MANAGER_FAIL;
    //Constant have topVar 1/0 but this is no variable
    //so it should not be considered by the highest topVariable depending on the variable order
    //missing this leads to
    if (!isConstant(i))
        topVarI = uniqueTable[i]->topVar;
    if (!isConstant(t))
        topVarT = uniqueTable[t]->topVar;
    if (!isConstant(e))
        topVarE = uniqueTable[e]->topVar;

    //minimal variable are the highest in the variable order
    BDD_ID min = currentId;
    if (topVarI != MANAGER_FAIL)
    {
        min = topVarI;
    }
    if (topVarT != MANAGER_FAIL && topVarT < min)
    {
        min = topVarT;
    }
    if (topVarE != MANAGER_FAIL && topVarE < min)
    {
        min = topVarE;
    }
    //new nodes?
    BDD_ID rHigh = ite(coFactorTrue(i, min), coFactorTrue(t, min), coFactorTrue(e, min));
    BDD_ID rLow = ite(coFactorFalse(i, min), coFactorFalse(t, min), coFactorFalse(e, min));

    //a new internal node isn't needed
    if (rHigh == rLow)
    {
        ITE_ID key(i, t, e);
        computeTable.insert({key,rHigh});
        return rHigh;
    }
    //check if the to return coFactor would be a constant
    if (rHigh == 1 && rLow == 0)
    {
        ITE_ID key(i, t, e);
        computeTable.insert({key,min});
        return min;
    }
    else
    {
        // check if already an internal node (rHigh,rLow,min) is in the UniqueTable
        BDD_ID alreadyExist = searchForNode(rHigh, rLow, min);
        if (alreadyExist != MANAGER_FAIL){
            ITE_ID key(i, t, e);
            computeTable.insert({key,alreadyExist});
            return alreadyExist;
        }
    }
    BDD_ID newNode = insertInUniquetable(rHigh, rLow, min, "");
    return newNode;
}

//! coFactorTrue
/*!
 Returns the positive cofactor of the function defined by f with respect to function x set to true.
*/
BDD_ID ClassProject::Manager::coFactorTrue(const BDD_ID f, BDD_ID x)
{

#ifdef inputErrorCheck
  if (!isValidID(f, x))
  {
    return MANAGER_FAIL;
  }
#endif
  if (!isVariable(x))
  {
    x = topVar(x);
  }

  if (isConstant(f))
  {
    return f;
  }

  Node *nodeF = uniqueTable[f];
  BDD_ID topVarF = nodeF->topVar;

  //if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
  //therefor x hasn't a effect.
  if (topVarF > x)
  {
    return nodeF->myId;
  }
  if (topVarF == x)
  {
    return nodeF->highId;
  }
  else
  { //topVarF<x so x is more down in the BDD

    BDD_ID coFacHigh = coFactorTrue(nodeF->highId, x);
    BDD_ID coFacLow = coFactorTrue(nodeF->lowId, x);

    //same child so the topVar topVarF can be skipped
    if (coFacHigh == coFacLow)
    {
      return coFacHigh;
    }
    else
    {
      //check if the to return coFactor would be a constant
      if (coFacHigh == 1 && coFacLow == 0)
      {
        return topVarF;
      }
      else
      {
        BDD_ID alreadyExist = searchForNode(coFacHigh, coFacLow, topVarF);
        if (alreadyExist != MANAGER_FAIL)
          return alreadyExist;
        //when no node fond
        std::string newLabel = "coFactorTrue(" + nodeF->label + "," + uniqueTable[f]->label + ")";
        return insertInUniquetable(coFacHigh, coFacLow, topVarF, newLabel);
      }
    }
  }
}

//! coFactorTrue
/*!
 Returns the positive cofactor of the function defined by f.
*/
BDD_ID ClassProject::Manager::coFactorTrue(const BDD_ID f)
{
#ifdef inputErrorCheck
  if (!isValidID(f))
  {
    return MANAGER_FAIL;
  }
#endif
  return uniqueTable[f]->highId;
}

//! coFactorFalse
/*!
  Returns the negativ cofactor of the function defined by f with respect to function x set to false.
*/
BDD_ID ClassProject::Manager::coFactorFalse(const BDD_ID f, BDD_ID x)
{
#ifdef inputErrorCheck
  if (!isValidID(f, x))
  {
    return MANAGER_FAIL;
  }
#endif
  if (!isVariable(x))
  {
    x = topVar(x);
  }

  if (isConstant(f))
  {
    return f;
  }

  Node *nodeF = uniqueTable[f];
  BDD_ID topVarF = nodeF->topVar;

  //if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
  //therefor x hasn't a effect.
  if (topVarF > x)
  {
    return nodeF->myId;
  }
  if (topVarF == x)
  {
    return nodeF->lowId;
  }
  else
  { //topVarF<x so x is more down in the BDD
    BDD_ID coFacHigh = coFactorFalse(nodeF->highId, x);
    BDD_ID coFacLow = coFactorFalse(nodeF->lowId, x);

    //same child so the topVar topVarF can be skipped
    if (coFacHigh == coFacLow)
    {
      return coFacHigh;
    }
    else
    {
      //check if the to return coFactor would be a constant
      if (coFacHigh == 1 && coFacLow == 0)
      {
        return topVarF;
      }
      else
      {
        BDD_ID alreadyExist = searchForNode(coFacHigh, coFacLow, topVarF);
        if (alreadyExist != MANAGER_FAIL)
          return alreadyExist;
        //when no node found
        std::string newLabel = "coFactorFalse(" + nodeF->label + "," + uniqueTable[x]->label + ")";
        return insertInUniquetable(coFacHigh, coFacLow, topVarF, newLabel);
      }
    }
  }
}
//! coFactorFalse
/*!
  Returns the negativ cofactor of the function defined by f.
*/
BDD_ID ClassProject::Manager::coFactorFalse(const BDD_ID f)
{
#ifdef inputErrorCheck
    if (!isValidID(f))
  {
    return MANAGER_FAIL;
  }
#endif
  return uniqueTable[f]->lowId;
}

//! coFactorFalse
/*!
  Returns the set of BDD nodes which are reachable
  from the BDD node root(including itself).
*/
void ClassProject::Manager::findNodes(BDD_ID root, std::set<BDD_ID> &nodes_of_root)
{
#ifdef inputErrorCheck
  if (!isValidID(root))
  {
    return ;
  }
#endif
  nodes_of_root.insert(root);
  if (isConstant(root))
  {
    return;
  }
  else
  {
    Node *rootNode = uniqueTable[root];
    BDD_ID high_of_root = rootNode->highId;
    BDD_ID low_of_root = rootNode->lowId;
    findNodes(high_of_root, nodes_of_root);
    findNodes(low_of_root, nodes_of_root);
  }
}

//! findVars
/*!
  Returns the set of variables which are either top variable of the BDD node root
   or the reachable nodes from root.
*/
void ClassProject::Manager::findVars(BDD_ID root, std::set<BDD_ID> &vars_of_root)
{
#ifdef inputErrorCheck
  if (!isValidID(root))
  {
    return ;
  }
#endif
  if (isConstant(root))
  {
    vars_of_root.insert(root);
    return;
  }
  else
  {
    Node *rootNode = uniqueTable[root];
    vars_of_root.insert(rootNode->topVar);
    BDD_ID high_of_root = rootNode->highId;
    BDD_ID low_of_root = rootNode->lowId;
    findVars(high_of_root, vars_of_root);
    findVars(low_of_root, vars_of_root);
  }
}

//! neg
/*!
  Returns BDD_ID of the negation of A if needed creats this node
*/
BDD_ID ClassProject::Manager::neg(const BDD_ID a)
{
#ifdef inputErrorCheck
  if (!isValidID(a))
  {
    return MANAGER_FAIL;
  }
#endif
  return ite(a, 0, 1);
}

//! and
/*!
  Returns BDD_ID of the Conjunktion of A and B. if needed creates this node
*/
BDD_ID ClassProject::Manager::and2(const BDD_ID a, const BDD_ID b)
{
#ifdef inputErrorCheck
  if (!isValidID(a, b))
  {
    return MANAGER_FAIL;
  }
#endif
  return ite(a, b, 0);
}

//! and
/*!
  Returns BDD_ID of the NAND with A and B. if needed creates this node
*/
BDD_ID ClassProject::Manager::nand2(const BDD_ID a, const BDD_ID b)
{
#ifdef inputErrorCheck
  if (!isValidID(a, b))
  {
    return MANAGER_FAIL;
  }
#endif
  return neg(and2(a, b));
}

//! or2
/*!
  Returns BDD_ID of the disjunction of A and B. if needed creates this node
*/
BDD_ID ClassProject::Manager::or2(const BDD_ID a, const BDD_ID b)
{
#ifdef inputErrorCheck
  if (!isValidID(a, b))
  {
    return MANAGER_FAIL;
  }
#endif
  return ite(a, 1, b);
}

//! xor2
/*!
  Returns BDD_ID of the exclusivOR of A and B. if needed creates this node
*/
BDD_ID ClassProject::Manager::xor2(const BDD_ID a, const BDD_ID b)
{
#ifdef inputErrorCheck
  if (!isValidID(a, b))
  {
    return MANAGER_FAIL;
  }
#endif
  return ite(a, neg(b), b);
}

//! nor2
/*!
  Returns BDD_ID of the negativ disjunction of A and B. if needed creates this node
*/
BDD_ID ClassProject::Manager::nor2(const BDD_ID a, const BDD_ID b)
{
#ifdef inputErrorCheck
  if (!isValidID(a, b))
  {
    return MANAGER_FAIL;
  }
#endif
  return neg(or2(a, b));
}
    /**
    * printTables
    *
    * @brief Prints all tables
    */
    void Manager::printTables(void)
    {
        // Unique table
        std::cout << " --- UNIQUE TABLE --- " << std::endl;
        for(auto it = uniqueTable.begin(); it != uniqueTable.end(); ++it)
        {
            std::cout << *(*it) << std::endl;
        }  
        // Compute table
        std::cout << " --- COMPUTE TABLE --- " << std::endl;
        for(auto it = computeTable.begin(); it != computeTable.end(); ++it)
        {
            std::cout << "BDD_ID[" << it->second << "]: " << it->first << std::endl;
        }
        std::cout << " --- LOOKUP TABLE --- " << std::endl;
        for(auto it = lookUpTable.begin(); it != lookUpTable.end(); ++it)
        {
            std::cout << "Label[" << it->first << "], BDD_ID[" << it->second << "]" << std::endl;
        }
    }
  //---------------------------------------------------------------------------------------------------------
  //private Methodes
  bool ClassProject::Manager::isValidID(BDD_ID i, BDD_ID t, BDD_ID e)
  {
    //check for corrupted input
    if (i >= currentId || i < 0)
    {
      return false;
    }
    if (t >= currentId || t < 0)
    {
      return false;
    }
    if (e >= currentId || e < 0)
    {
      return false;
    }
    return true;
  }

  BDD_ID ClassProject::Manager::insertInUniquetable(BDD_ID highID, BDD_ID lowID, BDD_ID topVar, std::string label)
  {
    Node *newNode = new Node(currentId, highID, lowID, topVar, label);
    // Insert new node to map
    uniqueTable.push_back( newNode);

#ifdef redundantCheck
    lookUpTable.insert({label, currentId});
#endif
    //std::string key = std::to_string(topVar)+"," + std::to_string(highID)+"," + std::to_string(lowID);
    //computeTable.insert({std::tuple<int,int,int>(topVar,highID,lowID), currentId});
    ITE_ID key(topVar, highID, lowID);
    computeTable.insert({key, currentId});

    return currentId++;
  }

  BDD_ID ClassProject::Manager::searchForNode(const BDD_ID _highId, const BDD_ID _lowId, const BDD_ID _topVar)
  {
    // define new key for compute table
    //std::tuple<int,int,int> key = std::tuple<int,int,int>(_topVar,_highId,_lowId);
    ITE_ID key(_topVar, _highId, _lowId);
    computeTable_t::const_iterator found = computeTable.find(key);
    if(found != computeTable.end())
    {
       return found->second;
    }
    return MANAGER_FAIL;
/*
    for (auto it = uniqueTable.begin(); it != uniqueTable.end() && it->first < currentId; ++it)
    {
      Node *iterateNode = it->second;
      BDD_ID topVar = iterateNode->topVar;
      BDD_ID highId = iterateNode->highId;
      BDD_ID lowId = iterateNode->lowId;
      //std::cout<<"Node: (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
      // there is a node already in the unique table
      if (topVar == _topVar && highId == _highId && lowId == _lowId)
      {
        return it->first;
      }
    }
    return MANAGER_FAIL;
*/
  }
