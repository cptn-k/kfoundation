//
//  LongInt.cpp
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
#include "LongInt.h"

namespace kfoundation {

// --- CONSTRUCTORS --- //

  /**
   * Constructor. Assigns the internal value to the given paramaeter.
   */
  
  LongInt::LongInt(kf_int64_t value)
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

  LongInt::LongInt(RefConst<UString> str)
  : _value(parse(str))
  {
    // Nothing
  }


// --- STATIC METHODS --- //

  kf_int64_t LongInt::parse(RefConst<UString> str) {
    StreamParser parser(new StringInputStream(str));
    kf_int64_t value;
    parser.readNumber(value);
    return value;
  }


// --- METHODS --- //

  /**
   * Getter method. Returns the internal value.
   */
  
  kf_int64_t LongInt::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given argument.
   *
   * @param v The value to be set.
   */
  
  void LongInt::set(const kf_int64_t v) {
    _value = v;
  }

  
  /**
   * Returns the hexadecimal representation of the given value as a string.
   *
   * @param v The value to convert to hexadecimal.
   * @return The hexadecimal representation of the given value.
   */

  Ref<UString> LongInt::toHexString() const {
    char buffer[50];
    int n = sprintf(buffer, "%lX", _value);
    return new UString((kf_octet_t*)buffer, n);
  }


  /**
   * Converts the given integer value to string.
   */

  void LongInt::printToStream(Ref<OutputStream> stream) const {
    char buffer[50];
    int n = sprintf(buffer, "%ld", _value);
    if(n > 0) {
      stream->write((kf_octet_t*)buffer, n);
    }
  }


  RefConst<UString> LongInt::toString() const {
    char buffer[50];
    int n = sprintf(buffer, "%ld", _value);
    return new UString((kf_octet_t*)buffer, n);
  }

} // namespace kfoundation