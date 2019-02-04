/** 
 * \defgroup bddPkg
 * \brief ManagerInterface is an abstract base class that supplies function decleratuons to the Manager derived class.
 * \file ManagerInterface.h
 * \author vdscp_8
*/
#ifndef __MANAGER_INTERFACE_H__
#define __MANAGER_INTERFACE_H__

/// STL
#include <set>
#include <string>
#include <iostream>
/// Typedef Integer representing of the ID for BDD Node - MUST also be defined outside namespace due to benchmark given software
typedef int BDD_ID;
/// Add specified namespace
namespace ClassProject {
    /// Typedef Integer representing of the ID for BDD Node
    typedef int BDD_ID;
    /**
    * @class Node
    *
    * @brief Describes the nodes, non-terminal and terminal, for the OBDD.
    *
    */
    class Node {
    public:
        /// ID that identifies this node
        BDD_ID myId;
        /// ID for the node for '1' evaluation, right node
        BDD_ID highId;
        /// ID for the node for '0' evaluation, left node
        BDD_ID lowId;
        /// Node's top variable
        BDD_ID topVar;
        /// The node's string label
        std::string label;
        /**
        * Node constructor
        *
        * @param All private members get initialized
        *
        * @return Nothing
        */
        Node(   const BDD_ID &myId, const BDD_ID &highId, 
                const BDD_ID &lowId, const BDD_ID &topVar, const std::string &label)
                : myId(myId), highId(highId), lowId(lowId), topVar(topVar), label(label) 
        {}
        /**
        * Overload "<<" operator
        *
        * @param n Node type
        *
        * @return cout
        */
	    friend std::ostream & operator <<(std::ostream &out, const Node &n) {
	        out << "BDD_ID[" << n.myId << "]: ";
	        out << "label: " << n.label;
	        out << ", highId: " << n.highId;
	        out << ", lowId: " << n.lowId;
	        out << ", topVar: " << n.topVar;
	        return out;
	    }
    }; // struct Node
/**
    * @class ManagerInterface
    *
    * @brief Defines the methods to be used by the Manager and that interface directly with the nodes.
    *        Purely virtual class given by the assignment.
    *
    */
    class ManagerInterface {
    public:
        /**
        * True
        *
        * @return ID for the node representing "true"
        */
        virtual const BDD_ID &True(void) = 0;
        /**
        * False
        *
        * @return ID for the node representing "false"
        */
        virtual const BDD_ID &False(void) = 0;
        /**
        * isConstant
        *
        * @param x BDD_ID that should be checked
        *
        * @return True if x is a leaf/terminal node
        */
        virtual bool isConstant(const BDD_ID x) = 0;
        /**
        * isVariable
        *
        * @param x BDD_ID that should be checked
        *
        * @return True if x is a variable/non-terminal node
        */
        virtual bool isVariable(const BDD_ID x) = 0;
        /**
        * createVar
        *
        * @param label The label that will be used to create the node and insert to the table
        *
        * @return BDD_ID if it is a new variable, otherewise, BDD_ID of existing node
        */
        virtual BDD_ID createVar(const std::string &label) = 0;
        /**
        * uniqueTableSize
        *
        * @return Number of the nodes currently in the unique table
        */
        virtual std::size_t uniqueTableSize(void) = 0;
        /**
        * topVar
        *
        * @param f ID of the node for top variable request
        *
        * @return BDD_ID of top variable of node f
        */
        virtual BDD_ID topVar(const BDD_ID f) = 0;
        /**
        * getTopVarName
        *
        * @param f ID of the node for label request of its top variable
        *
        * @return Label of top variable of node f
        */
        virtual std::string getTopVarName(const BDD_ID f) = 0;
        /**
        * ite
        *
        * @param i ID of the node for the "if" node for the if-then-else operation
        * @param t ID of the node for the "then" node for the if-then-else operation
        * @param e ID of the node for the "else" node for the if-then-else operation
        *
        * @return ID representation of the ITE operation
        */
        virtual BDD_ID ite(const BDD_ID i,const BDD_ID t, const BDD_ID e) = 0;
        /**
        * coFactorFalse
        *
        * @param f ID of the node for a function 'f'
        *
        * @return ID representation of the negative cofactor defined by the function 'f'
        */
        virtual BDD_ID coFactorFalse(const BDD_ID f) = 0;       
        /**
        * coFactorTrue
        *
        * @param f ID of the node for a function 'f'
        *
        * @return ID representation of the positive cofactor defined by the function 'f'
        */        
        virtual BDD_ID coFactorTrue(const BDD_ID f) = 0;
        /**
        * coFactorFalse
        *
        * @param f ID of the node for a function 'f'
        * @param x ID of the node for a variable in function 'f'
        *
        * @return ID representation of the negative cofactor for f(x), x = 0
        */
        virtual BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x) = 0;      
        /**
        * coFactorTrue
        *
        * @param f ID of the node for a function 'f'
        * @param x ID of the node for a variable in function 'f'
        *
        * @return ID representation of the positive cofactor for f(x), x = 1
        */
        virtual BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x) = 0;        
        /**
        * findNodes
        *
        * @param root ID for 'starting' point of sub-graph request
        * @param nodesOfRoot Set where all reachable nodes from root, including root, are saved
        *
        * @return ID None
        */
        virtual void findNodes(BDD_ID root, std::set<BDD_ID> &nodes_of_root) = 0;
        /**
        * findVars
        *
        * @param root ID for 'starting' point of top variable sub-graph request
        * @param varsOfRoot Set where all reachable nodes' top varaibles from root, including root, are saved
        *
        * @return ID None
        */
        virtual void findVars(BDD_ID root, std::set<BDD_ID> &vars_of_root) = 0;
        /**
        * neg
        *
        * @param a ID of node to be inverted
        *
        * @return ID of 'a' negation
        */
	    virtual BDD_ID neg(const BDD_ID a) = 0;
        /**
        * and2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' AND 'b' operation
        */
	    virtual BDD_ID and2(const BDD_ID a, const BDD_ID b) = 0;
        /**
        * nand2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' NAND 'b' operation
        */
        virtual BDD_ID nand2(const BDD_ID a, const BDD_ID b) = 0;
        /**
        * or2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' OR 'b' operation
        */
	    virtual BDD_ID or2(const BDD_ID a, const BDD_ID b) = 0;
        /**
        * xor2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' XOR 'b' operation
        */
	    virtual BDD_ID xor2(const BDD_ID a, const BDD_ID b) = 0;
        /**
        * nor2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' NOR 'b' operation
        */
	    virtual BDD_ID nor2(const BDD_ID a, const BDD_ID b) = 0;
    }; // class ManagerInterface
} // namespace ClassProject
#endif /* __MANAGER_INTERFACE_H__ */
