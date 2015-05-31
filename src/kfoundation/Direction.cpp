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
#include "Int.h"

// Self
#include "Direction.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  Direction::Direction(kf_int8_t s)
  : _size(s)
  {
    memset(_components, 0, _size * sizeof(kf_int8_t));
  }
  
  
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
  

  Direction Direction::getOpposite() const {
    Direction result(_size);
    for(int i = _size - 1; i >= 0; i--) {
      result._components[i] = -_components[i];
    }
    return result;
  }
  
  
  bool Direction::isCenter() const {
    for(int i = _size - 1; i >= 0; i--) {
      if(_components[i] != 0) {
        return false;
      }
    }
    return true;
  }
  
  
  // Inherited from Streamer //
  
  void Direction::printToStream(ostream& stream) const {
    stream << '(';
    for(int i = 0; i < _size; i++) {
      switch (_components[i]) {
        case BACK:
          stream << 'v';
          break;
          
        case FORTH:
          stream << '^';
          break;
          
        case NEUTRAL:
          stream << 'O';
          break;
      }
      
      if(i < _size - 1) {
        stream << ", ";
      }
    }
    
    stream << ')';
  }
  
} // namespace kfoundaiton
