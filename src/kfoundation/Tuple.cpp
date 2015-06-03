//
//  Tuple.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/17/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cstring>

#include "KFException.h"
#include "Int.h"
#include "Tuple.h"

namespace kfoundation {

  using namespace std;
  
//// Tuple ////////////////////////////////////////////////////////////////////
  
  
  Tuple Tuple::one(kf_int8_t size) {
    Tuple t(size);
    for(int i = size - 1; i >= 0; i--) {
      t._elements[i] = 1;
    }
    return t;
  }
  
  
  Tuple Tuple::zero(kf_int8_t size) {
    Tuple t(size);
    for(int i = size - 1; i >= 0; i--) {
      t._elements[i] = 0;
    }
    return t;
  }
  
  
  Tuple::Tuple()
  : _size(0)
  {
    memset(_elements, 0, sizeof(_elements));
  }
  
  
  Tuple::Tuple(kf_int8_t s)
  : _size(s)
  {
    memset(_elements, 0, sizeof(_elements));
  }
  
  
  Tuple::Tuple(const Tuple& other)
  : _size(other._size)
  {
    set(other);
  }
  
  
  void Tuple::set(const Tuple &other) {
    _size = other._size;
    memcpy(_elements, other._elements, _size * sizeof(kf_int32_t));
  }
  
  
  kf_int32_t Tuple::sumAll() const {
    kf_int32_t s = 0;
    for(kf_int8_t i = _size - 1; i >= 0; i--) {
      s += _elements[i];
    }
    return s;
  }
  
  
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
  
  
  Tuple Tuple::max(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
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
  
  
  Tuple Tuple::min(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
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
  
  
  Tuple Tuple::negate() const {
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = - _elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator-() const {
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = - _elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator+(const Tuple &other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
                        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] + other._elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator- (const Tuple &other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
                        + ", Expected: " + Int::toString(_size));
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] - other._elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator+ (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] + n;
    }
    return result;
  }
  
  
  Tuple Tuple::operator- (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] - n;
    }
    return result;
  }
  
  
  Tuple Tuple::operator* (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] * n;
    }
    return result;
  }
  
  
  Tuple Tuple::operator* (const Tuple& other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
                        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] * other._elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator/ (const int n) const {
    Tuple result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._elements[i] = _elements[i] / n;
    }
    return result;
  }
  
  
  Tuple Tuple::operator/(const Tuple& other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
                        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] / other._elements[i];
    }
    
    return result;
  }
  
  
  Tuple Tuple::operator%(const Tuple& other) const {
    if(other._size != _size) {
      throw KFException("Size mismatch. Given: " + Int(other._size)
                        + ", Expected: " + _size);
    }
    
    Tuple result(_size);
    for(int i = 0; i < _size; i++) {
      result._elements[i] = _elements[i] % other._elements[i];
    }
    
    return result;
  }
  
  
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
    
  void Tuple::printToStream(ostream& os) const {
    os << "(";
    for(kf_int8_t i = 0; i < _size; i++) {
      if(i > 0) {
        os << ", ";
      }
      os << _elements[i];
    }
    os << ")";
  }
  
  
//// Tuple1D //////////////////////////////////////////////////////////////////

  
  Tuple1D Tuple1D::ONE(1);
  Tuple1D Tuple1D::ZERO(0);
  
  
  Tuple1D::Tuple1D()
  : Tuple(1)
  {
    _elements[0] = 0;
  }
  
  
  Tuple1D::Tuple1D(int x)
  : Tuple(1)
  {
    _elements[0] = x;
  }
  
  
  int Tuple1D::get() const {
    return _elements[0];
  }
  
  
  Tuple1D::operator kf_int32_t() const {
    return _elements[0];
  }
  
  
//// Tuple2D //////////////////////////////////////////////////////////////////

  
  Tuple2D Tuple2D::ONE(1, 1);
  Tuple2D Tuple2D::ZERO(0, 0);
  
  
  Tuple2D::Tuple2D()
  : Tuple(2)
  {
    _elements[0] = 0;
    _elements[1] = 0;
  }
  
  
  Tuple2D::Tuple2D(int x, int y)
  : Tuple(2)
  {
    _elements[0] = x;
    _elements[1] = y;
  }
  
  Tuple& Tuple2D::set(int x, int y) {
    _elements[0] = x;
    _elements[1] = y;
    return *this;
  }
  
  Tuple& Tuple2D::setX(int x) {
    _elements[0] = x;
    return *this;
  }
  
  Tuple& Tuple2D::setY(int y) {
    _elements[1] = y;
    return *this;
  }
  
  int Tuple2D::getX() const {
    return _elements[0];
  }
  
  int Tuple2D::getY() const {
    return _elements[1];
  }
  

//// Tuple3D //////////////////////////////////////////////////////////////////
  
  
  Tuple3D Tuple3D::ONE(1, 1, 1);
  Tuple3D Tuple3D::ZERO(0, 0, 0);
  
  
  Tuple3D::Tuple3D()
  :Tuple(3)
  {
    _elements[0] = 0;
    _elements[1] = 0;
    _elements[2] = 0;
  }
  
  
  Tuple3D::Tuple3D(int x, int y, int z)
  :Tuple(3)
  {
    _elements[0] = x;
    _elements[1] = y;
    _elements[2] = z;
  }
  
  
  Tuple& Tuple3D::set(int x, int y, int z) {
    _elements[0] = x;
    _elements[1] = y;
    _elements[2] = z;
    return *this;
  }
  
  Tuple& Tuple3D::setX(int x) {
    _elements[0] = x;
    return *this;
  }
  
  Tuple& Tuple3D::setY(int y) {
    _elements[1] = y;
    return *this;
  }
  
  Tuple& Tuple3D::setZ(int z) {
    _elements[2] = z;
    return *this;
  }
  
  int Tuple3D::getX() const {
    return _elements[0];
  }
  
  int Tuple3D::getY() const {
    return _elements[1];
  }
  
  int Tuple3D::getZ() const {
    return _elements[2];
  }
  
  
//// END //////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation