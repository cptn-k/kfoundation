//
//  Tuple.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/17/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team.
//  All rights reserved.
//

// Std
#include <cstring>

// Internal
#include "KFException.h"
#include "UString.h"
#include "Ref.h"
#include "OutputStream.h"
#include "PrintWriter.h"

// Self
#include "Tuple.h"

namespace kfoundation {
  
//// Tuple ////////////////////////////////////////////////////////////////////
  
  /**
   * Returns a unity tuple of the given size.
   */
  
  Tuple Tuple::one(kf_int8_t size) {
    Tuple t(size);
    for(int i = size - 1; i >= 0; i--) {
      t._elements[i] = 1;
    }
    return t;
  }
  
  
  /**
   * Returns a zero tuple of the given size.
   */
  
  Tuple Tuple::zero(kf_int8_t size) {
    Tuple t(size);
    for(int i = size - 1; i >= 0; i--) {
      t._elements[i] = 0;
    }
    return t;
  }
  
  
  /**
   * Default constructor, creates a tuple of size 0.
   */
  
  Tuple::Tuple()
  : _size(0)
  {
    memset(_elements, 0, sizeof(_elements));
  }
  
  
  /**
   * Constructor, creates a tuple of the given size.
   */
  
  Tuple::Tuple(kf_int8_t s)
  : _size(s)
  {
    memset(_elements, 0, sizeof(_elements));
  }
  
  
  /**
   * Copy constructor.
   */
  
  Tuple::Tuple(const Tuple& other)
  : _size(other._size)
  {
    set(other);
  }
  
  
  /**
   * Setter, sets the elements of this tuple to the ones of the given parameter.
   */
  
  void Tuple::set(const Tuple &other) {
    _size = other._size;
    memcpy(_elements, other._elements, _size * sizeof(kf_int32_t));
  }
  
  
  /**
   * Returns the sum of all the elements.
   */
  
  kf_int32_t Tuple::sumAll() const {
    kf_int32_t s = 0;
    for(kf_int8_t i = _size - 1; i >= 0; i--) {
      s += _elements[i];
    }
    return s;
  }
  
  
  /**
   * Returns the product of all elements.
   */
  
  kf_int64_t Tuple::productAll() const {
    if(_size == 0) {
      return 0;
    }
    
    kf_int64_t p = 1;
    for(kf_int8_t i = _size - 1; i >= 0; i--) {
      p *= _elements[i];
    }
    return p;
  }
  
  
  /**
   * Checks if this tuple equals to the given parameter.
   */
  
  bool Tuple::equals(const Tuple &other) const {
    if(other._size != _size) {
      return false;
    }
    
    for(kf_int8_t i = _size - 1; i >= 0; i--) {
      if(other._elements[i] != _elements[i]) {
        return false;
      }
    }
    
    return true;
  }
  
  
  /**
   * Returns the value of the element with the highest value.
   */
  
  Tuple Tuple::max(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      if(_elements[i] > other._elements[i]) {
        result._elements[i] = _elements[i];
      } else {
        result._elements[i] = other._elements[i];
      }
    }
    
    return result;
  }
  
  
  /**
   * Returns the value of the element with the lowest value.
   */
  
  Tuple Tuple::min(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
          + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      if(_elements[i] < other._elements[i]) {
        result._elements[i] = _elements[i];
      } else {
        result._elements[i] = other._elements[i];
      }
    }
    
    return result;
  }
  
  
  /**
   * Negates all the elements.
   * @return A new tuple with each of its element being the inverse of that of 
   *         this one's.
   */
  
  Tuple Tuple::negate() const {
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = - _elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Additive inverse operator.
   * @return A new tuple with each of its element being the inverse of that of
   *         this one's.
   */
  
  Tuple Tuple::operator-() const {
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = - _elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Addition operator.
   *
   * @param other The tuple to add to.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator+(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
         + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] + other._elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Substraction operator.
   *
   * @param other The tuple to substract.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator- (const Tuple &other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] - other._elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Adds the given scalar to all elements of this tuple.
   *
   * @param other The scalar to add.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator+ (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] + n;
    }
    return result;
  }
  
  
  /**
   * Substracts the given scalar from all elements of this tuple.
   *
   * @param other The scalar to substract.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator- (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] - n;
    }
    return result;
  }
  
  
  /**
   * Multiplies all elements of this tuple by the given number.
   *
   * @param other The scalar to multiply.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator* (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] * n;
    }
    return result;
  }
  
  
  /**
   * Multiplies all elements of this tuple by the given number.
   *
   * @param other The scalar to multiply.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator* (const Tuple& other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
          + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] * other._elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Divides all elements of this tuple by the given number.
   *
   * @param other The scalar to divide to.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator/ (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] / n;
    }
    return result;
  }
  
  
  /**
   * Divides each element of this tuple by corresponding element of the given
   * tuple.
   *
   * @param other The tuple to divide to.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator/(const Tuple& other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
          + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] / other._elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Calculates remainder of each element of this tuple divided by 
   * corresponding element of the given tuple.
   *
   * @param other The tuple to divide to.
   * @return The resulting tuple.
   */
  
  Tuple Tuple::operator%(const Tuple& other) const {
    if(other._size != _size) {
      throw KFException(K"Size mismatch. Given: " + other._size
          + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] % other._elements[i];
    }
    
    return result;
  }
  
  
  /**
   * Equality operator.
   */
  
  bool Tuple::operator==(const Tuple &other) const {
    if(other._size != _size) {
      return false;
    }
    
    for(kf_int8_t i = _size - 1; i >= 0; i--) {
      if(other._elements[i] != _elements[i]) {
        return false;
      }
    }
    
    return true;
  }


  void Tuple::printToStream(Ref<OutputStream> os) const {
    PrintWriter pw(os);
    pw << "(";
    for(kf_int8_t i = 0; i < _size; i++) {
      if(i > 0) {
        pw << ", ";
      }
      pw << _elements[i];
    }
    pw << ")";
  }
  
  
