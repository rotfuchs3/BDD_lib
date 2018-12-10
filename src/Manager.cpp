/**
 * \defgroup bddPkg
 * \brief Manager is a derived class implementing all ManagerInterface's methods.
 * \file Manager.cpp
 * \author vdscp_8
*/
/// Header
#include "Manager.h"
//! Constructor
/*!
    Initializes uniqueTable
*/
Manager::Manager(void)
{
    uniqueTable = uniqueTable_t();
    lookUpTable = lookUpTable_t();
    
    // Insert 0 and configure
    createVar("0");
    uniqueTable[0]->highId = 0;

    // Insert 1 and configure
    createVar("1");
    uniqueTable[1]->lowId  = 1;
}
//! True
/*!
    Returns the ID of the node representing True
*/
const BDD_ID &Manager::True(void)
{
    return trueId;
}
//! False
/*!
    Returns the ID of the node representing False
*/
const BDD_ID &Manager::False(void)
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
    // check if BDD_ID exists and if it's not 0 or 1
    if( (searchUniTable(x) != MANAGER_FAIL) && !isConstant(x) ) {
        // check highId - check if highId and lowId is either 1 or 0
        if( (uniqueTable[x]->highId <= trueId) && (uniqueTable[x]->lowId <= trueId) ) {
            return true;
        }
    }
    return false;
}
/*! createVar
    Creates a new variable for the BDD
*/
BDD_ID Manager::createVar(const std::string &label)
{
	lookUpTable_t::const_iterator item=lookUpTable.find(label);
	//when label is not in the hashmap
	if(item == lookUpTable.end()){
		return insertInUniquetable(True(),False(),currentId,label);
	}else{
		return item->second;
	}
}
/*! uniqueTableSize
    Returns the number of the nodes currently exist in the unique table
*/
std::size_t Manager::uniqueTableSize(void)
{
    return uniqueTable.size();
}
//! topVar
    /*!
        Returns the ID of top variable of the BDD node f
    */
BDD_ID Manager::topVar(const BDD_ID f)
{
    return uniqueTable[f]->topVar;
}

//! ite
/*!
 Implements the if-then-else algorithm.
 Returns the new node that represents the ITE.
*/
BDD_ID Manager::ite(const BDD_ID i,const BDD_ID t, const BDD_ID e)
{
	if(!isValidID(i,t,e)){
			return MANAGER_FAIL;
	}
		//terminal cases
		if(t==e||i==1)
		{
			return t;
		}
		if(i==0)
		{
			return e;
		}
		BDD_ID alreadyExist = searchForNode(t,e, i);
		if (alreadyExist !=MANAGER_FAIL)
			return alreadyExist;

		//termine highest topvariable depending on the variable order
		BDD_ID topVarI=-1,topVarT=-1,topVarE=-1;
		//Constant have topVar 1/0 but this is no variable
		//so it should not be considered by the highest topVariable depending on the variable order
		//missing this leads to
		if(!isConstant(i))
			topVarI=uniqueTable.find(i)->second->topVar;
		if(!isConstant(t))
			topVarT=uniqueTable.find(t)->second->topVar;
		if(!isConstant(e))
			topVarE=uniqueTable.find(e)->second->topVar;

		//minimal variable are the highest in the variable order
		BDD_ID min=currentId;
		if(topVarI!=-1){
			min=topVarI;
		}
		if(topVarT!=-1 && topVarT<min){
			min=topVarT;
		}
		if(topVarE!=-1 && topVarE<min){
			min=topVarE;
		}
		//new nodes?
		BDD_ID rHigh=ite(coFactorTrue(i,min),coFactorTrue(t,min),coFactorTrue(e,min));
		BDD_ID rLow=ite(coFactorFalse(i,min),coFactorFalse(t,min),coFactorFalse(e,min));

		//a new internal node isn't needed
		if(rHigh==rLow){
			return rHigh;
		}
		//check if the to return coFactor would be a constant
		if(rHigh==1 && rLow==0){
			return min;
		}else{
			// check if already an internal node (rHigh,rLow,min) is in the UniqueTable
			BDD_ID alreadyExist = searchForNode(rHigh,rLow, min);
			if (alreadyExist !=MANAGER_FAIL)
				return alreadyExist;
		}
		BDD_ID newNode= insertInUniquetable(rHigh,rLow,min,"");
		return newNode;
}

