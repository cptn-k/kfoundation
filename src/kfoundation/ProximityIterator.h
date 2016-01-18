//
//  ProximityIterator.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/6/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__ProximityIterator__
#define __KFoundation__ProximityIterator__

// Internal
#include "Range.h"

// Super
#include "Tuple.h"

namespace kfoundation {
  
  /**
   * Iterates the proximity of a desired point. Particularly useful for stencil
   * computation.
   *
   * @ingroup range
   * @headerfile ProximityIterator.h <kfoundation/ProximityIterator.h>
   */
  
  class ProximityIterator : public Tuple {
    
  // --- FIELDS --- //
    
    private: int   _radius;
    private: Tuple _begin;
    private: Tuple _end;
    private: Tuple _center;
    private: bool  _hasMore;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ProximityIterator(int radius);
    public: ProximityIterator(int radius, const Tuple& center);
    
    
  // --- METHODS --- //
    
    public: ProximityIterator& centerAt(const Tuple& center);
    public: ProximityIterator& next();
    public: inline bool hasMore();
    
  };
  
  
  /**
   * Checks if there are more points to iterate.
   */
  
  inline bool ProximityIterator::hasMore() {
    return _hasMore;
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__ProximityIterator__) */