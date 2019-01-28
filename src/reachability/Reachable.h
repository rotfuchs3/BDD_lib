//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include "ReachableInterface.h"


//namespace ClassProject {
class Reachable : public ClassProject::ReachableInterface {
    public:
        /**
        * Constructor creates stateSize state bits for the user
        * @param stateSize state size
        */
        Reachable(unsigned int state_variable) 
        : ReachableInterface(state_variable), state_var(state_variable)
        {
            // Check if state variable is greater than 0
            if (state_variable <= 0) {
                throw std::invalid_argument("In Constructor: number states <= 0\n");
            }
            /* INITIALIZE STATES */
            // Increase the vector size by given state input
            states.reserve(state_var*2);
            std::cout << "states.capacity: " << states.capacity() << std::endl;
            // create variables for current and successor states
            for (int i = 0; i < state_var; ++i) 
            {
                states.push_back(createVar("s" + std::to_string(i)));
                std::cout << states.at(i) << std::endl;
            }
            // Compute delta by negation of state variables
            std::cout << "states.size: " << states.size() << std::endl;
        }
        /// Destructor
        ~Reachable() {}
        //! @return returns the XNOR of BDD IDs
        BDD_ID xnor2(BDD_ID a, BDD_ID b);
        /**
         * States are generated and stored in a vector.
         * These lsb (e.g. "s0")  is stored at location 0.
         * The msb(e.g. "s3") is stored at location size-1.
         * @return vector with the BDD_ID of each state bit
         */
        const std::vector<BDD_ID> &getStates() const;
        /**
         * Each state variable has a transition function.
         * The transition function specifies the value of the state after the transition.
         * The transition function are composed of only state variables.
         * For example: s0' = s0 XOR s1
         * The next state is2 defined as XOR of the current values of the state bit s0 and s1
         * @param transitionFunctions provide a transition function exactly for each state bit
         */
        void setDelta(const std::vector<BDD_ID> &transitionFunctions);
        /**
         * Each state machine has an inital state. The inital state is provided as a vector.
         * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
         * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
         * @param stateVector provide the assignemtn for each state bit
         */
        void setInitState(const std::vector<bool>& stateVector);
        /**
         * Computes the symbolic representation of the reachable states.
         * Before this method is called it is important to set the transition function and the initial state.
         * @return BDD_ID of the reachable state space
         */
        BDD_ID compute_reachable_states();
        /**
         * This method decides whether a specific state is in the reachable state space or not.
         * The inpute is provided as a vector of states. The value is true if the state bit is high in this state, false otherwise.
         * @param stateVector 
         * @return
         */
        bool is_reachable(const std::vector<bool>& stateVector);
    private:
        unsigned int state_var;
        /// Vector holding all states
        std::vector<BDD_ID> states;
        /// Vector holding the transition functions
        std::vector<BDD_ID> delta;
        /// Vector holding all state bits
        std::vector<bool> stateBits;
};


//}

#endif //VDSPROJECT_IMGCOMP_H
