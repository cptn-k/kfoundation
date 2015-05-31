//
//  GridIterator.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 1/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "Range.h"

// Self
#include "RangeIterator.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  RangeIterator::RangeIterator(const Tuple& upperBound)
  : Tuple(upperBound.getSize()),
    _lowerBound(upperBound.getSize()),
    _upperBound(upperBound),
    _hasMore(upperBound.productAll() > 0)
  {
    // Nothing;
  }
  
  
  RangeIterator::RangeIterator(const Tuple& lowerBound, const Tuple& upperBound)
  : Tuple(lowerBound),
    _lowerBound(lowerBound),
    _upperBound(upperBound),
    _hasMore((upperBound - lowerBound).productAll() > 0)
  {
    // Nothing;
  }
  
  
  RangeIterator::RangeIterator(const Range& range)
  : Tuple(range.getBegin()),
    _lowerBound(range.getBegin()),
    _upperBound(range.getEnd()),
    _hasMore(range.getVolume() > 0)
  {
    // Nothing;
  }
  
  
  RangeIterator::~RangeIterator() {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  const RangeIterator& RangeIterator::first() {
    Tuple::set(_lowerBound);
    _hasMore = ((_upperBound = _lowerBound).productAll() >= 0);
    return *this;
  }
  
  
  const RangeIterator& RangeIterator::next() {
    int i = 0;
    while (_elements[i] >= _upperBound.at(i) - 1 && i < getSize()) {
      _elements[i] = _lowerBound.at(i);
      i++;
    }
    
    if(i < getSize()) {
      _elements[i]++;
    } else {
      _hasMore = false;
    }
    
    return *this;
  }
  
} // kfoundation