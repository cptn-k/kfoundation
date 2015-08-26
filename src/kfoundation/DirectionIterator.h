//
//  DirectionIterator.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/2/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__DirectionIterator__
#define __KFoundation__DirectionIterator__

// Super
#include "Direction.h"


namespace kfoundation {
  
  /**
   * Used to iterate all possible directions in an space of given dimensions.
   * Usage:
   *
   *     for(DirectionIterator di(3); di.hasMore(); di.next()) {
   *         Direction d = di;
   *         ... do something with d ...
   *     }
   *.
   *
   * @ingroup range
   * @headerfile DirectionIterator.h <kfoundation/DirectionIterator.h>
   */
  
  class DirectionIterator : public Direction {
    
  // --- FIELDS --- //
    
    private: bool _hasMore;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: DirectionIterator(kf_int8_t nDims);
    
    
  // --- METHODS --- //
    
    public: void first();
    public: void next();
    public: inline bool hasMore() const;
    
  };
  
  
  /**
   * Checks if there is more items to iterate.
   */
  
  inline bool DirectionIterator::hasMore() const {
    return _hasMore;
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__DirectionIterator__) */
