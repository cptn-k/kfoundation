//
//  LongInt.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cstdlib>
#include <cstdio>

#include "LongInt.h"

namespace kfoundation {

  using namespace std;
  
  LongInt::LongInt(long int value)
    : _value(value)
  {
    // Nothing;
  }
  
  
  LongInt::LongInt(const string& str, const encoding_t& encoding)
    : _value(parse(str, encoding))
  {
    // Nothingl
  }
  
  
  inline long int LongInt::get() const {
    return _value;
  }
  
  
  inline void LongInt::set(const long int& v) {
    _value = v;
  }
  
  
  long int LongInt::parse(const string& str, const encoding_t encoding) {
    if(encoding == DECIMAL) {
      return atol(str.c_str());
    }
    return strtol(str.c_str(), NULL, 0);
  }
  
  
  string LongInt::toHexString(const long int v) {
    char buffer[16];
    sprintf(buffer, "%lx", v);
    return string(buffer);
  }
  
  
  string LongInt::toHexString(void* ptr) {
    char buffer[16];
    sprintf(buffer, "%p", ptr);
    return string(buffer);
  }
  
  
  string LongInt::toString(const long int v) {
    char buffer[16];
    sprintf(buffer, "%ld", v);
    return string(buffer);
  }
  
  
  void LongInt::printToStream(ostream &os) const {
    os << _value;
  }
  
} // namespace kfoundation
