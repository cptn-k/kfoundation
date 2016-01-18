//
//  Bool.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Internal
#include "UString.h"
#include "Ref.h"
#include "OutputStream.h"

// Self
#include "Bool.h"

namespace kfoundation {

// --- STATIC FIELDS --- //

  const StaticRefConst<UString> Bool::TRUE_STR = new UString("true");
  const StaticRefConst<UString> Bool::FALSE_STR = new UString("false");


// --- CONSTRUCTORS --- //

  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Bool::Bool(bool value)
    : _value(value)
  {
    // Nothing
  }


// --- METHODS --- //
  
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


  void Bool::printToStream(Ref<OutputStream> stream) const {
    return toString()->printToStream(stream);
  }


  /**
   * Returns the string representation of the given argument.
   *
   * @param value The value to be converted to string.
   */

  RefConst<UString> Bool::toString() const {
    if(_value) {
      return TRUE_STR;
    } else {
      return FALSE_STR;
    }
  }

} // namespace kfoundation