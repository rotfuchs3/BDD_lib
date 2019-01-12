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

/// Add specified namespace
namespace ClassProject {

/// Apparantley BDD_ID needs to be defined inside AND outside namespace!
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
        \return the ID of the node representing True
    */
    virtual const BDD_ID &True() = 0;
    //! False
    /*!
        \return the ID of the node representing False
    */
    virtual const BDD_ID &False() = 0;
    //! isConstant
    /*!
     * 	\param x is the BDD_ID that should be checked
        \return true if x is a leaf node
    */
    virtual bool isConstant(const BDD_ID x) = 0;
    //! isVariable
    /*!
     * 	\param x is the BDD_ID that should be checked
        \return true if x is a variable
    */
    virtual bool isVariable(const BDD_ID x) = 0;
    //! createVar
    /*! 
        \brief Creates a new variable for the BDD with string &label in the unique table
        \param &label the label that got created and inserted in the unique table
        \return the BDD_ID if it is a new label. When it is inserted before it returns the ID of the first Variable with label &label
    */
    virtual BDD_ID createVar(const std::string &label) = 0;
    //! uniqueTableSize
    /*!
        \return the number of the nodes + variables currently exist in the unique table
    */
    virtual std::size_t uniqueTableSize(void) = 0;
    //! topVar
    /*!
     * 	\param f the ID of the node which topVariable is requested
        \return the ID of top variable of the BDD node f
    */
    virtual BDD_ID topVar(const BDD_ID f) = 0;
    //! getTopVarName
    /*!
     *  \param f the ID of the node which topVariable-label is requested
    	\return the label of top variable of the BDD node f
    */
    virtual std::string getTopVarName(const BDD_ID f) = 0;
    //! ite
    /*!
     *\param i is the ID for the If node of the If-then-else expression
      \param t is the ID for the Then node of the If-then-else expression
      \param e is the ID for the Else node of the If-then-else expression
      \return the new node ID that represents the ITE. If it didn't exist it will be inserted to the uniqueTable
    */
    virtual BDD_ID ite(const BDD_ID i,const BDD_ID t, const BDD_ID e) = 0;
    //! coFactorTrue
    /*!
     *\param f is the ID which the positive coFactor of variable x is requested
     *\param x is the ID which f should be the CoFactor in the positiv way
      \return the positive cofactor of the function defined by f with respect to function x set to true.
    */
    virtual BDD_ID coFactorTrue(const BDD_ID f,BDD_ID x) = 0;
    //! coFactorTrue
    /*!
     *\param f is the ID which the positive coFactor the topVar of f is requested
      \return the positive cofactor of the function defined by f.
    */
    virtual BDD_ID coFactorTrue(const BDD_ID f) = 0;
    //! coFactorFalse
    /*!
	 *\param f is the ID which the negativ coFactor of variable x is requested
	 *\param x is the ID which f should be the CoFactor in the negativ way
	  \return the negativ cofactor of the function defined by f with respect to function x set to true.
	*/
    virtual BDD_ID coFactorFalse(const BDD_ID f,BDD_ID x) = 0;
    //! coFactorFalse
    /*!
	*\param f is the ID which the negativ coFactor the topVar of f is requested
	 \return the negativ cofactor of the function defined by f.
	*/
    virtual BDD_ID coFactorFalse(const BDD_ID f) = 0;
    //! findNodes
	/*!
	 * \param root is the ID which is the root where all reachable nodes are requested.
	 * \param the set where all different node IDs should be saved in
	   \return the set of nodes which are either the root itself or the reachable nodes from root.
	*/
	virtual void findNodes(BDD_ID root,std::set<BDD_ID> &nodes_of_root) = 0;
    //! findVars
	/*!
	 * \param root is the ID which is the root where all reachable nodes top variable are requested.
	 * \param the set where all different variable IDs should be saved in
	   \return the set of variables which are either top variable of the BDD node root or the reachable nodes from root.
	*/
	virtual void findVars(BDD_ID root,std::set<BDD_ID> &vars_of_root) = 0;
	//! neg
	/*!
	  \param the BDD_ID (root of this node) which should be inverted 
	  \return the BDD_ID of the negation of A if needed creates this node
	*/
	virtual BDD_ID neg(const BDD_ID a) = 0;
	//! and
	/*!
	  \param the BDD_ID of A
	  \param the BDD_ID of B
	  \return BDD_ID of the conjunction of A and B. if needed creates this node
	*/
	virtual BDD_ID and2(const BDD_ID a,const BDD_ID b) = 0;
	//! nand
	/*!
	  \param the BDD_ID of A
	  \param the BDD_ID of B
	  \return the BDD_ID of the NAND with A and B. if needed creates this node
	*/
	virtual BDD_ID nand2(const BDD_ID a,const BDD_ID b) = 0;
	//! or2
	/*!
	  \param the BDD_ID of A
	  \param the BDD_ID of B
	  \return the BDD_ID of the disjunction of A and B. if needed creates this node
	*/
	virtual BDD_ID or2(const BDD_ID a,const BDD_ID b) = 0;
	//! xor2
	/*!
	  \param the BDD_ID of A
	  \param the BDD_ID of B
	  \return the BDD_ID of the exclusivOR of A and B. if needed creates this node
	*/
	virtual BDD_ID xor2(const BDD_ID a,const BDD_ID b) = 0;
	//! nor2
	/*!
	  \param the BDD_ID of A
	  \param the BDD_ID of B
	  \return BDD_ID of the negativ disjunction of A and B. if needed creates this node
	*/
	virtual BDD_ID nor2(const BDD_ID a,const BDD_ID b) = 0;
};
}
#endif /* __MANAGER_INTERFACE_H__ */
