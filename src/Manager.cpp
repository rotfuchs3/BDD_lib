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
    uniqueTable = std::unordered_map<BDD_ID, Node*>();
    lookUpTable = std::unordered_map<std::string,BDD_ID>();
    // Insert 0 and configure
    createVar("0");
    uniqueTable[0]->highId = 0;
    //uniqueTable[0]->lowId  = 0;
    //uniqueTable[0]->topVar = 0;

    // Insert 1 and configure
    createVar("1");
    //uniqueTable[1]->highId = 1;
    uniqueTable[1]->lowId  = 1;
    //uniqueTable[1]->topVar = 1;
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
    if(x == trueId || x == falseId) {
        return false;
    }
    return true;
}
/*! createVar
    Creates a new variable for the BDD
*/
BDD_ID Manager::createVar(const std::string &label)
{
	std::unordered_map<std::string,BDD_ID>::const_iterator item=lookUpTable.find(label);
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
	std::cout<<"Beginn:ite("<<i<<","<<t<<","<<e<<")"<<std::endl;
	if(!isValidID(i,t,e)){
			return -2;
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

		for(auto it=uniqueTable.begin(); it != uniqueTable.end()&&it->first<currentId; ++it ){
			Node* iterateNode=it->second;
			BDD_ID topVar=iterateNode->topVar;
			BDD_ID highId=iterateNode->highId;
			BDD_ID lowId=iterateNode->lowId;
			std::cout<<"Node(begin): (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
			// there is a node already in the unique table

			if(topVar==i && highId==t && lowId==e){
				return it->first;
			}
		}
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
			std::cout<<"if: min= "<<min<<std::endl;
		}
		if(topVarT!=-1 && topVarT<min){
			min=topVarT;
			std::cout<<"then: min= "<<min<<std::endl;
		}
		if(topVarE!=-1 && topVarE<min){
			min=topVarE;
			std::cout<<"else: min= "<<min<<std::endl;
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
			for(auto it=uniqueTable.begin(); it != uniqueTable.end()&&it->first<currentId; ++it ){
				Node* iterateNode=it->second;
				BDD_ID topVar=iterateNode->topVar;
				BDD_ID highId=iterateNode->highId;
				BDD_ID lowId=iterateNode->lowId;
				std::cout<<"Node: (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
				// there is a node already in the unique table
				if(topVar==min && highId==rHigh && lowId==rLow){
					return it->first;
				}
			}
		}
		BDD_ID newNode= insertInUniquetable(rHigh,rLow,min,"");
		std::cout<<"  return newNode("<<rHigh<<","<<rLow<<","<<min<<")ID: "<<newNode<<std::endl;
		return newNode;
}

//! coFactorTrue
/*!
 Returns the positive cofactor of the function defined by f with respect to function x set to true.
*/
BDD_ID Manager::coFactorTrue(const BDD_ID f,BDD_ID x){
	if(!isValidID(f,x)){
			return -2;
	}

	if(!isVariable(x)){
		x=topVar(x);
	}

	Node* nodeF=uniqueTable.find(f)->second;
		BDD_ID topVarF=nodeF->topVar;

		//if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
		//therefor x hasn't a effect.
		if(topVarF>x){
			return nodeF->myId;
		}
		if(topVarF=x){
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
					for(auto it=uniqueTable.begin(); it != uniqueTable.end()&&it->first<currentId; ++it ){
						Node* iterateNode=it->second;
						BDD_ID topVar=iterateNode->topVar;
						BDD_ID highId=iterateNode->highId;
						BDD_ID lowId=iterateNode->lowId;
						//std::cout<<"Node: (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
						// there is a node already in the unique table
						if(topVar==topVarF && highId==coFacHigh && lowId==coFacLow){
							return it->first;
						}
					}
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
	return -1;
}

//! coFactorFalse
/*!
  Returns the negativ cofactor of the function defined by f with respect to function x set to false.
*/
BDD_ID Manager::coFactorFalse(const BDD_ID f,BDD_ID x){
	if(!isValidID(f,x)){
		return -2;
	}

	if(!isVariable(x)){
		x=topVar(x);
	}

	Node* nodeF=uniqueTable.find(f)->second;
		BDD_ID topVarF=nodeF->topVar;

		//if x> topvariable then x will never be a topVar in f because x is higher in the VarOrder
		//therefor x hasn't a effect.
		if(topVarF>x){
			return nodeF->myId;
		}
		if(topVarF=x){
			return nodeF->lowId;
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
					for(auto it=uniqueTable.begin(); it != uniqueTable.end()&&it->first<currentId; ++it ){
						Node* iterateNode=it->second;
						BDD_ID topVar=iterateNode->topVar;
						BDD_ID highId=iterateNode->highId;
						BDD_ID lowId=iterateNode->lowId;
						//std::cout<<"Node: (key,highId,lowId,topVar): ("<<iterateNode->myId<<","<<highId<<","<<lowId<<","<<topVar<<")"<<std::endl;
						// there is a node already in the unique table
						if(topVar==topVarF && highId==coFacHigh && lowId==coFacLow){
							return it->first;
						}
					}
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
	return -1;
}

//---------------------------------------------------------------------------------------------------------
//private Methodes
bool Manager::isValidID(BDD_ID i,BDD_ID t, BDD_ID e)
{
	//check for corrupted input
		if(i>=currentId||i<0)
		{
			std::cout<<"arg1 of the method is not a actual BDD_ID in the uniquetable"<<std::endl;
			return false;
		}
		if(t>=currentId||t<0)
		{
				std::cout<<"arg2 of the method is not a actual BDD_ID in the uniquetable"<<std::endl;
				return false;
		}
		if(e>=currentId||e<0)
		{
				std::cout<<"arg3 of the method is not a actual BDD_ID in the uniquetable"<<std::endl;
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
