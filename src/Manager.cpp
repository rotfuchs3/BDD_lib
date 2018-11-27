/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.cpp
 * \author vdscp_8
*/
/// Header
include "Manager.h"
//! True
/*!
  Returns the ID of the node representing True
*/
const BDD_ID Manager::&True(void)
{
  return trueId;
}
//! False
/*!
  Returns the ID of the node representing False
*/
const BDD_ID Manager::&False(void)
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
