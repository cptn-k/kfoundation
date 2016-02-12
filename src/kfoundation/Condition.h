/*---[Condition.h]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::Condition::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef __KFoundation__Condition__
#define __KFoundation__Condition__

#include "KFObject.h"

namespace kfoundation {
  
  /**
   * Condition variable, used to control a thread from another.
   *
   * @ingroup thread
   * @headerfile Condition.h <kfoundation/Condition.h>
   */
  
  class Condition : public KFObject {
  
  // --- NESTED TYPE --- //
    
    public: class ConditionImplementation {
      public: virtual ~ConditionImplementation();
      public: virtual void block(kf_int64_t timeout) = 0;
      public: virtual void block() = 0;
      public: virtual void release() = 0;
      public: virtual void releaseAll() = 0;
      public: virtual bool isBlocked() const = 0;
    };
    
    
  // --- FIELDS --- //
    
    private: ConditionImplementation* _implementation;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Condition(bool isShared = false);
    public: ~Condition();
    
    
  // --- METHODS --- //
    
    public: inline void block(const kf_int64_t timeout);
    public: inline void block();
    public: inline void release();
    public: inline void releaseAll();
    public: inline bool isBlocked() const;
    
  };
  
  
// --- INLINE METHODS --- //
  
  /**
   * Blocks this thread until the release() is called by another thread, or
   * the given timeout is reached. If it is meant to be block for a given 
   * period of time `dt`, use the following pattern:
   *
   *     condition.block(System::getCurrentTimeInMiliseconds() + dt);
   *
   * @param timeout Target absolute time measured in miliseconds.
   */
  
  inline void Condition::block(const kf_int64_t timeout) {
    _implementation->block(timeout);
  }
  
  
  /**
   * Blocks this thread unless release() is called by another thread.
   */
  
  inline void Condition::block() {
    _implementation->block();
  }
  
  
  /**
   * Releases the thread blocked by this condition variable. If multiple threads
   * are blocked, one of them will be released.
   */
  
  inline void Condition::release() {
    _implementation->release();
  }
  
  
  /**
   * Releases all threads blocked by this condition.
   */
  
  inline void Condition::releaseAll() {
    _implementation->releaseAll();
  }
  
  
  /**
   * Checks if this condition variable is currently blocking any thread.
   */
  
  inline bool Condition::isBlocked() const {
    return _implementation->isBlocked();
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Condition__) */