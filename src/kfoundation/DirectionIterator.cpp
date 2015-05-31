//
//  DirectionIterator.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/2/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include "DirectionIterator.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  DirectionIterator::DirectionIterator(kf_int8_t nDims)
  : Direction(nDims)
  {
    first();
  }
  
  
// --- METHODS --- //
  
  void DirectionIterator::first() {
    _hasMore = true;
    for(int i = getSize() - 1; i >= 0; i--) {
      set(i, NEUTRAL);
    }
  }
  
  
  void DirectionIterator::next() {
    short int s = getSize();
    
    int i = 0;
    while (get(i) == FORTH && i < s) {
      set(i, NEUTRAL);
      i++;
    }
    
    if(i < s) {
      inc(i);
    } else {
      _hasMore = false;
    }
  }
  
  
} // namespace kfoundation