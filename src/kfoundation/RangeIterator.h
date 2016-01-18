//
//  GridIterator.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 1/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__GridIterator__
#define __KFoundation__GridIterator__

// Internal
#include "Tuple.h"

// Super
#include "KFObject.h"

namespace kfoundation {
  
  class Range;
  
  
  /**
   * Used to iterate over all points in a given range. The begining of the 
   * range is inclusive and the end of it is exclusive.
   * Usage:
   *
   *     Range r(Tupel2D(10, 10), Tuple2D(20, 30));
   *     for(RangeIterator(r); r.hasMore(); r.next()) {
   *         Tuple point = r;
   *         ... do somethinf with r ...
   *     }
   *.
   *
   * @ingroup range
   * @headerfile RangeIterator.h <kfoundation/RangeIterator.h>
   */
   
  
  class RangeIterator : public Tuple {
    
  // --- FIELDS --- //
    
    private: Tuple _lowerBound;
    private: Tuple _upperBound;
    private: bool  _hasMore;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: RangeIterator(const Tuple& upperBound);
    public: RangeIterator(const Tuple& lowerBound, const Tuple& upperBound);
    public: RangeIterator(const Range& range);
    public: ~RangeIterator();
    
    
  // --- METHODS --- //
    
    public: const RangeIterator& first();
    public: const RangeIterator& next();
    public: inline bool hasMore() const;
    
  };
  
  
  /**
   * Checks if there are more points to iterator.
   */
  
  inline bool RangeIterator::hasMore() const {
    return _hasMore;
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__GridIterator__) */