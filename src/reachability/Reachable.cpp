//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
/****** PUBLIC ******/
//! @return returns the XNOR of BDD IDs
BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b){
    BDD_ID negB =this->neg(b);

    return this->ite(a,b,negB);
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
 * Each state variable has a transition function.
 * The transition function specifies the value of the state after the transition.
 * The transition function are composed of only state variables.
 * For example: s0' = s0 XOR s1
 * The next state is defined as XOR of the current values of the state bit s0 and s1
 * @param transitionFunctions provide a transition function exactly for each state bit
 */
void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions)
{
    delta = transitionFunctions;
    return;
}
/**
 * Each state machine has an inital state. The inital state is provided as a vector.
 * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
 * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
 * @param stateVector provide the assignemtn for each state bit
 */
void Reachable::setInitState(const std::vector<bool>& stateVector)
{
    stateBits = stateVector;
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
    *       Reachable (R_it) = S0;   -> Initial state
    *       do
    *       {
    *           R = R_it;
    *           R_it = R set of elements in compute_reachable_states(t, R)
    *       } while (R_it == R);
    *       return R;
    */
}
/**
 * This method decides whether a specific state is in the reachable state space or not.
 * The inpute is provided as a vector of states. The value is true if the state bit is high in this state, false otherwise.
 * @param stateVector 
 * @return
 */
bool Reachable::is_reachable(const std::vector<bool>& stateVector)
{

}
/****** PRIVATE ******/
