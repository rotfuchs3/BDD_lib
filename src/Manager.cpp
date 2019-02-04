/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.cpp
 * \author vdscp_8
*/
/// Header
#include "Manager.h"
/// Add specified namespace
namespace ClassProject {
    // Define static vars
    const BDD_ID Manager::trueId;
    const BDD_ID Manager::falseId;
    const BDD_ID Manager::MANAGER_FAIL;
    /// Constructor
    Manager::Manager(void)
    {   
        uniqueTable 	= uniqueTable_t();
        computeTable 	= computeTable_t();
        lookUpTable     = lookUpTable_t();

        // Insert 0 and configure
        createVar("0");
        (uniqueTable.at(0))->highId = falseId;
        // Insert 1 and configure
        createVar("1");
        (uniqueTable.at(1))->lowId = trueId;
    }
    /// Destructor
    Manager::~Manager(void) {}
    /* -------------- Private Methods -------------- */
    /// Checks validity of variables without searching tables
    bool Manager::isValidID(BDD_ID i, BDD_ID t, BDD_ID e)
    {
        // check for corrupted input
        if (i >= currentId || i < 0)
        {
          return false;
        }
        if (t >= currentId || t < 0)
        {
          return false;
        }
        if (e >= currentId || e < 0)
        {
          return false;
        }
        return true;
    }
    /// Inserts into unique table and returns it's ID
    BDD_ID Manager::insertInUniquetable(BDD_ID highID, BDD_ID lowID, 
                                        BDD_ID topVar, std::string label)
    {
        // Create node and insert
        Node *newNode = new Node(currentId, highID, lowID, topVar, label);
        // Create new key and insert to tables
        uniqueTable.push_back(newNode);
        lookUpTable.insert({label, currentId});
        ITE_ID key(topVar, highID, lowID);
        computeTable.insert({key, currentId});
        return currentId++;
    }
    /// Searches compute table for given variables and returns the BDD_ID with corresponding values
    BDD_ID Manager::searchForNode(const BDD_ID _highId, const BDD_ID _lowId, const BDD_ID _topVar)
    {
        // define new key for compute table
        ITE_ID key(_topVar, _highId, _lowId);
        cIter_t found = computeTable.find(key);
        if(found != computeTable.end())
        {
           return found->second;
        }
        return MANAGER_FAIL;
    }
    /// returns the BDD_ID of the BDD_ID with these (topvar, highId, lowID) as a root
    BDD_ID Manager::ite_return_logic(const BDD_ID _topVar ,const BDD_ID rHigh, const BDD_ID rLow){
        //a new internal node isn't needed
        if(rHigh == rLow)
        {
            return rHigh;
        }
        //check if the to return coFactor would be a constant
        if(rHigh == 1 && rLow == 0)
        {
            return _topVar;
        }
        else
        {
            // check if already an internal node (rHigh,rLow,min) is in the UniqueTable
            BDD_ID alreadyExist = searchForNode(rHigh, rLow, _topVar);
            if (alreadyExist != MANAGER_FAIL)
            {
                return alreadyExist;
            }
        }
        BDD_ID newNode = insertInUniquetable(rHigh, rLow, _topVar, "");
        return newNode;
    }
    /**
    * True
    *
    * @return ID for the node representing "true"
    */
    const BDD_ID &Manager::True(void)
    {
      return trueId;
    }
    /**
    * False
    *
    * @return ID for the node representing "false"
    */
    const BDD_ID &Manager::False(void)
    {
      return falseId;
    }
    /**
    * isConstant
    *
    * @param x BDD_ID that should be checked
    *
    * @return True if x is a leaf/terminal node
    */
    bool Manager::isConstant(const BDD_ID x)
    {
      return (x == trueId || x == falseId);
    }
    /**
    * isVariable
    *
    * @param x BDD_ID that should be checked
    *
    * @return True if x is a variable/non-terminal node
    */
    bool Manager::isVariable(const BDD_ID x)
    {
        // check if BDD_ID exists and if it's not 0 or 1
        if ( (x < 0) || (x > currentId)  || isConstant(x) )
        {
            return false;
        }
        // check highId == 1 and  check lowId == 0
        return ( (uniqueTable[x]->highId == trueId) && (uniqueTable[x]->lowId == falseId) );
    }
    /**
    * createVar
    *
    * @param label The label that will be used to create the node and insert to the table
    *
    * @return BDD_ID if it is a new variable, otherewise, BDD_ID of existing node
    */
    BDD_ID Manager::createVar(const std::string &label)
    {
        lIter_t item = lookUpTable.find(label);
        // when label is not in the hashmap
        if (item == lookUpTable.end())
        {
            return insertInUniquetable(True(), False(), currentId, label);
        }
        else
        {
            return item->second;
        }
    }
    /**
    * uniqueTableSize
    *
    * @return Number of the nodes currently in the unique table
    */
    std::size_t Manager::uniqueTableSize(void)
    {
      return uniqueTable.size()-2;
    }
    /**
    * topVar
    *
    * @param f ID of the node for top variable request
    *
    * @return BDD_ID of top variable of node f
    */
    BDD_ID Manager::topVar(const BDD_ID f)
    {
      return uniqueTable[f]->topVar;
    }
    /**
    * getTopVarName
    *
    * @param f ID of the node for label request of its top variable
    *
    * @return Label of top variable of node f
    */
    std::string Manager::getTopVarName(const BDD_ID f)
    {
        if (!isValidID(f))
        {
            return "";
        }
        return uniqueTable[topVar(f)]->label;
    }
    /**
    * ite
    *
    * @param i ID of the node for the "if" node for the if-then-else operation
    * @param t ID of the node for the "then" node for the if-then-else operation
    * @param e ID of the node for the "else" node for the if-then-else operation
    *
    * @return ID representation of the ITE operation
    */
    BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
    {
        if (!isValidID(i, t, e))
        {
            return MANAGER_FAIL;
        }
        // terminal cases
        if (t == e || i == 1)
        {
            return t;
        }
        if (i == 0)
        {
            return e;
        }
        BDD_ID alreadyExist = searchForNode(t, e, i);
        if (alreadyExist != MANAGER_FAIL)
        {
            return alreadyExist;
        }
        // termine highest topvariable depending on the variable order
        BDD_ID topVarI = MANAGER_FAIL, topVarT = MANAGER_FAIL, topVarE = MANAGER_FAIL;
        //Constant have topVar 1/0 but this is no variable
        //so it should not be considered by the highest topVariable depending on the variable order
        //missing this leads to
        if (!isConstant(i)) 
        {
            topVarI = uniqueTable[i]->topVar;
        }
        if (!isConstant(t))
        {
            topVarT = uniqueTable[t]->topVar;
        }
        if (!isConstant(e))
        {
            topVarE = uniqueTable[e]->topVar;
        }

        // minimal variable are the highest in the variable order
        BDD_ID min = currentId;
        if(topVarI != MANAGER_FAIL)
        {
            min = topVarI;
        }
        if( (topVarT != MANAGER_FAIL) && (topVarT < min) )
        {
            min = topVarT;
        }
        if( (topVarE != MANAGER_FAIL) && (topVarE < min) )
        {
            min = topVarE;
        }
        //new nodes?
        BDD_ID rHigh = ite(coFactorTrue(i, min), coFactorTrue(t, min), coFactorTrue(e, min));
        BDD_ID rLow  = ite(coFactorFalse(i, min), coFactorFalse(t, min), coFactorFalse(e, min));

        BDD_ID erg = ite_return_logic(min,rHigh,rLow);

        ITE_ID key(i, t, e);
        computeTable.insert({key, erg});

        return erg;
    }
    /**
    * coFactorTrue
    *
    * @param f ID of the node for a function 'f'
    * @param x ID of the node for a variable in function 'f'
    *
    * @return ID representation of the positive cofactor for f(x), x = 1
    */
    BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x)
    {
        // Check input
        if(!isValidID(f, x))
        {
            return MANAGER_FAIL;
        }
        if(!isVariable(x))
        {
            x = topVar(x);
        }
        if(isConstant(f))
        {
            return f;
        }
        Node *nodeF = uniqueTable[f];
        BDD_ID topVarF = nodeF->topVar;
        // if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
        //t herefor x hasn't a effect.
        if(topVarF > x)
        {
            return nodeF->myId;
        }
        if(topVarF == x)
        {
            return nodeF->highId;
        }
        else
        { 
            // topVarF < x so x is more down in the BDD
            BDD_ID coFacHigh = coFactorTrue(nodeF->highId, x);
            BDD_ID coFacLow  = coFactorTrue(nodeF->lowId, x);

            BDD_ID erg = ite_return_logic(topVarF,coFacHigh,coFacLow);
            return erg;
        }
    }
    /**
    * coFactorTrue
    *
    * @param f ID of the node for a function 'f'
    *
    * @return ID representation of the positive cofactor defined by the function 'f'
    */   
    BDD_ID Manager::coFactorTrue(const BDD_ID f)
    {
        if (!isValidID(f))
        {
            return MANAGER_FAIL;
        }
        return uniqueTable[f]->highId;
    }
    /**
    * coFactorFalse
    *
    * @param f ID of the node for a function 'f'
    * @param x ID of the node for a variable in function 'f'
    *
    * @return ID representation of the negative cofactor for f(x), x = 0
    */
    BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x)
    {
        if(!isValidID(f, x))
        {
            return MANAGER_FAIL;
        }
        if(!isVariable(x))
        {
            x = topVar(x);
        }
        if(isConstant(f))
        {
            return f;
        }
        Node *nodeF = uniqueTable[f];
        BDD_ID topVarF = nodeF->topVar;
        // if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
        // therefor x hasn't a effect.
        if(topVarF > x)
        {
            return nodeF->myId;
        }
        if(topVarF == x)
        {
            return nodeF->lowId;
        }
        else
        {
            // topVarF < x so x is more down in the BDD
            BDD_ID coFacHigh = coFactorFalse(nodeF->highId, x);
            BDD_ID coFacLow = coFactorFalse(nodeF->lowId, x);

            BDD_ID erg = ite_return_logic(topVarF,coFacHigh,coFacLow);
            return  erg;
        }
    }
    /**
    * coFactorFalse
    *
    * @param f ID of the node for a function 'f'
    *
    * @return ID representation of the negative cofactor defined by the function 'f'
    */
    BDD_ID Manager::coFactorFalse(const BDD_ID f)
    {
        if (!isValidID(f))
        {
            return MANAGER_FAIL;
        }
        return uniqueTable[f]->lowId;
    }
    /**
    * findNodes
    *
    * @param root ID for 'starting' point of sub-graph request
    * @param nodesOfRoot Set where all reachable nodes from root, including root, are saved
    *
    * @return ID None
    */
    void Manager::findNodes(BDD_ID root, std::set<BDD_ID> &nodes_of_root)
    {
        if(!isValidID(root))
        {
            return ;
        }
        nodes_of_root.insert(root);
        if(isConstant(root))
        {
            return;
        }
        else
        {
            Node *rootNode      = uniqueTable[root];
            BDD_ID high_of_root = rootNode->highId;
            BDD_ID low_of_root  = rootNode->lowId;
            findNodes(high_of_root, nodes_of_root);
            findNodes(low_of_root, nodes_of_root);
        }
    }
    /**
    * findVars
    *
    * @param root ID for 'starting' point of top variable sub-graph request
    * @param varsOfRoot Set where all reachable nodes' top varaibles from root, including root, are saved
    *
    * @return ID None
    */
    void Manager::findVars(BDD_ID root, std::set<BDD_ID> &vars_of_root)
    {
        if (!isValidID(root))
        {
            return ;
        }
        if (isConstant(root))
        {
            vars_of_root.insert(root);
            return;
        }
        else
        {
            Node *rootNode = uniqueTable[root];
            vars_of_root.insert(rootNode->topVar);
            BDD_ID high_of_root = rootNode->highId;
            BDD_ID low_of_root = rootNode->lowId;
            findVars(high_of_root, vars_of_root);
            findVars(low_of_root, vars_of_root);
        }
    }
    /**
    * neg
    *
    * @param a ID of node to be inverted
    *
    * @return ID of 'a' negation
    */
    BDD_ID Manager::neg(const BDD_ID a)
    {
        return ite(a, 0, 1);
    }
    /**
    * and2
    *
    * @param a ID of first operand
    * @param b ID of second operand
    *
    * @return ID of 'a' AND 'b' operation
    */
    BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b)
    {
        return ite(a, b, 0);
    }
    /**
    * nand2
    *
    * @param a ID of first operand
    * @param b ID of second operand
    *
    * @return ID of 'a' NAND 'b' operation
    */
    BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b)
    {
        return neg(and2(a, b));
    }
    /**
    * or2
    *
    * @param a ID of first operand
    * @param b ID of second operand
    *
    * @return ID of 'a' OR 'b' operation
    */
    BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b)
    {
        return ite(a, 1, b);
    }
    /**
    * xor2
    *
    * @param a ID of first operand
    * @param b ID of second operand
    *
    * @return ID of 'a' XOR 'b' operation
    */
    BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b)
    {
        return ite(a, neg(b), b);
    }
    /**
    * nor2
    *
    * @param a ID of first operand
    * @param b ID of second operand
    *
    * @return ID of 'a' NOR 'b' operation
    */
    BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b)
    {
        return neg(or2(a, b));
    }
    /**
    * printTables
    *
    * @brief Prints all tables
    */
    void Manager::printTables(void)
    {
        // Unique table
        std::cout << " --- UNIQUE TABLE --- " << std::endl;
        for(auto it = uniqueTable.begin(); it != uniqueTable.end(); ++it)
        {
            std::cout << *(*it) << std::endl;
        }  
        // Compute table
        std::cout << " --- COMPUTE TABLE --- " << std::endl;
        for(auto it = computeTable.begin(); it != computeTable.end(); ++it)
        {
            std::cout << "BDD_ID[" << it->second << "]: " << it->first << std::endl;
        }
        std::cout << " --- LOOKUP TABLE --- " << std::endl;
        for(auto it = lookUpTable.begin(); it != lookUpTable.end(); ++it)
        {
            std::cout << "Label[" << it->first << "], BDD_ID[" << it->second << "]" << std::endl;
        }
    }
} // namespace ClassProject
