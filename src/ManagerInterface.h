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
#include <set>

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
    BDD_ID highId;
    //! falseId
    /*!
        ID for the node for '0' evaluation, left node
    */
    BDD_ID lowId;
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
    Node(BDD_ID myId, BDD_ID highId, BDD_ID lowId, BDD_ID topVar, std::string label)
            : myId(myId), highId(highId), lowId(lowId), topVar(topVar), label(label) 
    {}
    //! Destructor
    /*!
        The destructor
    */
    ~Node()
    {}
};
//!  ManagerInterface class
/*!
  This class defines the methods to be used by the Manager and that interface directly with the nodes.
*/
class ManagerInterface {
public:
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
    //! createVar
    /*! 
        Creates a new variable for the BDD
    */
    virtual BDD_ID createVar(const std::string &label) = 0;
    //! uniqueTableSize
    /*!
        Returns the number of the nodes currently exist in the unique table
    */
    virtual std::size_t uniqueTableSize(void) = 0;
    //! topVar
    /*!
        Returns the ID of top variable of the BDD node f
    */
    virtual BDD_ID topVar(const BDD_ID f) = 0;
    //! ite
    /*!
      Implements the if-then-else algorithm.
	  Returns the new node that represents the ITE.
    */
    virtual BDD_ID ite(const BDD_ID i,const BDD_ID t, const BDD_ID e) = 0;
    //! coFactorTrue
    /*!
      Returns the positive cofactor of the function defined by f with respect to function x set to true.
    */
    virtual BDD_ID coFactorTrue(const BDD_ID f,BDD_ID x) = 0;
    //! coFactorTrue
    /*!
      Returns the positive cofactor of the function defined by f.
    */
    virtual BDD_ID coFactorTrue(const BDD_ID f) = 0;
    //! coFactorFalse
    /*!
      Returns the negativ cofactor of the function defined by f with respect to function x set to false.
    */
    virtual BDD_ID coFactorFalse(const BDD_ID f,BDD_ID x) = 0;
    //! coFactorFalse
    /*!
      Returns the negativ cofactor of the function defined by f.
    */
    virtual BDD_ID coFactorFalse(const BDD_ID f) = 0;
    //! findNodes
	/*!
	  Returns the set of BDD nodes which are reachable
	  from the BDD node root(including itself).
	*/
	virtual void findNodes(BDD_ID root,std::set<BDD_ID> &nodes_of_root) = 0;
    //! findVars
	/*!
	  Returns the set of variables which are either top variable of the BDD node root
	   or the reachable nodes from root.
	*/
	virtual void findVars(BDD_ID root,std::set<BDD_ID> &vars_of_root) = 0;
	//! neg
	/*!
	  Returns BDD_ID of the negation of A if needed creats this node
	*/
	virtual BDD_ID neg(const BDD_ID a) = 0;
	//! and
	/*!
	  Returns BDD_ID of the conjunction of A and B. if needed creates this node
	*/
	virtual BDD_ID and2(const BDD_ID a,const BDD_ID b) = 0;
	//! and
	/*!
	  Returns BDD_ID of the NAND with A and B. if needed creates this node
	*/
	virtual BDD_ID nand2(const BDD_ID a,const BDD_ID b) = 0;
	//! getTopVarName
	/*!
	  Returns the label of top variable of the BDD node f
	*/
	virtual std::string getTopVarName(const BDD_ID f) = 0;
	//! or2
	/*!
	  Returns BDD_ID of the disjunction of A and B. if needed creates this node
	*/
	virtual BDD_ID or2(const BDD_ID a,const BDD_ID b) = 0;
};
#endif /* __MANAGER_INTERFACE_H__ */
