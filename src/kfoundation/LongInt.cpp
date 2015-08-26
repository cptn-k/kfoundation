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
  
  /**
   * Constructor. Assigns the internal value to the given paramaeter.
   */
  
  LongInt::LongInt(long int value)
    : _value(value)
  {
    // Nothing;
  }
  
  
  /**
   * Constructor. Parses the given string according to the given encoding and
   * sets the internal value accordingly.
   *
   * @param str The string to parse.
   * @param encoding The encoding of the given string. Default value is DECIMAL.
   */

  LongInt::LongInt(const string& str, const encoding_t& encoding)
    : _value(parse(str, encoding))
  {
    // Nothing
  }
  
  
  /**
   * Getter method. Returns the internal value.
   */
  
  inline long int LongInt::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given argument.
   *
   * @param v The value to be set.
   */
  
  inline void LongInt::set(const long int v) {
    _value = v;
  }

  
  /**
   * Parses the given string according the given encoding and returns the
   * extracted value. 
   *
   * @param str The string to parse
   * @param encoding The encoding of the given string. Default value is DECIMAL.
   */
  
  long int LongInt::parse(const string& str, const encoding_t encoding) {
    if(encoding == DECIMAL) {
      return atol(str.c_str());
    }
    return strtol(str.c_str(), NULL, 0);
  }
  
  
  /**
   * Converts the given value to hexadecimal string representation.
   */
  
  string LongInt::toHexString(const long int v) {
    char buffer[16];
    sprintf(buffer, "%lx", v);
    return string(buffer);
  }
  
  
  /**
   * Converts the value of the given pointer (not the value it is pointing at)
   * to hexadecimal string.
   */
  
  string LongInt::toHexString(void* ptr) {
    char buffer[16];
    sprintf(buffer, "%p", ptr);
    return string(buffer);
  }
  
  
  /**
   * Converts the given value to string.
   */
  
  string LongInt::toString(const long int v) {
    char buffer[16];
    sprintf(buffer, "%ld", v);
    return string(buffer);
  }
  
  
  void LongInt::printToStream(ostream &os) const {
    os << _value;
  }
  
} // namespace kfoundation
