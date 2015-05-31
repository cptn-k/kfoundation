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

  Bool::Bool(bool value)
    : _value(value)
  {
    // Nothing
  }

  inline bool Bool::get() const {
    return _value;
  }
  
  inline void Bool::set(const bool& value) {
    _value = value;
  }

  string Bool::toString(bool value) {
    if(value)
      return "true";
    return "false";
  }
  
  void Bool::printToStream(ostream& os) const {
    os << toString(_value);
  }


} // namespace kfoundation