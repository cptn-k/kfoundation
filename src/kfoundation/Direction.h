//
//  Direction.h
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Direction__
#define __KFoundation__Direction__

// Std
#include <string>

// Internal
#include "definitions.h"

// Super
#include "Streamer.h"


namespace kfoundation {
  
  using namespace std;
  
  class Direction : public Streamer {
    
  // --- NESTED TYPES --- //
    
    public: typedef enum {
      BACK    = -1,
      NEUTRAL = 0,
      FORTH   = 1
    } component_t;
    
    
  // --- FIELDS --- //
    
    private: kf_int8_t _components[4];
    private: kf_int8_t _size;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Direction(kf_int8_t size);
    public: Direction(const Direction& copy);
    
    
  // --- METHODS --- //
    
    public: inline kf_int8_t getSize() const;
    public: inline Direction& set(kf_int8_t index, const component_t value);
    public: inline component_t get(kf_int8_t index) const;
    public: void inc(kf_int8_t index);
    public: Direction getOpposite() const;
    public: bool isCenter()  const;
    
    // Inherited from Streamer //
    public: void printToStream(ostream& stream) const;
    
  };
  
  
// --- INLINE METHODS --- //
  
  
  inline kf_int8_t Direction::getSize() const {
    return _size;
  }
  
  
  inline Direction& Direction::set(kf_int8_t index, const component_t value) {
    _components[index] = (kf_int8_t)value;
    return *this;
  }
  
  
  inline Direction::component_t Direction::get(kf_int8_t index) const {
    return (component_t)_components[index];
  }
    
} // nam

#endif /* defined(__KFoundation__Direction__) */
