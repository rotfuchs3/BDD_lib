/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.h
 * \author vdscp_8
*/
#ifndef __MANAGER_H__
#define __MANAGER_H__

/// Code versions
#include "ManagerInterface.h"

/// STL
#include <unordered_map>
#include <vector>

/// Add specified namespace
namespace ClassProject {
    /// ITE_ID is used as key for the compute table hash
    struct ITE_ID {
        BDD_ID i;
        BDD_ID t;
        BDD_ID e;
        // Create ITE_ID
        ITE_ID(BDD_ID i,BDD_ID t,BDD_ID e) {
            this->i = i;
            this->t = t;
            this->e = e;
        }
        // Needed hash
        bool operator==(const ITE_ID &other) const {
            return (i == other.i && t == other.t && e == other.e);
        }
        /**
        * Overload "<<" operator
        *
        * @param t ITE_ID type
        *
        * @return cout
        */
	    friend std::ostream & operator <<(std::ostream &out, const ITE_ID &t) {
	        out << "if: " << t.i << ", then: " << t.t << ", else: " << t.e;
	        return out;
	    }
    }; // struct ITE_ID
    /// Compute table hashing function
    struct computeHash
    {
        std::size_t operator()(const ITE_ID &iteID) const
        {
            // Compute indiviual values
            std::size_t myhash = 15;
            myhash = myhash * 256 + std::hash<BDD_ID>()(iteID.i);
            myhash = myhash * 256 + std::hash<BDD_ID>()(iteID.t);
            myhash = myhash * 256 + std::hash<BDD_ID>()(iteID.e);
            return myhash;
        }
    }; // computeHash
    /// Manager typedef
    typedef std::vector<Node*>	                            uniqueTable_t;
    typedef std::unordered_map<ITE_ID, BDD_ID, computeHash> computeTable_t;
    typedef std::unordered_map<std::string, BDD_ID>	        lookUpTable_t;
    typedef lookUpTable_t::iterator                         lIter_t;
    typedef computeTable_t::iterator                        cIter_t;
   /**
    * @class Manager
    *
    * @brief This class is derived from Interface, it implements all functions and is the main class used by application.
    */
    class Manager : public ManagerInterface {
    private:
        // Make Reachable a friend
        friend class Reachable;
        /// Terminal true BDD_ID
        const static BDD_ID trueId          = 1;
        /// Terminal false BDD_ID
        const static BDD_ID falseId         = 0;
        /// Keep track of current BDD_ID, start off at 2 since IDs 0 and 1 are for true and false
        BDD_ID currentId                    = 0;
        
        /// uniqueTable, hashmap for performance
        uniqueTable_t   uniqueTable;
        computeTable_t  computeTable;
        lookUpTable_t   lookUpTable;

