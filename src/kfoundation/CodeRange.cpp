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
  
  CodeRange::CodeRange(const CodeLocation& begin, const CodeLocation& end)
  : _begin(begin),
    _end(end)
  {
    // Nothing
  }
  
  
  const CodeLocation& CodeRange::getBegin() const {
    return _begin;
  }
  
  
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