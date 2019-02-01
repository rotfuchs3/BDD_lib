//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
/****** PUBLIC ******/
//! @return returns the XNOR of BDD IDs
BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b)
{
    return ite(a, b, neg(b));
}
/**
 * States are generated and stored in a vector.
 * These lsb (e.g. "s0")  is stored at location 0.
 * The msb(e.g. "s3") is stored at location size-1.
 * @return vector with the BDD_ID of each state bit
 */
const std::vector<BDD_ID> &Reachable::getStates() const{
    return states;
}
/**
 * Each state machine has an inital state. The inital state is provided as a vector.
 * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
 * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
 * @param stateVector provide the assignemtn for each state bit
 */
void Reachable::setInitState(const std::vector<bool>& stateVector){
    initialStates = &stateVector;
    if (stateVector.size() == state_var){
        BDD_ID tmp;
        for (int i = 0; i < state_var; ++i) {
            if( i==0 ){
                if (stateVector.at(0)){
                    tmp = 2 + state_var-1;
                }else{
                    tmp = 2 + 2*state_var-1;
                }
            }else{
                if (stateVector.at(i)) {
                    tmp = and2(tmp, 2 + state_var - 1 - i);
                }else {
                    tmp = and2(tmp, 2 + 2*state_var - 1 - i);
                }
            }

        }
        initialstate=tmp;
    }else{
        throw std::invalid_argument("too few or too many state vaiable in this initial state");
    }

}
/**
 * Each state variable has a transition function.
 * The transition function specifies the value of the state after the transition.
 * The transition function are composed of only state variables.
 * For example: s0' = s0 XOR s1
 * The next state is2 defined as XOR of the current values of the state bit s0 and s1
 * @param transitionFunctions provide a transition function exactly for each state bit
 */
void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions){
    if (transitionFunctions.size() == state_var){
        *transitons=transitionFunctions;
    }else{
        throw std::invalid_argument("too few or too many transitions for the amout of state variables");
    }

}
/**
 * Each state machine has an inital state. The inital state is provided as a vector.
 * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
 * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
 * @param stateVector provide the assignemtn for each state bit
 */
void Reachable::initStates() {
/*
    // one vector for positiv literals and one for negativ
    std::vector<BDD_ID>variablesPos=std::vector<BDD_ID>();
    std::vector<BDD_ID>variablesNeg=std::vector<BDD_ID>();
    std::vector<BDD_ID>* allStates = new std::vector<BDD_ID>();

    std::string label;

    for (int i = 0; i < state_var; ++i) {
        BDD_ID tmp= this->createVar("s"+std::to_string(i));
        variablesPos.push_back(tmp);
    }

    for (int i = 0; i < state_var; ++i) {
        BDD_ID notTmp=this->neg(variablesPos.at(i));
        variablesNeg.push_back(notTmp);
    }


    //printUniqueTable();

    bool  counter[state_var];
    bool finishedCounting =false;
    BDD_ID tmp;

    //idea counting up binary numbers form 0 to 2^state_var-1 and
    for (int i=0; i< state_var; i++){
        counter[i]= false;

        if (i==0){
            tmp=variablesNeg.at(0);
        }else {
            tmp = and2(tmp, variablesNeg.at(i));
        }
    }

    allstates.push_back(tmp);

    do{
        counter[0]=!counter[0];
        tmp = counter[0]?variablesPos.at(0):variablesNeg.at(0);
        // if the last bit is a 1 then it was step from 0 -> 1 so the current bit can't flip
        int j;
        for ( j = 1; j <  state_var && !counter[j-1]; ++j) {
            counter[j]=!counter[j];
            tmp = counter[j]?and2(tmp,variablesPos.at(j)):and2(tmp,variablesNeg.at(j));
        }
        // assign the rest of the bits unchanged for j...statevar-1;
        for ( int i = j; i <  state_var ; ++i) {
            tmp = counter[i]?and2(tmp,variablesPos.at(i)):and2(tmp,variablesNeg.at(i));
        }

        allstates.push_back(tmp);

        //stop tht do while loop when counter hits 11...1
        finishedCounting= true;
        for (int i=0; i< state_var && finishedCounting; i++){
            finishedCounting= counter[i]? true: false;
        }
    } while(!finishedCounting);

    states = allStates;
    */
}

/**
 * Computes the symbolic representation of the reachable states.
 * Before this method is called it is important to set the transition function and the initial state.
 * @return BDD_ID of the reachable state space
 */
