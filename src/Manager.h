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
/// Container
#include <unordered_map>
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

  void insert(BDD_ID key,Node value){
	  uniqueTable.insert({key,value});
  }

  int size(){
	  return uniqueTable.size();
  }

private:
  /// Terminal true BDD_ID
  const BDD_ID trueId  = 1;
  /// Terminal false BDD_ID
  const BDD_ID falseId = 0;
  /// uniqueTable, hashmap for performance
  std::unordered_map<BDD_ID, Node> uniqueTable;
  
  struct Key{
	  BDD_ID id;
  };

  struct KeyHash{
  	std::size_t operator()(const BDD_ID& k) const{
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
