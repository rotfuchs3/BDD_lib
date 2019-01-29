//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H
/// STL
#include <vector>
#include <math.h>
/// Header
#include "ReachableInterface.h"
/// namespace
namespace ClassProject {
    class Reachable : public ReachableInterface {
    private:
        unsigned int state_var;
        /// Vector holding all states
        std::vector<BDD_ID> states;
        /// Vector holding the transition functions
        const std::vector<BDD_ID> *delta;
        /// Vector holding all state bits
        const std::vector<bool> *stateBits;
        /**
        * computeTransitionRelation
        * @brief 
        *   Computes the translation relation by, c_si = (xnor(si, 0)) * (xnor(si, 0))
        */
        void computeTransitionRelation(void);
        /**
        * computeCharFunction
        * @brief 
        *   Computes the characteristic function by, c_s0 = and2(s0 == 0, s1 == 1)
        */
        void computeCharFunction(void);
    public:
        /// lsb state should always be s0 location
        BDD_ID lsb;
        /// msb state should always be size-1 of states
        BDD_ID msb;
        /**
        * Constructor creates stateSize state bits for the user
        * @param stateSize state size
        */
        Reachable(unsigned int state_variable) 
        : ReachableInterface(state_variable), state_var(state_variable)
        {
            // Check if state variable is greater than 0
            if (state_variable <= 0) 
            {
                throw std::invalid_argument("In Constructor: number states <= 0\n");
            }
            // Increase the vector size by given state input, memory for all states are then 2^state_var
            states.reserve(pow(2, state_var));
            // create variables for current states
            std::string label;
            for (uint i = 0; i < state_var; ++i) 
            {
                label = "s" + std::to_string(i);
                states.push_back(createVar(label));
            }
            // create variables for next states states
            for (uint i = 0; i < state_var; ++i) 
            {
                label = "s'" + std::to_string(i);
                states.push_back(createVar(label));
            }
            // Set lsb/msb
            lsb = states.at(0);
            msb = states.at(state_var-1);
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
        /**
        * printVectors
        *
        * @brief Prints all vectors
        */
        void printVectors(void); 
    }; // class Reachable
} // namespace bdd

#endif //VDSPROJECT_IMGCOMP_H
