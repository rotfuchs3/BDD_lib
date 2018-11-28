/** 
 * \defgroup bddPkg
 * \brief ManagerInterface is an abstract base class that supplies function decleratuons to the Manager derived class.
 * \file ManagerInterface.h
 * \author vdscp_8
*/
#ifndef __MANAGER_INTERFACE_H__
#define __MANAGER_INTERFACE_H__
/// Standard Lib
#include <string>

/// Typedef Integer representing of the ID for BDD Node
typedef int BDD_ID;

//!  Node class
/*!
  This class describes the nodes, non-terminal and terminal nodes for the ROBDD.
*/
class Node {
public:
  //! myId
    /*!
      ID that identifies this node
    */
    BDD_ID myId;
  //! trueId
    /*!
      ID for the node for '1' evaluation, right node
    */
    BDD_ID trueId;
  //! falseId
    /*!
      ID for the node for '0' evaluation, left node
    */
    BDD_ID falseId;
  //! topVar
    /*!
      Current nodes top variable
    */
    BDD_ID topVar;
  //! label
  
    /*!
      The node's string label
    */
  std::string label;
  
  //! Constructor
    /*!
      Sets all values to default
    */
  Node(BDD_ID myId, BDD_ID trueId, BDD_ID falseId, BDD_ID topVar, std::string label)
  : myId(myId), trueId(trueId), falseId(falseId), topVar(topVar), label(label) {}
  //! Destructor
    /*!
      The destructor
    */
   ~Node(){}
};
//!  ManagerInterface class
/*!
  This class defines the methods to be used by the Manager and that interface directly with the nodes.
*/
class ManagerInterface {
public:
  //! Constructor
    /*!
      Abstract class, not much to do here yet, maybe there will be private vars
    */
  //ManagerInterface(void);
  //! True
  /*!
    Returns the ID of the node representing True
  */
  virtual const BDD_ID &True() = 0;
  //! False
  /*!
    Returns the ID of the node representing False
  */
  virtual const BDD_ID &False() = 0;
  //! isConstant
  /*!
    Returns true if x is a leaf node
  */
  virtual bool isConstant(const BDD_ID x) = 0;
  //! isVariable
  /*!
    Returns true if x is a variable
  */
  virtual bool isVariable(const BDD_ID x) = 0;	  
};
#endif /* __MANAGER_INTERFACE_H__ */
