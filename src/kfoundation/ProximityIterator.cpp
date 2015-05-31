//
//  ProximityIterator.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/6/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include "KFException.h"
#include "Int.h"

#include "ProximityIterator.h"

namespace kfoundation {

// --- (DE)CONSTRUCTORS --- //
  
  ProximityIterator::ProximityIterator(int radius)
  : _radius(radius)
  {
    if(radius <= 0) {
      throw KFException("Invalid radius: " + Int::toString(radius));
    }
    _hasMore = false;
  }
  
  
  ProximityIterator::ProximityIterator(int radius, const Tuple& center)
  : _radius(radius)
  {
    if(radius <= 0) {
      throw KFException("Invalid radius: " + Int::toString(radius));
    }
    centerAt(center);
  }
  

// --- METHODS --- //
  
  ProximityIterator& ProximityIterator::centerAt(const Tuple &center) {
    _center = center;
    _begin = center - _radius;
    _end = center + _radius;
    _hasMore = true;
    set(_begin);
    return *this;
  }
  
  
  ProximityIterator& ProximityIterator::next() {
    int i = 0;
    while (_elements[i] == _end.at(i) && i < getSize()) {
      _elements[i] = _begin.at(i);
      i++;
    }
    
    if(i < getSize()) {
      _elements[i]++;
    } else {
      _hasMore = false;
    }
        
    return *this;
  }

} // namespace kfoundation
