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
#include "ManagedObject.h"

namespace kfoundation {
  
  class Range;
  
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
  
  
  inline bool RangeIterator::hasMore() const {
    return _hasMore;
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__GridIterator__) */
