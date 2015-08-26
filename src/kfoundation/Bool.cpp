//
//  Bool.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "Bool.h"

using namespace std;

namespace kfoundation {

  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Bool::Bool(bool value)
    : _value(value)
  {
    // Nothing
  }
  
  
  /**
   * Getter method. Returns the internal value.
   */

  inline bool Bool::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given parameter.
   */
  
  inline void Bool::set(const bool value) {
    _value = value;
  }
  
  
  /**
   * Returns the string representation of the given argument.
   *
   * @param value The value to be converted to string.
   */

  string Bool::toString(const bool value) {
    if(value)
      return "true";
    return "false";
  }
  
  void Bool::printToStream(ostream& os) const {
    os << toString(_value);
  }


} // namespace kfoundation