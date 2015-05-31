//
//  RangeMap.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include "RangeMap.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  RangeMap::RangeMap() {
    // Nothing;
  }
  
  
  RangeMap::RangeMap(const Range& local)
  : _local(local),
    _offset(Tuple::zero(local.getNDimensions()))
  {
    // Nothing;
  }
  
  
  RangeMap::RangeMap(const Range& local, const Tuple& offset)
  : _local(local),
    _offset(offset)
  {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  const Range& RangeMap::getLocalRange() const {
    return _local;
  }
  
  
  const Tuple& RangeMap::getGlobalOffset() const {
    return _offset;
  }
  
  
  Tuple RangeMap::mapLocalToGlobal(const Tuple& t) {
    return t + _offset;
  }
  
  
  Tuple RangeMap::mapGlobalToLocal(const Tuple& t) {
    return t - _offset;
  }
  
} // namespace kfoundation