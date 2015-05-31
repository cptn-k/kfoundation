//
//  Tuple.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/17/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KFoundation__Tuple__
#define __KFoundation__Tuple__

#include "definitions.h"
#include "Streamer.h"

namespace kfoundation {

//// Tuple ////////////////////////////////////////////////////////////////////

  class Tuple : public Streamer {
  // --- FIELDS --- //
    
    protected: kf_int32_t _elements[4];
    private: kf_int8_t _size;
    
  
  // --- STATIC METHODS --- //
    
    public: static Tuple one(kf_int8_t size);
    public: static Tuple zero(kf_int8_t size);
    
    
  // --- CONSTRUCTOR --- //
    
    public: Tuple();
    public: explicit Tuple(kf_int8_t size);
    public: Tuple(const Tuple& other);
    
    
  // --- METHODS --- //
    
    public: inline kf_int8_t getSize() const;
    public: inline kf_int32_t& at(const kf_int8_t index);
    public: inline kf_int32_t at(const kf_int8_t index) const;
    public: void set(const Tuple& other);
    public: bool equals(const Tuple& other) const;
    public: kf_int32_t sumAll() const;
    public: kf_int64_t productAll() const;
    public: Tuple max(const Tuple& other) const;
    public: Tuple min(const Tuple& other) const;
    public: Tuple negate() const;
    public: Tuple operator- () const;
    public: Tuple operator+ (const Tuple& other) const;
    public: Tuple operator- (const Tuple& other) const;
    public: Tuple operator+ (const int n) const;
    public: Tuple operator- (const int n) const;
    public: Tuple operator* (const int n) const;
    public: Tuple operator* (const Tuple& other) const;
    public: Tuple operator/ (const int n) const;
    public: bool operator== (const Tuple& other) const;
    
    // From Streamer
    public: void printToStream(ostream& os) const;
    
  };
  
  
  inline kf_int8_t Tuple::getSize() const {
    return _size;
  }
  
  
  inline kf_int32_t& Tuple::at(const kf_int8_t index) {
    return _elements[index];
  }
  
  
  inline kf_int32_t Tuple::at(const kf_int8_t index) const {
    return _elements[index];
  }

  
//// Tuple1D //////////////////////////////////////////////////////////////////
  
  class Tuple1D : public Tuple {
    
  // --- STATIC FIELDS --- //
    
    public: static Tuple1D ONE;
    public: static Tuple1D ZERO;
    
    
  // --- CONSTRUCTOR --- //
    
    public: Tuple1D();
    public: Tuple1D(kf_int32_t x);
    
    
  // --- METHODS --- //
    
    public: inline Tuple& set(kf_int32_t x);
    public: kf_int32_t get() const;
    public: operator kf_int32_t() const;
    
  };
  
  
  inline Tuple& Tuple1D::set(int x) {
    _elements[0] = x;
    return *this;
  }
  
  
//// Tuple2D //////////////////////////////////////////////////////////////////
  
  class Tuple2D : public Tuple {
  
  // --- STATIC FIELDS --- //
    
    public: static Tuple2D ONE;
    public: static Tuple2D ZERO;
    
  // --- CONSTRUCTOR --- //
    
    public: Tuple2D();
    public: Tuple2D(int x, int y);
    
    
  // --- METHODS --- //
    
    public: Tuple& set(int x, int y);
    public: Tuple& setX(int x);
    public: Tuple& setY(int y);
    public: int getX() const;
    public: int getY() const;
    
  };
  
  
//// Tuple3D //////////////////////////////////////////////////////////////////
  
  class Tuple3D : public Tuple {
    
  // --- STATIC FIELDS --- //
    
    public: static Tuple3D ONE;
    public: static Tuple3D ZERO;
    
    
  // --- CONSTRUCTOR --- //
    
    public: Tuple3D();
    public: Tuple3D(int x, int y, int z);
    
    
  // --- METHODS --- //
    
    public: Tuple& set(int x, int y, int z);
    public: Tuple& setX(int x);
    public: Tuple& setY(int y);
    public: Tuple& setZ(int z);
    public: int getX() const;
    public: int getY() const;
    public: int getZ() const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Tuple__) */