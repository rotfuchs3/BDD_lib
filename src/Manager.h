/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's
 * methods. \file Manager.h \author vdscp_8
 */
#ifndef __MANAGER_H__
#define __MANAGER_H__
/// Base class
#include "ManagerInterface.h"
#include <iostream>
/// Container
#include <iterator>
#include <unordered_map>
/// Manager typedef
typedef std::unordered_map<BDD_ID, Node *> uniqueTable_t;
typedef std::unordered_map<std::string, BDD_ID> lookUpTable_t;
typedef std::unordered_map<std::string, BDD_ID> computedTable_t;
#define MANAGER_FAIL -1
//!  Manager class
/*!
  This class is derived from MamagerInterface, it implements all virtual
  functions and is the main class used by application.
*/
class Manager : ManagerInterface {
public:
  //! Constructor
  /*!
        Initializes uniqueTable
    */
  Manager(void);
  //! True
  /*!
        Returns the ID of the node representing True
    */
  const BDD_ID &True(void);
  //! False
  /*!
        Returns the ID of the node representing False
    */
  const BDD_ID &False(void);
  //! isConstant
  /*!
        Returns true if x is a leaf node
    */
  bool isConstant(const BDD_ID x);
  //! isVariable
  /*!
        Returns true if x is a variable
    */
  bool isVariable(const BDD_ID x);
  /*! createVar
        Creates a new variable for the BDD
    */
  BDD_ID createVar(const std::string &label);
  //! uniqueTableSize
  /*!
        Returns the number of the nodes currently exist in the unique table
    */
  std::size_t uniqueTableSize(void);
  //! topVar
  /*!
        Returns the ID of top variable of the BDD node f
    */
  BDD_ID topVar(const BDD_ID f);
  //! ite
  /*!
     Implements the if-then-else algorithm.
     Returns the new node that represents the ITE.
    */
  BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);
  //! coFactorTrue
  /*!
     Returns the positive cofactor of the function defined by f with respect to
     function x set to true.
    */
  BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x);
  //! coFactorTrue
  /*!
     Returns the positive cofactor of the function defined by f.
    */
  BDD_ID coFactorTrue(const BDD_ID f);
  //! coFactorFalse
  /*!
      Returns the negativ cofactor of the function defined by f with respect to
     function x set to false.
    */
  BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x);
  //! coFactorFalse
  /*!
      Returns the negativ cofactor of the function defined by f.
    */
  BDD_ID coFactorFalse(const BDD_ID f);
  //! coFactorFalse
  /*!
      Returns the set of BDD nodes which are reachable from the BDD node root(including itself).
    */
  void findNodes(BDD_ID root, std::set<BDD_ID> &nodes_of_root);
  //! findVars
  /*!
      Returns the set of variables which are either top variable of the BDD node root or the reachable nodes from root.
    */
  void findVars(BDD_ID root, std::set<BDD_ID> &vars_of_root);
  //! neg
  /*!
      Returns BDD_ID of the negation of A if needed creats this node
    */
  BDD_ID neg(const BDD_ID a);
  //! and
  /*!
      Returns BDD_ID of the Conjunktion of A and B. if needed creates thisnode
    */
  BDD_ID and2(const BDD_ID a, const BDD_ID b);
  //! and
  /*!
      Returns BDD_ID of the NAND with A and B. if needed creates this node
    */
  BDD_ID nand2(const BDD_ID a, const BDD_ID b);
  //! or2
  /*!
      Returns BDD_ID of the OR for A and B. if needed creates this node
    */
  BDD_ID or2(const BDD_ID a, const BDD_ID b);
  //! xor2
  /*!
      Returns BDD_ID of the XOR for A and B. if needed creates this node
    */
  BDD_ID xor2(const BDD_ID a, const BDD_ID b);
  //! nor2
  /*!
      Returns BDD_ID of the NOR for A and B. if needed creates this node
    */
  BDD_ID nor2(const BDD_ID a, const BDD_ID b);
  //! printUniqueTable
  /*!
      Prints table
    */
  void printUniqueTable(void);
private:
  /// Terminal true BDD_ID
  const BDD_ID trueId = 1;
  /// Terminal false BDD_ID
  const BDD_ID falseId = 0;
  /// Keep track of current BDD_ID, start off at 2 since IDs 0 and 1 are for
  /// true and false
  BDD_ID currentId = 0;
  /// unique table
  uniqueTable_t uniqueTable;
  /// lookup table
  lookUpTable_t lookUpTable;
  /// maps i, t, e with computed ite(i, t, e)
  computedTable_t computedTable;
  //! isValidID
  /*!
      Returns true/false if given arguments are valid BDD_IDs
    */
  bool isValidID(BDD_ID arg1, BDD_ID arg2 = 0, BDD_ID arg3 = 0);
  //! insertInUniquetable
  /*!
      Inserts to the unique table and returns BDD_ID
    */
  BDD_ID insertInUniquetable(BDD_ID highID, BDD_ID lowID, BDD_ID topVar,
                             std::string label);
  //! searchUniTable
  /*!
      Returns MANAGER_FAIL if id is not found in unique table
    */
  Node *searchUniTable(const BDD_ID id);
  //! searchLookUpTable
  /*!
      Returns MANAGER_FAIL if label is not found in lookup table, otherwise, the BDD_ID is returned
    */
  BDD_ID searchLookUpTable(const std::string label);
};
#endif /* __MANAGER_H__ */
