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
  
  Double::Double(double value)
    : _value(value)
  {
    // Nothing
  }
  
  inline double Double::get() const {
    return _value;
  }
  
  inline void Double::set(double value) {
    _value = value;
  }
  
  double Double::parse(const string& str) {
    return atof(str.c_str());
  }

  void Double::printToStream(ostream& os) const {
    os << _value;
  }

} // namespace kfoundation