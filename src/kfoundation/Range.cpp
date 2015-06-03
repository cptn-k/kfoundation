//
//  Range.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "KFException.h"
#include "Int.h"
#include "RangeIterator.h"

// Self
#include "Range.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  Range::Range() {
    // Nothing;
  }
  
  
  Range::Range(kf_int8_t size)
  : _begin(size),
    _end(size),
    _size(size)
  {
    // Nothing
  }
  
  
  Range::Range(const Tuple& begin, const Tuple& end)
  : _begin(begin),
    _end(end),
    _size(end - begin)
  {
    if(begin.getSize() != end.getSize()) {
      throw KFException("Supplied argument do not have the same size: "
        + begin + ", " + end);
    }
  }
  
  
  Range::Range(const Tuple& end)
  : _begin(end.getSize()),
    _end(end),
    _size(end)
  {
    // Nothing;
  }
  
  
  Range::Range(const Range& other)
  : _begin(other._begin),
    _end(other._end),
    _size(other._size)
  {
    // Nothing
  }
  
  
// --- METHODS --- //
  
  kf_int8_t Range::getNDimensions() const {
    return _begin.getSize();
  }
  
  
  const Tuple& Range::getBegin() const {
    return _begin;
  }
  
  
  const Tuple& Range::getEnd() const {
    return _end;
  }
  
  
  kf_int64_t Range::indexToOrdinal(const Tuple& index) const {
    kf_octet_t s = _begin.getSize();
    kf_int64_t ordinal = 0;
    kf_int64_t p = 1;
    
    for(int i = 0; i < s; i++) {
      ordinal += (index.at(i) - _begin.at(i)) * p;
      p = p * _size.at(i);
    }
    
    return ordinal;
  }
  
  
  Range Range::translate(const Tuple &amount) const {
    return Range(_begin + amount, _end + amount);
  }
  
  
  Range Range::grow(const int s) const {
    return Range(_begin - s, _end + s);
  }
  
  
  Range Range::shrink(const int s) const {
    Range r(_begin + s, _end - s);
    for(kf_int8_t i = _begin.getSize() - 1; i >= 0; i--) {
      if(_end.at(i) < _begin.at(i)) {
        r._begin.at(i) = _begin.at(i);
        r._end.at(i) = _begin.at(i);
      }
    }
    
    return r;
  }
  
  
  Range Range::border(const Direction& d, const int s) const {
    if(d.getSize() != _begin.getSize()) {
      throw KFException("Size mismatch. Expected an Direction of size "
          + Int::toString(_begin.getSize()) + ", Provided: "
          + Int::toString(d.getSize()));
    }
    
    Range result(_begin.getSize());
    
    for(kf_int8_t i = _begin.getSize() - 1; i >= 0; i--) {
      switch (d.get(i)) {
        case Direction::BACK:
          result._begin.at(i) = _begin.at(i);
          result._end.at(i) = min(_begin.at(i) + s, _end.at(i));
          break;
          
        case Direction::FORTH:
          result._begin.at(i) = max(_begin.at(i), _end.at(i) - s);
          result._end.at(i) = _end.at(i);
          break;
          
        case Direction::NEUTRAL:
          result._begin.at(i) = _begin.at(i);
          result._end.at(i) = _end.at(i);
          break;
      }
    }
    
    return result;
  }
  
  
  Range Range::flip(const Direction& d) const {
    Range result(_begin.getSize());
    
    for(kf_int8_t i = _begin.getSize() - 1; i >= 0; i--) {
      switch (d.get(i)) {
        case Direction::BACK:
          result._begin.at(i) = _begin.at(i)*2 - _end.at(i);
          result._end.at(i) = _begin.at(i);
          break;
          
        case Direction::FORTH:
          result._begin.at(i) = _end.at(i);
          result._end.at(i) = _end.at(i)*2 - _begin.at(i);
          break;
          
        case Direction::NEUTRAL:
          result._begin.at(i) = _begin.at(i);
          result._end.at(i) = _end.at(i);
          break;
      }
    }
    
    return result;
  }
  
  
  Range Range::intersectWith(const Range& other) const {
    return Range(_begin.max(other._begin), _end.min(other._end));
  }
  
  
  Range Range::joinWith(const Range &other) const {
    return Range(_begin.min(other._begin), _end.max(other._end));
  }
  
  
  Range Range::joinWith(const Tuple& point) const {
    return Range(_begin.min(point), _end.max(point));
  }
  
  
  Range Range::divide(const Tuple& divisor, const Tuple& selector) const {
    Tuple mSize = _size / divisor;
    Tuple mBegin = _begin + mSize * selector;
    Tuple mEnd = mBegin + mSize;
    return Range(mBegin, mEnd);
  }
  
  
  bool Range::isAdjecentTo(const Range &other) const {
    return intersectWith(other).isEmpty()
        && !intersectWith(other.grow(1)).isEmpty();
  }
  
  
  bool Range::contains(const Tuple& point) const {
    for(kf_int8_t i = _begin.getSize() - 1; i >=0; i--) {
      if(_begin.at(i) > point.at(i) || _end.at(i) <= point.at(i)) {
        return false;
      }
    }
    return true;
  }
  
  
  bool Range::contains(const Range& other) const {
    return contains(other._begin) && contains(other._end);
  }
  
  
  Direction Range::getRelativePositionTo(const Range &other) const {
    Direction result(_begin.getSize());
    for(int i = _begin.getSize() - 1; i >= 0; i--) {
      if(_begin.at(i) >= other._end.at(i)) {
        result.set(i, Direction::FORTH);
      } else if(_end.at(i) <= other._begin.at(i)) {
        result.set(i, Direction::BACK);
      } else {
        result.set(i, Direction::NEUTRAL);
      }
    }
    return result;
  }
  
  
  bool Range::isEmpty() const {
    for(int i = _begin.getSize() - 1; i >= 0; i--) {
      if(_end.at(i) <= _begin.at(i)) {
        return true;
      }
    }
    
    return false;
  }
  
  
  const Tuple& Range::getSize() const {
    return _size;
  }
  
  
  kf_int64_t Range::getVolume() const {
    return getSize().productAll();
  }
  
  
  RangeIterator Range::getIterator() const {
    return RangeIterator(*this);
  }
  
  
  Range Range::operator+ (const int n) const {
    return Range(_begin + n, _end + n);
  }
  
  
  Range Range::operator- (const int n) const {
    return Range(_begin - n, _end - n);
  }
  
  
  Range Range::operator* (const int n) const {
    return Range(_begin * n, _end * n);
  }
  
  
  Range Range::operator/ (const int n) const {
    return Range(_begin / n, _end / n);
  }

  
  // Inherited from Streamer //
  
  void Range::printToStream(ostream& os) const {
    os << '(' << _begin << ", " << _end << ')';
  }
  
} // namespace kfoundation