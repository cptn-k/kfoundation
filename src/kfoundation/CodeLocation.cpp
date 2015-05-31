//
//  CodeLocation.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "ObjectSerializer.h"
#include "Ptr.h"

#include "CodeLocation.h"

namespace kfoundation {
  
  CodeLocation::CodeLocation()
  : _line(0),
    _col(0),
    _byteIndex(0),
    _charIndex(0)
  {
    // Nothing;
  }

  
  void CodeLocation::set(const CodeLocation& other) {
    _line = other._line;
    _col = other._col;
    _byteIndex = other._byteIndex,
    _charIndex = other._charIndex;
  }
  
  
  void CodeLocation::set(long int line, long int col, long int byteIndex)
  {
    _line = line;
    _col = col;
    _byteIndex = byteIndex,
    _charIndex = byteIndex;
  }
  
  
  void CodeLocation::set(long int line, long int col, long int charIndex, long int byteIndex)
  {
    _line = line;
    _col = col;
    _byteIndex = byteIndex,
    _charIndex = charIndex;
  }
  
  
  CodeLocation& CodeLocation::setLine(long int line) {
    _line = line;
    return *this;
  }
  
  
  CodeLocation& CodeLocation::setCol(long int col) {
    _col = col;
    return *this;
  }
  
  
  CodeLocation& CodeLocation::setByteIndex(long int byteIndex) {
    _byteIndex = byteIndex;
    return *this;
  }
  
  
  CodeLocation& CodeLocation::setCharIndex(long int charIndex) {
    _charIndex = charIndex;
    return *this;
  }
  
  
  void CodeLocation::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("CodeLocation")
      ->attribute("line", _line)
      ->attribute("col", _col)
      ->attribute("byteIndex", _byteIndex)
      ->attribute("charIndex", _charIndex)
      ->endObject();
  }
  
} // namespace kfoundation