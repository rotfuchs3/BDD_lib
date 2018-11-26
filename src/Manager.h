include "ManagerInterface.h"

//!  Manager class
/*!
  This class is derived from MamagerInterface, it implements all virtual functions and is the main class used by application.
*/
class Manager : ManagerInterface {
public:
  //! True
  /*!
    Returns the ID of the node representing True
  */
  const BDD_ID &True(void);
  //! False
  /*!
    Returns the ID of the node representing False
  */
  const BDD_ID &False(void);
  //! isConstant
  /*!
    Returns true if x is a leaf node
  */
  bool isConstant(const BDD_ID x);
  //! isVariable
  /*!
    Returns true if x is a variable
  */
  bool isVariable(const BDD_ID x);
}
private:
// PLACE HOLDER
}