//! coFactorTrue
/*!
 Returns the positive cofactor of the function defined by f with respect to function x set to true.
*/
BDD_ID Manager::coFactorTrue(const BDD_ID f,BDD_ID x){
	if(!isValidID(f,x)){
			return MANAGER_FAIL;
	}

	if(!isVariable(x)){
		x=topVar(x);
	}

	if(isConstant(f)){
		return f;
	}

	Node* nodeF=uniqueTable.find(f)->second;
		BDD_ID topVarF=nodeF->topVar;

		//if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
		//therefor x hasn't a effect.
		if(topVarF>x){
			return nodeF->myId;
		}
		if(topVarF==x){
			return nodeF->highId;
		}else{//topVarF<x so x is more down in the BDD

			BDD_ID coFacHigh=coFactorTrue(nodeF->highId,x);
			BDD_ID coFacLow=coFactorTrue(nodeF->lowId,x);

			//same child so the topVar topVarF can be skipped
			if(coFacHigh==coFacLow){
				return coFacHigh;
			}else{
				//check if the to return coFactor would be a constant
				if(coFacHigh==1 && coFacLow==0){
					return topVarF;
				}else{
					BDD_ID alreadyExist = searchForNode(coFacHigh,coFacLow, topVarF);
					if (alreadyExist !=MANAGER_FAIL)
						return alreadyExist;
					//when no node fond
					std::string newLabel="coFactorTrue("+nodeF->label+","+uniqueTable.find(x)->second->label+")";
					return insertInUniquetable(coFacHigh,coFacLow,topVarF,newLabel);
				}
			}
		}
}

//! coFactorTrue
/*!
 Returns the positive cofactor of the function defined by f.
*/
BDD_ID Manager::coFactorTrue(const BDD_ID f){
	if(!isValidID(f)){
			return MANAGER_FAIL;
	}
	return uniqueTable.find(f)->second->highId;
}

//! coFactorFalse
/*!
  Returns the negativ cofactor of the function defined by f with respect to function x set to false.
*/
BDD_ID Manager::coFactorFalse(const BDD_ID f,BDD_ID x){
	if(!isValidID(f,x)){
		return MANAGER_FAIL;
	}

	if(!isVariable(x)){
		x=topVar(x);
	}

	if(isConstant(f)){
		return f;
	}

	Node* nodeF=uniqueTable.find(f)->second;
	BDD_ID topVarF=nodeF->topVar;

	//if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
	//therefor x hasn't a effect.
	if(topVarF>x){
		return nodeF->myId;
	}
	if(topVarF==x){
		return nodeF->lowId;
	}else{//topVarF<x so x is more down in the BDD
		BDD_ID coFacHigh=coFactorFalse(nodeF->highId,x);
		BDD_ID coFacLow=coFactorFalse(nodeF->lowId,x);

		//same child so the topVar topVarF can be skipped
		if(coFacHigh==coFacLow){
			return coFacHigh;
		}else{
			//check if the to return coFactor would be a constant
			if(coFacHigh==1 && coFacLow==0){
				return topVarF;
			}else{
				BDD_ID alreadyExist = searchForNode(coFacHigh,coFacLow, topVarF);
				if (alreadyExist !=MANAGER_FAIL)
					return alreadyExist;
				//when no node found
				std::string newLabel="coFactorFalse("+nodeF->label+","+uniqueTable.find(x)->second->label+")";
				return insertInUniquetable(coFacHigh,coFacLow,topVarF,newLabel);
			}
		}
	}
}
//! coFactorFalse
/*!
  Returns the negativ cofactor of the function defined by f.
*/
BDD_ID Manager::coFactorFalse(const BDD_ID f) {
	if(!isValidID(f)){
		return MANAGER_FAIL;
	}
	return uniqueTable.find(f)->second->lowId;
}

