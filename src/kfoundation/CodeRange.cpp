//
//  CodeRange.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 4/7/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include "Ptr.h"
#include "ObjectSerializer.h"
#include "CodeRange.h"

namespace kfoundation {
  
  
  /**
   * Constructor.
   *
   * @param begin The begining of the range (inclusive).
   * @param end The end of the range (inclusive).
   */
  
  CodeRange::CodeRange(const CodeLocation& begin, const CodeLocation& end)
  : _begin(begin),
    _end(end)
  {
    // Nothing
  }
  
  
  /**
   * Returns the begining of the range.
   */
  
  const CodeLocation& CodeRange::getBegin() const {
    return _begin;
  }
  
  
  /**
   * Returns the end of the range.
   */
  
  const CodeLocation& CodeRange::getEnd() const {
    return _end;
  }
  
  
  void CodeRange::serialize(PPtr<ObjectSerializer> serializer) const {
    serializer->object("CodeRange")
      ->member("begin")->object(_begin)
      ->member("end")->object(_end)
      ->endObject();
  }
  
} // namespace kfoundation