BDD_ID Reachable::compute_reachable_states()
{
    /* Given transition function t and intial state S0
    *   {
    *       Reachable (R_it) = S0;   -> Initial state charachteristic function
    *       do
    *       {
    *           R = R_it;
    *           R_it = R set of elements in compute_reachable_states(t, R)
    *       } while (R_it == R);
    *       return R;
    */
    // Compute transition relation
    BDD_ID tau  = computeTransitionRelation();
    // Compute characteristic function
    BDD_ID c_s0 = computeCharFunction();
    // set char.function for Reachable state intial (cR_it) to c_s0
    BDD_ID cR_it    = c_s0;
    BDD_ID cR       = MANAGER_FAIL;
    BDD_ID char_trans_and   = 0;
    BDD_ID exen_qua         = 0;
    BDD_ID img      = 0;
    uint i          = 1;
    //do
    //{
        cR = cR_it;
        // Now compute the image
        // and char.function with trans.relation, used to compute img
        char_trans_and  = and2(cR, tau);
        // Exenstential qunatification of states
        exen_qua        = or2( \
                            coFactorTrue(char_trans_and, states.at(i)), \
                            coFactorFalse(char_trans_and, states.at(i)));
        // compute the set of immediate successors
        img  = 0;
        // Update the char.function
        cR_it = or2(cR, img);
        i++;
    //} while(cR == cR_it);
    return MANAGER_FAIL;
}
/**
 * This method decides whether a specific state is in the reachable state space or not.
 * The inpute is provided as a vector of states. The value is true if the state bit is high in this state, false otherwise.
 * @param stateVector 
 * @return
 */
bool Reachable::is_reachable(const std::vector<bool>& stateVector)
{
    return false;
}
/**
* printVectors
*
* @brief Prints all vectors
*/
void Reachable::printVectors(void)
{
    // states
    std::cout << " --- STATE VECTOR --- " << std::endl;
    for(uint i = 0; i < state_var; i++)
    {
        std::cout << "BDD_ID[" << states.at(i) << "]::s" << i << std::endl;
    }
    // delta vector
    std::cout << " --- DELTA VECTOR --- " << std::endl;
    for(auto it = transitons->begin(); it != transitons->end(); ++it)
    {
        //std::cout << *(uniqueTable.at((*it))) << std::endl;
    }
    // init states
    std::cout << " --- INITIAL STATE VECTOR --- " << std::endl;
    std::string s;
    for(uint i = 0; i < state_var; i++)
    {
        std::cout << "s" << i << ": ";
        initialStates->at(i) ? std::cout<<"true" : std::cout<<"false";
        std::cout << std::endl;
    }
}
/****** PRIVATE ******/
/**
* computeTransitionRelation
* @brief 
*   Computes the translation relation (tau) by, POS from i = 1 to states_var of:
*       [(s'i * transitons(si)) + (~s'i * ~transitons(si))]
*   Assumes:
*       Application provides transition function (transitons) for states and finite number of states (si)
* @return BDD_ID of transition relation
*/
BDD_ID Reachable::computeTransitionRelation(void)
{
    BDD_ID tau          = 0;
    BDD_ID tempTau      = 0;
    BDD_ID firstTerm    = 0;
    BDD_ID secndTerm    = 0;
    
    // Start with Products
    // s'0 (located at the end of all non-next state variables
    // (s'0 * transitons(s0)) 
    firstTerm   = and2(states.at(state_var), transitons->at(0));
    // (~s'0 * ~transitons(s0))
    secndTerm   = and2(neg(states.at(state_var)), neg(transitons->at(0)));
    // Sum
    tau         = or2(firstTerm, secndTerm);
    for(uint i = 1; i < state_var; i++)
    {
        // POS from i = 1 to states_var:
        // Products, (s'i * transitons(si)) 
        firstTerm   = and2(states.at(i), transitons->at(i));
        secndTerm   = and2(neg(states.at(state_var+i)), neg(transitons->at(i)));
        // Sum
        tempTau     = or2(firstTerm, secndTerm);
        // New tau
        tau         = and2(tau, tempTau);
    }
    std::cout << "Transition relation: BDD_ID[" << tau << "]" << std::endl;
    return tau;
}
/**
* computeCharFunction
* @brief 
*   Computes the characteristic function by, c_s0 = and2(s0 == initialStates(0), ..., si == initialStates(i))
*   Assumes: 
*       Application provides initial state bits for given number of states.
* @return BDD_ID of characteristic function
*/
BDD_ID Reachable::computeCharFunction(void)
{
    // Compute characteristic function of initial state s0
    BDD_ID tmp  = 0;
    BDD_ID c_s0 = xnor2(states.at(0), initialStates->at(0));
    for(uint i = 1; i < state_var; i++)
    {
        tmp     = xnor2(states.at(i), initialStates->at(i));
        c_s0    = and2(c_s0, tmp);
    }
    std::cout << "Char.Function s0: BDD_ID[" << c_s0 << "]" << std::endl;
    return c_s0;
}