        /// Checks if given ID is valid
        const bool isValidID(BDD_ID arg1, BDD_ID arg2=0, BDD_ID arg3=0);
        /// Inserts into unique table
        BDD_ID insertInUniquetable(BDD_ID highID, BDD_ID lowID, BDD_ID topVar, std::string label);
        /// Search unique table for an ID and returns if it does (BDD_ID) or doesn't (MANAGER_FAIL) exists
        BDD_ID searchUniTable(const BDD_ID id);
        /// Searches compute table for a given ITE_ID and returns the BDD_ID with corresponding values
        BDD_ID searchForNode(const BDD_ID _highId, const BDD_ID _lowId, const BDD_ID _topVar);
    public:
        /// Fail constant
        const static BDD_ID MANAGER_FAIL    = -1;
        /// Constructor
        Manager(void);
        /// Destructor
        ~Manager(void);
        /**
        * True
        *
        * @return ID for the node representing "true"
        */
        const BDD_ID &True(void);
        /**
        * False
        *
        * @return ID for the node representing "false"
        */
        const BDD_ID &False(void);
        /**
        * isConstant
        *
        * @param x BDD_ID that should be checked
        *
        * @return True if x is a leaf/terminal node
        */
        const bool isConstant(const BDD_ID &x);
        /**
        * isVariable
        *
        * @param x BDD_ID that should be checked
        *
        * @return True if x is a variable/non-terminal node
        */
        const bool isVariable(const BDD_ID &x);
        /**
        * createVar
        *
        * @param label The label that will be used to create the node and insert to the table
        *
        * @return BDD_ID if it is a new variable, otherewise, BDD_ID of existing node
        */
        const BDD_ID createVar(const std::string &label);
        /**
        * uniqueTableSize
        *
        * @return Number of the nodes currently in the unique table
        */
        const std::size_t uniqueTableSize(void);
        /**
        * topVar
        *
        * @param f ID of the node for top variable request
        *
        * @return BDD_ID of top variable of node f
        */
        const BDD_ID topVar(const BDD_ID &f);
        /**
        * getTopVarName
        *
        * @param f ID of the node for label request of its top variable
        *
        * @return Label of top variable of node f
        */
        const std::string getTopVarName(const BDD_ID &f);
        /**
        * ite
        *
        * @param i ID of the node for the "if" node for the if-then-else operation
        * @param t ID of the node for the "then" node for the if-then-else operation
        * @param e ID of the node for the "else" node for the if-then-else operation
        *
        * @return ID representation of the ITE operation
        */
        const BDD_ID ite(const BDD_ID &i, const BDD_ID &t, const BDD_ID &e);
        /**
        * coFactorFalse
        *
        * @param f ID of the node for a function 'f'
        *
        * @return ID representation of the negative cofactor defined by the function 'f'
        */
        const BDD_ID coFactorFalse(const BDD_ID &f);        
        /**
        * coFactorTrue
        *
        * @param f ID of the node for a function 'f'
        *
        * @return ID representation of the positive cofactor defined by the function 'f'
        */        
        const BDD_ID coFactorTrue(const BDD_ID &f);
        /**
        * coFactorFalse
        *
        * @param f ID of the node for a function 'f'
        * @param x ID of the node for a variable in function 'f'
        *
        * @return ID representation of the negative cofactor for f(x), x = 0
        */
        const BDD_ID coFactorFalse(const BDD_ID &f, BDD_ID &x);        
        /**
        * coFactorTrue
        *
        * @param f ID of the node for a function 'f'
        * @param x ID of the node for a variable in function 'f'
        *
        * @return ID representation of the positive cofactor for f(x), x = 1
        */
        const BDD_ID coFactorTrue(const BDD_ID &f, BDD_ID &x);        
        /**
        * findNodes
        *
        * @param root ID for 'starting' point of sub-graph request
        * @param nodesOfRoot Set where all reachable nodes from root, including root, are saved
        *
        * @return ID None
        */
        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodesOfRoot);
        /**
        * findVars
        *
        * @param root ID for 'starting' point of top variable sub-graph request
        * @param varsOfRoot Set where all reachable nodes' top varaibles from root, including root, are saved
        *
        * @return ID None
        */
        void findVars(const BDD_ID &root, std::set<BDD_ID> &varsOfRoot);
        /**
        * neg
        *
        * @param a ID of node to be inverted
        *
        * @return ID of 'a' negation
        */
	    const BDD_ID neg(const BDD_ID &a);
        /**
        * and2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' AND 'b' operation
        */
	    const BDD_ID and2(const BDD_ID &a, const BDD_ID &b);
        /**
        * nand2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' NAND 'b' operation
        */
        const BDD_ID nand2(const BDD_ID &a, const BDD_ID &b);
        /**
        * or2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' OR 'b' operation
        */
	    const BDD_ID or2(const BDD_ID &a, const BDD_ID &b);
        /**
        * xor2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' XOR 'b' operation
        */
	    const BDD_ID xor2(const BDD_ID &a, const BDD_ID &b);
        /**
        * nor2
        *
        * @param a ID of first operand
        * @param b ID of second operand
        *
        * @return ID of 'a' NOR 'b' operation
        */
	    const BDD_ID nor2(const BDD_ID &a, const BDD_ID &b);
        /**
        * printTables
        *
        * @brief Prints all tables
        */
        void printTables(void);
    }; // class Manager
} // namespace ClassProject
#endif /* __MANAGER_H__ */
