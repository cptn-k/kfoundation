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
  
  
  /**
   * Default constructor, sets all values to zero.
   */
  
  CodeLocation::CodeLocation()
  : _line(0),
    _col(0),
    _byteIndex(0),
    _charIndex(0)
  {
    // Nothing;
  }

  
  /**
   * Setter, copies all the values from the given argument.
   */
  
  void CodeLocation::set(const CodeLocation& other) {
    _line = other._line;
    _col = other._col;
    _byteIndex = other._byteIndex,
    _charIndex = other._charIndex;
  }
  
  
  /**
   * Setter, sets all values except charIndex which will be set equal to 
   * byteIndex.
   *
   * @param line Line number
   * @param col Column number
   * @param byteIndex Byte index from the begining of the file.
   */
   
  
  void CodeLocation::set(long int line, long int col, long int byteIndex)
  {
    _line = line;
    _col = col;
    _byteIndex = byteIndex,
    _charIndex = byteIndex;
  }
  
  
  /**
   * Setter, sets all values as given in arguments.
   *
   * @param line Line number
   * @param col Column number
   * @param charIndex Character index from the begining of the file.
   * @param byteIndex Byte index from the begining of the file.
   */
  
  void CodeLocation::set(long int line, long int col, long int charIndex, long int byteIndex)
  {
    _line = line;
    _col = col;
    _byteIndex = byteIndex,
    _charIndex = charIndex;
  }
  
  
  /**
   * Setter, sets the line number.
   *
   * @param line Line number
   * @return Reference to slef.
   */
  
  CodeLocation& CodeLocation::setLine(long int line) {
    _line = line;
    return *this;
  }
  
  
  /**
   * Setter, sets the column number.
   *
   * @param col Column number
   * @return Reference to self.
   */
  
  CodeLocation& CodeLocation::setCol(long int col) {
    _col = col;
    return *this;
  }
  
  
  /**
   * Setter, sets byte index.
   *
   * @param byteIndex Byte index from the begining of the file.
   * @param Reference to self.
   */
  
  CodeLocation& CodeLocation::setByteIndex(long int byteIndex) {
    _byteIndex = byteIndex;
    return *this;
  }
  
  
  /**
   * Setter, sets character index.
   *
   * @param charIndex Character index from the begining of the file.
   * @return Reference to self.
   */
  
  CodeLocation& CodeLocation::setCharIndex(long int charIndex) {
    _charIndex = charIndex;
    return *this;
  }
  
  
  void CodeLocation::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("CodeLocation")
      ->attribute("line", _line)
      ->attribute("col", _col)
      ->endObject();
  }
  
} // namespace kfoundation