//// Tuple1D //////////////////////////////////////////////////////////////////

  
  Tuple1D Tuple1D::ONE(1);  ///< Unity, (1)
  Tuple1D Tuple1D::ZERO(0); ///< Zero, (0)
  
  
  /**
   * Default constructor, creates a zero 1D tuple.
   */
  
  Tuple1D::Tuple1D()
  : Tuple(1)
  {
    _elements[0] = 0;
  }
  
  
  /**
   * Constructor, assigns the given parameter to the solve element of this
   * tuple.
   *
   * @param x The value to be assigned.
   */
  
  Tuple1D::Tuple1D(int x)
  : Tuple(1)
  {
    _elements[0] = x;
  }
  
  
  /**
   * Getter, returns the value of the sole element of this tuple.
   */
  
  int Tuple1D::get() const {
    return _elements[0];
  }
  
  
  /**
   * Cast operator, casts this tuple to scalar of type kf_int32_t.
   */
  
  Tuple1D::operator kf_int32_t() const {
    return _elements[0];
  }
  
  
//// Tuple2D //////////////////////////////////////////////////////////////////

  
  Tuple2D Tuple2D::ONE(1, 1);  ///< Unity, (1, 1)
  Tuple2D Tuple2D::ZERO(0, 0); ///< Zero, (0, 0)
  
  
  /**
   * Defautl constructor, creates a zero 2-dimensional tuple.
   */
  
  Tuple2D::Tuple2D()
  : Tuple(2)
  {
    _elements[0] = 0;
    _elements[1] = 0;
  }
  
  
  /**
   * Constructor, assigns the elements of the new tuple according to the
   * given parameters.
   *
   * @param x Value for the first element.
   * @param y Value for the second element.
   */
  
  Tuple2D::Tuple2D(int x, int y)
  : Tuple(2)
  {
    _elements[0] = x;
    _elements[1] = y;
  }
  
  
  /**
   * Setter, sets the elements of this tuple according to the given parameters.
   *
   * @param x Value for the first element.
   * @param y Value for the second element.
   */
  
  Tuple& Tuple2D::set(int x, int y) {
    _elements[0] = x;
    _elements[1] = y;
    return *this;
  }
  
  /**
   * Setter, sets the first element of this tuple to the given parameter.
   */
  
  Tuple& Tuple2D::setX(int x) {
    _elements[0] = x;
    return *this;
  }
  
  
  /**
   * Setter, sets the second element of this tuple to the given parameter.
   */
  
  Tuple& Tuple2D::setY(int y) {
    _elements[1] = y;
    return *this;
  }
  
  
  /**
   * Getter, returns the value of the first element of this tuple.
   */
  
  int Tuple2D::getX() const {
    return _elements[0];
  }
  
  
  /**
   * Getter, returns the value of the second element of this tuple.
   */
  
  int Tuple2D::getY() const {
    return _elements[1];
  }
  

//// Tuple3D //////////////////////////////////////////////////////////////////
  
  
  Tuple3D Tuple3D::ONE(1, 1, 1);  ///< Unity, (1, 1, 1)
  Tuple3D Tuple3D::ZERO(0, 0, 0); ///< Zeor, (0, 0, 0)
  
  
  /**
   * Default constructor, creates a zero 3-dimensional tuple.
   */
  
  Tuple3D::Tuple3D()
  :Tuple(3)
  {
    _elements[0] = 0;
    _elements[1] = 0;
    _elements[2] = 0;
  }
  
  
  /**
   * Constructor, assigns the elements of the new tuple according to the given
   * parameters.
   *
   * @param x The value for the first element.
   * @param y The value for the second element.
   * @param z The value for the third element.
   */
  
  Tuple3D::Tuple3D(int x, int y, int z)
  :Tuple(3)
  {
    _elements[0] = x;
    _elements[1] = y;
    _elements[2] = z;
  }
  
  
  /**
   * Setter, sets the elements of this tuple according to the given parameters.
   *
   * @param x The value for the first element.
   * @param y The value for the second element.
   * @param z The value for the third element.
   */
  
  Tuple& Tuple3D::set(int x, int y, int z) {
    _elements[0] = x;
    _elements[1] = y;
    _elements[2] = z;
    return *this;
  }
  
  
  /**
   * Setter, sets the first element of this tuple to the given parameter.
   */
  
  Tuple& Tuple3D::setX(int x) {
    _elements[0] = x;
    return *this;
  }

  
  /**
   * Setter, sets the second element of this tuple to the given parameter.
   */
  
  Tuple& Tuple3D::setY(int y) {
    _elements[1] = y;
    return *this;
  }
  

  /**
   * Setter, sets the third element of this tuple to the given parameter.
   */
  
  Tuple& Tuple3D::setZ(int z) {
    _elements[2] = z;
    return *this;
  }
  
  
  /**
   * Getter, returns the value of the first element of this tuple.
   */
  
  int Tuple3D::getX() const {
    return _elements[0];
  }
  
  
  /**
   * Getter, returns the value of the second element of this tuple.
   */
  
  int Tuple3D::getY() const {
    return _elements[1];
  }
  
  
  /**
   * Getter, returns the value of the third element of this tuple.
   */
  
  int Tuple3D::getZ() const {
    return _elements[2];
  }
  
  
//// END //////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation