//
// Created by ludwig on 27.11.18.
//
#include "Reachable.h"
namespace ClassProject {
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
        BDD_ID formedImg        = MANAGER_FAIL;
        BDD_ID img_sNext        = MANAGER_FAIL;
        uint timeout            = 1000;
        std::cout << "Start cR_it: " << cR_it << std::endl;
        do
        {
            cR = cR_it;
            // Now compute the image
            img_sNext = computeImage(cR, tau);
            std::cout << "img_sNext: " << img_sNext << std::endl;
            // Form the image of s by renaming s'i to si
            formedImg = formImage(img_sNext);
            std::cout << "formedImg: " << formedImg << std::endl;
            // Update the char.function
            cR_it = or2(cR, formedImg);
            std::cout << "Updated cR_it: " << cR_it << std::endl;
            timeout--;
        } while((cR != cR_it) && (timeout != 0));
        if(timeout == 0)
        {
            std::cout << "FAIL: Timeout in do/while loop" << std::endl;
            return MANAGER_FAIL;
        }
        std::cout << "cR: " << cR << std::endl;
        return cR;
    }
    /**
     * This method decides whether a specific state is in the reachable state space or not.
     * The inpute is provided as a vector of states. The value is true if the state bit is high in this state, false otherwise.
     * @param stateVector 
     * @return
     */
    bool Reachable::is_reachable(const std::vector<bool>& stateVector)
    {
        if (cR == this->MANAGER_FAIL)
        {
            compute_reachable_states();
        }

        BDD_ID tmp;
        BDD_ID c_test = xnor2(s0, stateVector.at(0));
        for(uint i = 1; i < state_var; i++)
        {
            tmp     = xnor2(s0 + i,(int) stateVector.at(i));
            c_test    = and2(c_test, tmp);
        }

        //std::cout<<"char func teststate "<<c_test << std::endl;
        //state c_test included in c_R
        BDD_ID tmp2 = and2(cR,c_test);
        //std::cout<<"contain states "<<tmp2 << std::endl;
        //printTables();

        bool res = (c_test == tmp2);

        return res;
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
        for(uint i = 0; i < states.size(); i++)
        {
            if(i < state_var)
            {
                std::cout << "BDD_ID[" << states.at(i) << "]::s" << i << std::endl;
            }
            else
            {
                std::cout << "BDD_ID[" << states.at(i) << "]::s'" << i-state_var << std::endl;
            }
        }
        // delta vector
        std::cout << " --- DELTA VECTOR --- " << std::endl;
        for(auto it = transitions->begin(); it != transitions->end(); ++it)
        {
            std::cout << *(uniqueTable.at((*it))) << std::endl;
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
        BDD_ID tau          = MANAGER_FAIL;
        BDD_ID tempTau      = MANAGER_FAIL;
        BDD_ID firstTerm    = MANAGER_FAIL;
        BDD_ID secndTerm    = MANAGER_FAIL;
        
        // Start with Products
        // s'0 (located at the end of all non-next state variables
        // (s'0 * transitions(s0)) 
        firstTerm   = and2(s0_next, transitions->at(0));
        // (~s'0 * ~transitions(s0))
        secndTerm   = and2(neg(s0_next), neg(transitions->at(0)));
        // Sum
        tau         = or2(firstTerm, secndTerm);
        for(uint i = 1; i < state_var; i++)
        {
            // POS from i = 1 to states_var:
            // Products, (s'i * transitions(si)) 
            firstTerm   = and2(s0_next + i, transitions->at(i));
            secndTerm   = and2(neg(s0_next + i), neg(transitions->at(i)));
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
    *   Computes the characteristic function by, c_s0 = and2(s0 == initialStates(0), ..., s(n-1) == initialStates(n-1))
    *   Assumes: 
    *       Application provides initial state bits for given number of states.
    * @return BDD_ID of characteristic function
    */
    BDD_ID Reachable::computeCharFunction(void)
    {
        // Compute characteristic function of initial state s0
        BDD_ID tmp  = MANAGER_FAIL;
        BDD_ID c_s0 = xnor2(s0, initialStates->at(0));
        for(uint i = 1; i < state_var; i++)
        {
            tmp     = xnor2(s0 + i, initialStates->at(i));
            c_s0    = and2(c_s0, tmp);
        }
        std::cout << "Char.Function s0: BDD_ID[" << c_s0 << "]" << std::endl;
        return c_s0;
    }
    /**
    * computeImage
    * @brief 
    *   Computes the image for si by, E_s0 ... E_s(n-1)[cR * tau]
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
        // E_s0 ... E_s(n-1)
        for(uint i = si; i > s0; i--)
        {
            // Find exenstential qunatification in realation to si for (cR * tau), done through cofactoring
            coHi    = coFactorTrue(tmp, i);  // Starts from the last state
            coLo    = coFactorFalse(tmp, i);
            // [(cR * tau)|si = 1] + [(cR * tau)|si = 0]
            tmp     = or2(coHi, coLo);
        }
        // Determine last exet. quant. and return image
        coHi    = coFactorTrue(tmp, s0);
        coLo    = coFactorFalse(tmp, s0);
        BDD_ID img_sNext = or2(coHi, coLo);
        return img_sNext;
    }
    /**
    * formImage
    * @brief 
    *   The successor state becomes the current state. In the characteristic function, this corresponds to a substitution of s' by s
    *   Done by, E_s'0 ... E_s'(n-1) [ (s0 == s'0) * ... * (s(n-1) == s'(n-1)) * img(s'0 ... s'(n-1)) ]
    *   Assumes: 
    *       Application provides initial state bits for given number of states.
    *
    * @param imgNext ID for BDD of successor state image
    * @return BDD_ID of formed image
    */
    BDD_ID Reachable::formImage(BDD_ID imgNext)
    {
        // and characeristic function with transition relation
        BDD_ID tmp       = xnor2(s0, s0_next);
        BDD_ID xnorTmp   = MANAGER_FAIL;
        BDD_ID formedImg = MANAGER_FAIL;
        BDD_ID orTmp1    = MANAGER_FAIL;
        BDD_ID orTmp2    = MANAGER_FAIL;
        // Iterate until we xnor all states
        for(uint i = 1; i < state_var; i++)
        {
            // (si == s'i)
            xnorTmp = xnor2(s0 + i,  s0_next + i);
            // And all xnor values
            tmp     = and2(tmp, xnorTmp);
        }

        // Now and with imgNext
        tmp = and2(tmp, imgNext);
        
        // Iterate until we cofactor all states. This finds exenstentail qunatification
        BDD_ID coHi = MANAGER_FAIL;
        BDD_ID coLo = MANAGER_FAIL;
        // E_s'0 ... E_s'(n-1)
        for(uint i = si_next; i > s0_next; i--)
        {
            // Find exenstential qunatification, done through cofactoring
            coHi    = coFactorTrue(tmp, i);  // Starts from the last state
            coLo    = coFactorFalse(tmp, i);
            // [((si == s'i) * imgNext)|s'i = 1] + [((si == s'i) * imgNext)|s'i = 0]
            tmp     = or2(coHi, coLo);
        }
        // Determine last exet. quant. and return formed image
        coHi        = coFactorTrue(tmp, s0_next);
        coLo        = coFactorFalse(tmp, s0_next);
        formedImg   = or2(coHi, coLo);
        return formedImg;
    }
} // namespace ClassProject
