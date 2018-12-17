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
/// Manager typedef
typedef std::unordered_map<BDD_ID, Node*>        uniqueTable_t;
typedef std::unordered_map<std::string, BDD_ID>  lookUpTable_t;
#define     MANAGER_FAIL    -1
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
     \return the ID of the node representing True
    */
    const BDD_ID &True(void);
    //! False
    /*!
        \return the ID of the node representing False
    */
    const BDD_ID &False(void);
    //! isConstant
    /*!
    * 	\param x is the BDD_ID that should be checked
        \return true if x is a leaf node
    */
    bool isConstant(const BDD_ID x);
    //! isVariable
    /*!
    * 	\param x is the BDD_ID that should be checked
        \return true if x is a leaf node
    */
    bool isVariable(const BDD_ID x);
    //! createVar
    /*!
        \brief Creates a new variable for the BDD with string &label in the unique table
        \param &label the label that got created and inserted in the unique table
        \return the BDD_ID if it is a new label. When it is inserted before it returns the ID of the first Variable with label &label
    */
    BDD_ID createVar(const std::string &label);
    //! uniqueTableSize
    /*!
     *
        \return the number of the nodes + variables currently exist in the unique table
    */
    std::size_t uniqueTableSize(void);
    //! topVar
    /*!
       \param f the ID of the node which topVariable is requested
       \return the ID of top variable of the BDD node f
    */
    BDD_ID topVar(const BDD_ID f);
    //! getTopVarName
    /*!
      \param f the ID of the node which topVariable-label is requested
      \return the label of top variable of the BDD node f
    */
    std::string getTopVarName(const BDD_ID f) ;
    //! ite
    /*!
     *\param i is the ID for the If node of the If-then-else expression
      \param t is the ID for the Then node of the If-then-else expression
      \param e is the ID for the Else node of the If-then-else expression
      \return the new node ID that represents the ITE. If it didn't exist it will be inserted to the uniqueTable
    */
    BDD_ID ite(const BDD_ID i,const BDD_ID t, const BDD_ID e);
    //! coFactorTrue
    /*!
     *\param f is the ID which the positive coFactor of variable x is requested
     *\param x is the ID which f should be the CoFactor in the positiv way
      \return the positive cofactor of the function defined by f with respect to function x set to true.
    */
    BDD_ID coFactorTrue(const BDD_ID f,BDD_ID x);
    //! coFactorTrue
    /*!
    *\param f is the ID which the positive coFactor the topVar of f is requested
     \return the positive cofactor of the function defined by f.
    */
    BDD_ID coFactorTrue(const BDD_ID f);
    //! coFactorFalse
	/*!
	 *\param f is the ID which the negativ coFactor of variable x is requested
	 *\param x is the ID which f should be the CoFactor in the negativ way
	  \return the negativ cofactor of the function defined by f with respect to function x set to true.
	*/
    BDD_ID coFactorFalse(const BDD_ID f,BDD_ID x);
    //! coFactorFalse
    /*!
	*\param f is the ID which the negativ coFactor the topVar of f is requested
	 \return the negativ cofactor of the function defined by f.
	*/
    BDD_ID coFactorFalse(const BDD_ID f) ;
    //! findNodes
    /*!
	 * \param root is the ID which is the root where all reachable nodes are requested.
	 * \param the set where all different node IDs should be saved in
	   \return the set of nodes which are either the root itself or the reachable nodes from root.
	*/
	void findNodes(BDD_ID root,std::set<BDD_ID> &nodes_of_root);
	//! findVars
	/*!
	 * \param root is the ID which is the root where all reachable nodes top variable are requested.
	 * \param the set where all different variable IDs should be saved in
	   \return the set of variables which are either top variable of the BDD node root or the reachable nodes from root.
	*/
	void findVars(BDD_ID root,std::set<BDD_ID> &vars_of_root);
	//! neg
	/*!
	  Returns BDD_ID of the negation of A if needed creats this node
	*/
	BDD_ID neg(const BDD_ID a);
	//! and
	/*!
	  Returns BDD_ID of the Conjunktion of A and B. if needed creates this node
	*/
	BDD_ID and2(const BDD_ID a,const BDD_ID b);
	//! and
	/*!
	  Returns BDD_ID of the NAND with A and B. if needed creates this node
	*/
	BDD_ID nand2(const BDD_ID a,const BDD_ID b);
	//! or2
	/*!
	  Returns BDD_ID of the disjunction of A and B. if needed creates this node
	*/
	BDD_ID or2(const BDD_ID a,const BDD_ID b);
	//! xor2
	/*!
	  Returns BDD_ID of the exclusivOR of A and B. if needed creates this node
	*/
	BDD_ID xor2(const BDD_ID a,const BDD_ID b);
	//! nor2
	/*!
	  Returns BDD_ID of the negativ disjunction of A and B. if needed creates this node
	*/
	BDD_ID nor2(const BDD_ID a,const BDD_ID b);
  //! printUniqueTable
  /*!
      Prints unique table
  */
void printUniqueTable(void);
private:
    /// Terminal true BDD_ID
    const BDD_ID trueId  = 1;
    /// Terminal false BDD_ID
    const BDD_ID falseId = 0;
    /// Keep track of current BDD_ID, start off at 2 since IDs 0 and 1 are for true and false
    BDD_ID currentId = 0;

    /// uniqueTable, hashmap for performance
    uniqueTable_t uniqueTable;
    lookUpTable_t lookUpTable;
	
    bool isValidID(BDD_ID arg1,BDD_ID arg2=0, BDD_ID arg3=0);
    BDD_ID insertInUniquetable(BDD_ID highID,BDD_ID lowID,BDD_ID topVar,std::string label);
    BDD_ID searchUniTable(const BDD_ID id);
    BDD_ID searchForNode(const BDD_ID _highId,const BDD_ID _lowId, const BDD_ID _topVar);

  struct Key{
	  BDD_ID id;
  };


};
#endif /* __MANAGER_H__ */
