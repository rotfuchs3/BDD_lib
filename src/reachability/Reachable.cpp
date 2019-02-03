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
const std::vector<BDD_ID> &Reachable::getStates() const 
{
    return states;
}
/**
 * Each state machine has an inital state. The inital state is provided as a vector.
 * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
 * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
 * @param stateVector provide the assignemtn for each state bit
 */
void Reachable::setInitState(const std::vector<bool>& stateVector) {
    initialStates = &stateVector;
    return;
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
    transitions = &transitionFunctions;
    return;
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
    BDD_ID cR_it            = c_s0;
    BDD_ID cR               = MANAGER_FAIL;
    BDD_ID img_sNext        = 0;
    //do
    //{
        cR = cR_it;
        // Now compute the image
        img_sNext = computeImage(cR, tau);
        std::cout << "img_sNext: " << img_sNext << std::endl;
        // compute the set of immediate successors
        // Update the char.function
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
    for(auto it = transitions->begin(); it != transitions->end(); ++it)
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
*       [(s'i * transitions(si)) + (~s'i * ~transitions(si))]
*   Assumes:
*       Application provides transition function (transitions) for states and finite number of states (si)
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
    // (s'0 * transitions(s0)) 
    firstTerm   = and2(states.at(state_var), transitions->at(0));
    // (~s'0 * ~transitions(s0))
    secndTerm   = and2(neg(states.at(state_var)), neg(transitions->at(0)));
    // Sum
    tau         = or2(firstTerm, secndTerm);
    for(uint i = 1; i < state_var; i++)
    {
        // POS from i = 1 to states_var:
        // Products, (s'i * transitions(si)) 
        firstTerm   = and2(states.at(state_var + i), transitions->at(i));
        secndTerm   = and2(neg(states.at(state_var + i)), neg(transitions->at(i)));
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
    BDD_ID c_s0 = xnor2(lsb, initialStates->at(0));
    for(uint i = 1; i < state_var; i++)
    {
        tmp     = xnor2(lsb + i, initialStates->at(i));
        c_s0    = and2(c_s0, tmp);
    }
    std::cout << "Char.Function s0: BDD_ID[" << c_s0 << "]" << std::endl;
    return c_s0;
}
/**
* computeImage
* @brief 
*   Computes the image for s'i by, imge = E_si * cR * tau.
*   E_si refers to the true and false cofactoring of si
*   Assumes: 
*       Application provides initial state bits for given number of states.
*
* @param cR Characteristic function of state set
* @param tau Transition relation
* @return BDD_ID of image computation
*/
BDD_ID Reachable::computeImage(BDD_ID cR, BDD_ID tau)
{
    // and characeristic function with transition relation
    BDD_ID tmp  = and2(cR, tau);
    BDD_ID coHi = MANAGER_FAIL;
    BDD_ID coLo = MANAGER_FAIL;
    for(uint s = msb; s > lsb; s--)
    {
        // Find exenstential qunatification in realation to si for (cR * tau), done through cofactoring
        coHi    = coFactorTrue(tmp, s);  // Starts from the last state
        coLo    = coFactorFalse(tmp, s);
        // [(cR * tau)|si = 1] + [(cR * tau)|si = 0]
        tmp     = or2(coHi, coLo);
        std::cout << "s: " << s << std::endl;
    }
    std::cout << "lsb: " << lsb << std::endl;
    std::cout << "msb: " << msb << std::endl;
    // Determine last exet. quant. and return image
    coHi    = coFactorTrue(tmp, lsb);
    coLo    = coFactorFalse(tmp, lsb);
    BDD_ID img_sNext = or2(coHi, coLo);
    return img_sNext;
}
