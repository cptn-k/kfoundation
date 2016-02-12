//
//  Int64.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Std
#include <cstdio>

// Internal
#include "UString.h"
#include "OutputStream.h"
#include "StringInputStream.h"
#include "StreamParser.h"
#include "InputStream.h"

// Self
#include "Int64.h"

namespace kfoundation {

// --- CONSTRUCTORS --- //

  /**
   * Constructor. Assigns the internal value to the given paramaeter.
   */
  
  Int64::Int64(kf_int64_t value)
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

  Int64::Int64(RefConst<UString> str)
  : _value(parse(str))
  {
    // Nothing
  }


// --- STATIC METHODS --- //

  kf_int64_t Int64::parse(RefConst<UString> str) {
    StreamParser parser(new StringInputStream(str));
    kf_int64_t value;
    parser.readNumber(value);
    return value;
  }


// --- METHODS --- //

  /**
   * Getter method. Returns the internal value.
   */
  
  kf_int64_t Int64::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given argument.
   *
   * @param v The value to be set.
   */
  
  void Int64::set(const kf_int64_t v) {
    _value = v;
  }

  
  /**
   * Returns the hexadecimal representation of the given value as a string.
   *
   * @param v The value to convert to hexadecimal.
   * @return The hexadecimal representation of the given value.
   */

  Ref<UString> Int64::toHexString() const {
    char buffer[50];
    int n = sprintf(buffer, "%lX", _value);
    return new UString((kf_octet_t*)buffer, n);
  }


  /**
   * Converts the given integer value to string.
   */

  void Int64::printToStream(Ref<OutputStream> stream) const {
    char buffer[50];
    int n = sprintf(buffer, "%ld", _value);
    if(n > 0) {
      stream->write((kf_octet_t*)buffer, n);
    }
  }


  RefConst<UString> Int64::toString() const {
    char buffer[50];
    int n = sprintf(buffer, "%ld", _value);
    return new UString((kf_octet_t*)buffer, n);
  }

} // namespace kfoundation