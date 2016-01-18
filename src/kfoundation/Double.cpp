//
//  Double.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Std
#include <cstdio>

// Internal
#include "Ref.h"
#include "StreamParser.h"
#include "StringInputStream.h"
#include "UString.h"
#include "OutputStream.h"

// Self
#include "Double.h"

namespace kfoundation {

// --- CONSTRUCTOR --- //

  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Double::Double(const double value)
  : _value(value)
  {
    // Nothing
  }


  Double::Double(RefConst<UString> str)
  : _value(parse(str))
  {
    // Nothing;
  }


// --- STATIC METHODS --- //

  double Double::parse(RefConst<UString> str) {
    StreamParser parser(new StringInputStream(str));
    double v;
    parser.readNumber(v);
    return v;
  }


// --- METHODS --- //
  
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
  
  
  void Double::printToStream(Ref<OutputStream> stream) const {
    char buffer[50];
    int n = sprintf(buffer, "%f", _value);
    if(n > 0) {
      stream->write((kf_octet_t*)buffer, n);
    }
  }


  RefConst<UString> Double::toString() const {
    char buffer[50];
    int n = sprintf(buffer, "%f", _value);
    return new UString((kf_octet_t*)buffer, n);
  }

} // namespace kfoundation