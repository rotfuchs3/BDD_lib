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
//!  Manager class
/*!
  This class is derived from MamagerInterface, it implements all virtual functions and is the main class used by application.
*/
class Manager : ManagerInterface {
public:
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

private:
  /// Terminal true BDD_ID
  const BDD_ID trueId  = 1;
  /// Terminal false BDD_ID
  const BDD_ID falseId = 0;
};
#endif /* __MANAGER_H__ */
