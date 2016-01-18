//
//  Range.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Std
#include <algorithm>

// Internal
#include "Ref.h"
#include "KFException.h"
#include "RangeIterator.h"
#include "OutputStream.h"
#include "PrintWriter.h"
#include "UString.h"

// Self
#include "Range.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Default constructor. Creates a range in 0-D space.
   */
  
  Range::Range() {
    // Nothing;
  }
  
  
  /**
   * Constructor, creates a range in a space of the given dimensions, with
   * all elements assigned to zero.
   */
  
  Range::Range(kf_int8_t size)
  : _begin(size),
    _end(size),
    _size(size)
  {
    // Nothing
  }
  
  
  /**
   * Constructor, creates a range with the given begining and end.
   */
  
  Range::Range(const Tuple& begin, const Tuple& end)
  : _begin(begin),
    _end(end),
    _size(end - begin)
  {
    if(begin.getSize() != end.getSize()) {
      throw KFException(K"Supplied argument do not have the same size: "
          + begin + ", " + end);
    }
  }
  
  
  /**
   * Constructor, creates a range with its begining at the origin and end at
   * the given parameter.
   */
  
  Range::Range(const Tuple& end)
  : _begin(end.getSize()),
    _end(end),
    _size(end)
  {
    // Nothing;
  }
  
  
  /**
   * Copy constructor.
   */
  
  Range::Range(const Range& other)
  : _begin(other._begin),
    _end(other._end),
    _size(other._size)
  {
    // Nothing
  }
  
  
// --- METHODS --- //
  
  /**
   * Returns the number of dimensions.
   */
  
  kf_int8_t Range::getNDimensions() const {
    return _begin.getSize();
  }
  
  
  /**
   * Returns the begining of this range.
   */
  
  const Tuple& Range::getBegin() const {
    return _begin;
  }
  
  
  /**
   * Returns the end of this range.
   */
  
  const Tuple& Range::getEnd() const {
    return _end;
  }
  
  
  /**
   * Returns a unique integer number for the given point in this range.
   */
  
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
  
  
  /**
   * Returns the translation of this range moved by the given tuple.
   */
  
  Range Range::translate(const Tuple &amount) const {
    return Range(_begin + amount, _end + amount);
  }
  
  
  /**
   * Returns the result of moving the boundaries of this range outwards by the
   * given value.
   */
  
  Range Range::grow(const int s) const {
    return Range(_begin - s, _end + s);
  }
  
  
  /**
   * Returns the result of moving the boundaries of this range inwards by the
   * given value.
   */
  
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
  
  
  /**
   * Returns the range of elements on the border of this range with given
   * thickness.
   *
   * @param d The direction of the desired broder.
   * @param s The thickness of the desired border.
   */
  
  Range Range::border(const Direction& d, const int s) const {
    if(d.getSize() != _begin.getSize()) {
      throw KFException(K"Size mismatch. Expected an Direction of size "
          + _begin.getSize() + ", Provided: " + d.getSize());
    }
    
    Range result(_begin.getSize());
    
    for(kf_int8_t i = _begin.getSize() - 1; i >= 0; i--) {
      switch (d.get(i)) {
        case Direction::BACK:
          result._begin.at(i) = _begin.at(i);
          result._end.at(i) = std::min<int>(_begin.at(i) + s, _end.at(i));
          break;
          
        case Direction::FORTH:
          result._begin.at(i) = std::max<int>(_begin.at(i), _end.at(i) - s);
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
  
  
  /**
   * Returns the result of flipping this range to the given direction.
   */
  
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
  
  
  /**
   * Returns the result of intesection of this range and the given parameter.
   */
  
  Range Range::intersectWith(const Range& other) const {
    return Range(_begin.max(other._begin), _end.min(other._end));
  }
  
  
  /**
   * Returns the smallest range containing both this range and the given
   * parameter.
   */
  
  Range Range::joinWith(const Range &other) const {
    return Range(_begin.min(other._begin), _end.max(other._end));
  }
  
  
  /**
   * Returns the smallest range containing both this range and the given point.
   */
  
  Range Range::joinWith(const Tuple& point) const {
    return Range(_begin.min(point), _end.max(point));
  }
  
  
  /**
   * Divides this range to the given divisor and returns the one at the given
   * index.
   */
  
  Range Range::divide(const Tuple& divisor, const Tuple& selector) const {
    Tuple mSize = _size / divisor;
    Tuple mBegin = _begin + mSize * selector;
    Tuple mEnd = mBegin + mSize;
    return Range(mBegin, mEnd);
  }
  
  
  /**
   * Checks if this range is adgecent to the given one.
   */
  
  bool Range::isAdjecentTo(const Range &other) const {
    return intersectWith(other).isEmpty()
        && !intersectWith(other.grow(1)).isEmpty();
  }
  
  
  /**
   * Checks of this range contains the given point.
   */
  
  bool Range::contains(const Tuple& point) const {
    for(kf_int8_t i = _begin.getSize() - 1; i >=0; i--) {
      if(_begin.at(i) > point.at(i) || _end.at(i) <= point.at(i)) {
        return false;
      }
    }
    return true;
  }
  
  
  /**
   * Checks if this range contains the given range.
   */
  
  bool Range::contains(const Range& other) const {
    return contains(other._begin) && contains(other._end);
  }
  
  
  /**
   * Returns the relative direction of this range to the given one.
   */
  
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
  
  
  /**
   * Checks of this range is empty, i.e. it's volume is zero.
   */
  
  bool Range::isEmpty() const {
    for(int i = _begin.getSize() - 1; i >= 0; i--) {
      if(_end.at(i) <= _begin.at(i)) {
        return true;
      }
    }
    
    return false;
  }
  
  
  /**
   * Returns the number of dimensions of this range.
   */
  
  const Tuple& Range::getSize() const {
    return _size;
  }
  
  
  /**
   * Returns the volume of this range.
   */
  
  kf_int64_t Range::getVolume() const {
    return getSize().productAll();
  }
  
  
  /**
   * Returns an iterator for this range.
   */
  
  RangeIterator Range::getIterator() const {
    return RangeIterator(*this);
  }
  
  
  /**
   * Returns the result of adding the given scalar to all elements of this 
   * range.
   */
  
  Range Range::operator+ (const int n) const {
    return Range(_begin + n, _end + n);
  }
  
  
  /**
   * Returns the result of substracting the given scalar from all elements of
   * this range.
   */
  
  Range Range::operator- (const int n) const {
    return Range(_begin - n, _end - n);
  }
  
  
  /**
   * Returns the result of multiplying the given scalar to all elements of this
   * range.
   */
  
  Range Range::operator* (const int n) const {
    return Range(_begin * n, _end * n);
  }
  
  
  /**
   * Returns the result of dividing each element of this range to the given
   * scalar.
   */
  
  Range Range::operator/ (const int n) const {
    return Range(_begin / n, _end / n);
  }

  
  // Inherited from Streamer //
  
  void Range::printToStream(Ref<OutputStream> os) const {
    PrintWriter(os) << '(' << _begin << ", " << _end << ')';
  }
  
} // namespace kfoundation