//
//  Direction.h
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Direction__
#define __KFoundation__Direction__

// Internal
#include "definitions.h"

// Super
#include "Streamer.h"


namespace kfoundation {
  
  
  /**
   * Represents directions in n-dimensional space. n can be between 0 to 4.
   *
   * @ingroup range
   * @headerfile Direction.h <kfoundation/Direction.h>
   */
  
  class Direction : public Streamer {
    
  // --- NESTED TYPES --- //
    
    /**
     * Directions alongside a line.
     */
    
    public: typedef enum {
      BACK    = -1, ///< Backward
      NEUTRAL = 0,  ///< Netural
      FORTH   = 1   ///< Forward
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
    public: void printToStream(Ref<OutputStream> os) const;
    
  };
  
  
// --- INLINE METHODS --- //
  
  
  /**
   * Returns the number of elements.
   */
  
  inline kf_int8_t Direction::getSize() const {
    return _size;
  }
  
  
  /**
   * Setter, sets the element at the given index to the given value.
   *
   * @return Reference to self.
   */
  
  inline Direction& Direction::set(kf_int8_t index, const component_t value) {
    _components[index] = (kf_int8_t)value;
    return *this;
  }
  
  
  /**
   * Returns the value of element at the given index.
   */
  
  inline Direction::component_t Direction::get(kf_int8_t index) const {
    return (component_t)_components[index];
  }
    
} // nam

#endif /* defined(__KFoundation__Direction__) */
