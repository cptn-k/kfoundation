//
//  Direction.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Std
#include <cstring>

// Internal
#include "OutputStream.h"
#include "PrintWriter.h"
#include "Ref.h"

// Self
#include "Direction.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor, creates a neutral direction of the given size.
   */
  
  Direction::Direction(kf_int8_t s)
  : _size(s)
  {
    memset(_components, 0, _size * sizeof(kf_int8_t));
  }
  
  
  /**
   * Copy constructor.
   */
  
  Direction::Direction(const Direction& copy)
  : _size(copy._size)
  {
    memcpy(_components, copy._components, _size);
  }
  
  
// --- METHODS --- //
  
  void Direction::inc(kf_int8_t index) {
    switch ((component_t)_components[index]) {
      case BACK:
        _components[index] = FORTH;
        break;
        
      case NEUTRAL:
        _components[index] = BACK;
        break;
        
      case FORTH:
        _components[index] = NEUTRAL;
        break;
    }
  }
  
  
  /**
   * Returns a direction opposite to this one.
   */

  Direction Direction::getOpposite() const {
    Direction result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._components[i] = -_components[i];
    }
    return result;
  }
  
  
  /**
   * Checks if this is a neutral direction.
   */
  
  bool Direction::isCenter() const {
    for(int i = _size - 1; i >= 0; i--) {
      if(_components[i] != 0) {
        return false;
      }
    }
    return true;
  }
  
  
  // Inherited from Streamer //
  
  void Direction::printToStream(Ref<OutputStream> os) const {
    PrintWriter pw(os);
    pw << '(';
    for(int i = 0; i < _size; i++) {
      switch (_components[i]) {
        case BACK:
          pw << 'v';
          break;
          
        case FORTH:
          pw << '^';
          break;
          
        case NEUTRAL:
          pw << 'O';
          break;
      }
      
      if(i < _size - 1) {
        pw << ", ";
      }
    }
    
    pw << ')';
  }
  
} // namespace kfoundaiton