//! coFactorFalse
/*!
  Returns the set of BDD nodes which are reachable
  from the BDD node root(including itself).
*/
void Manager::findNodes(BDD_ID root,std::set<BDD_ID> &nodes_of_root){
	if(!isValidID(root)){
			return ;
	}
	nodes_of_root.insert(root);
	if(isConstant(root)){
		return;
	}else{
		Node* rootNode=uniqueTable.find(root)->second;
		BDD_ID high_of_root = rootNode->highId;
		BDD_ID low_of_root = rootNode->lowId;
		findNodes(high_of_root,nodes_of_root);
		findNodes(low_of_root,nodes_of_root);
	}
}

//! findVars
/*!
  Returns the set of variables which are either top variable of the BDD node root
   or the reachable nodes from root.
*/
void Manager::findVars(BDD_ID root,std::set<BDD_ID> &vars_of_root){
	if(!isValidID(root)){
			return ;
	}
	if(isConstant(root)){
		vars_of_root.insert(root);
		return;
	}else{
		Node* rootNode=uniqueTable.find(root)->second;
		vars_of_root.insert(rootNode->topVar);
		BDD_ID high_of_root = rootNode->highId;
		BDD_ID low_of_root = rootNode->lowId;
		findVars(high_of_root,vars_of_root);
		findVars(low_of_root,vars_of_root);
	}
}

//! neg
/*!
  Returns BDD_ID of the negation of A if needed creats this node
*/
BDD_ID Manager::neg(const BDD_ID a){
	if(!isValidID(a)){
		return MANAGER_FAIL;
	}
	return ite(a,0,1);
}

//! and
/*!
  Returns BDD_ID of the Conjunktion of A and B. if needed creates this node
*/
BDD_ID Manager::and2(const BDD_ID a,const BDD_ID b){
	if(!isValidID(a,b)){
		return MANAGER_FAIL;
	}
	return ite(a,b,0);
}

//! and
/*!
  Returns BDD_ID of the NAND with A and B. if needed creates this node
*/
BDD_ID Manager::nand2(const BDD_ID a,const BDD_ID b){
	if(!isValidID(a,b)){
		return MANAGER_FAIL;
	}
	return ite(a,ite(b,0,1),1);
}

//! getTopVarName
/*!
  Returns the label of top variable of the BDD node f
*/
std::string Manager::getTopVarName(const BDD_ID f){
	return "";
}

//---------------------------------------------------------------------------------------------------------
//private Methodes
bool Manager::isValidID(BDD_ID i,BDD_ID t, BDD_ID e)
{
	//check for corrupted input
		if(i>=currentId||i<0)
		{
			return false;
		}
		if(t>=currentId||t<0)
		{
				return false;
		}
		if(e>=currentId||e<0)
		{
				return false;
		}
		return true;
}

BDD_ID Manager::insertInUniquetable(BDD_ID highID,BDD_ID lowID,BDD_ID topVar,std::string label)
{
	Node *newNode = new Node(currentId, highID, lowID, topVar, label);
	// Insert new node to map
	uniqueTable.insert({currentId, newNode});
	lookUpTable.insert({label,currentId});

	return currentId++;
}
BDD_ID Manager::searchUniTable(const BDD_ID id)
{
    uniqueTable_t::const_iterator found = uniqueTable.find(id);
    if(found != uniqueTable.end()) {
        return found->first;
    }
    return MANAGER_FAIL;
}

BDD_ID Manager::searchForNode(const BDD_ID _highId,const BDD_ID _lowId, const BDD_ID _topVar){
	for(auto it=uniqueTable.begin(); it != uniqueTable.end()&&it->first<currentId; ++it ){
		Node* iterateNode=it->second;
		BDD_ID topVar=iterateNode->topVar;
		BDD_ID highId=iterateNode->highId;
		BDD_ID lowId=iterateNode->lowId;
		//std::cout<<"Node: (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
		// there is a node already in the unique table
		if(topVar==_topVar && highId==_highId && lowId==_lowId){
			return it->first;
		}
	}
	return MANAGER_FAIL;
}
