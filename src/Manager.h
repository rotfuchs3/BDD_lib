/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.h
 * \author vdscp_8
*/
#ifndef __MANAGER_H__
#define __MANAGER_H__
/// Base class
#include "ManagerInterface.h"
#include <iostream>
/// Container
#include <unordered_map>
#include <iterator>
//!  Manager class
/*!
  This class is derived from MamagerInterface, it implements all virtual functions and is the main class used by application.
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
    BDD_ID ite(const BDD_ID i,const BDD_ID t, const BDD_ID e);
    //! coFactorTrue
    /*!
     Returns the positive cofactor of the function defined by f with respect to function x set to true.
    */
    BDD_ID coFactorTrue(const BDD_ID f,BDD_ID x);
    //! coFactorTrue
    /*!
     Returns the positive cofactor of the function defined by f.
    */
    BDD_ID coFactorTrue(const BDD_ID f);
    //! coFactorFalse
    /*!
      Returns the negativ cofactor of the function defined by f with respect to function x set to false.
    */
    BDD_ID coFactorFalse(const BDD_ID f,BDD_ID x);
    //! coFactorFalse
    /*!
      Returns the negativ cofactor of the function defined by f.
    */
    BDD_ID coFactorFalse(const BDD_ID f) ;

    //! coFactorFalse
	/*!
	  Returns the set of BDD nodes which are reachable
	  from the BDD node root(including itself).
	*/
	void findNodes(BDD_ID root,std::set<BDD_ID> &nodes_of_root);

private:
    /// Terminal true BDD_ID
    const BDD_ID trueId  = 1;
    /// Terminal false BDD_ID
    const BDD_ID falseId = 0;
    /// Keep track of current BDD_ID, start off at 2 since IDs 0 and 1 are for true and false
    BDD_ID currentId = 0;
    /// uniqueTable, hashmap for performance
    std::unordered_map<BDD_ID, Node*> uniqueTable;
    std::unordered_map<std::string, BDD_ID> lookUpTable;
	
    bool isValidID(BDD_ID arg1,BDD_ID arg2=0, BDD_ID arg3=0);
    BDD_ID insertInUniquetable(BDD_ID highID,BDD_ID lowID,BDD_ID topVar,std::string label);

  struct Key{
	  BDD_ID id;
  };

  struct KeyHash{
  	std::size_t operator()(const BDD_ID& k) const {
  		return k%31;
  	}
  };

  	struct KeyEqual{
  		bool operator()(const BDD_ID& first, const BDD_ID& second)	const{
  			return first==second;
  		}
  	};
};
#endif /* __MANAGER_H__ */
