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
  
  /**
   * Constructor, sets the rectangular radius around the point to be iterated.
   * The point which its proximity is being iterated can be set later using 
   * centerAt() method.
   */
  
  ProximityIterator::ProximityIterator(int radius)
  : _radius(radius)
  {
    if(radius <= 0) {
      throw KFException("Invalid radius: " + Int::toString(radius));
    }
    _hasMore = false;
  }
  
  
  /**
   * Constructor, sets the point and the rectangular radius around it to be
   * iterated.
   * The point at center can later be changes using centerAt() method.
   */
  
  ProximityIterator::ProximityIterator(int radius, const Tuple& center)
  : _radius(radius)
  {
    if(radius <= 0) {
      throw KFException("Invalid radius: " + Int::toString(radius));
    }
    centerAt(center);
  }
  

// --- METHODS --- //
  
  /**
   * Resets this iterator, centering the iteration region at the given point.
   */
  
  ProximityIterator& ProximityIterator::centerAt(const Tuple &center) {
    _center = center;
    _begin = center - _radius;
    _end = center + _radius;
    _hasMore = true;
    set(_begin);
    return *this;
  }
  
  
  /**
   * Moves on to the next point.
   * @return Reference to self.
   */
  
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
