typedef int BDD_ID;
class node {
public:
	BDD_ID myId;
	BDD_ID trueId;
	BDD_ID falseId;
	BDD_ID topVar;
	string label;
}