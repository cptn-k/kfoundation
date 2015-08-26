//
//  Double.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cstdlib>

#include "Double.h"

namespace kfoundation {

  using namespace std;
  
  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Double::Double(const double value)
    : _value(value)
  {
    // Nothing
  }
  
  
  /**
   * Getter method. Returns the internal value.
   */
  
  inline double Double::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given parameter.
   */
  
  inline void Double::set(const double value) {
    _value = value;
  }
  
  
  /**
   * Parses the given string to the corresponding `double` value.
   *
   * @param str The string to be parsed.
   * @return The numerical value parsed from the given string.
   */
  
  double Double::parse(const string& str) {
    return atof(str.c_str());
  }

  void Double::printToStream(ostream& os) const {
    os << _value;
  }

} // namespace kfoundation