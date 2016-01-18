/*---[Mutex.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::Mutex::*
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

#ifndef __KFoundation__Mutex__
#define __KFoundation__Mutex__

/**
 * This mimics the `synchronized` block in Java. For example, with `m` being
 * a Mutex,
 *
 *     KF_SYNCHRONIZED(m,
 *         // Critical region
 *     )
 *.
 *
 * The following syntax is also valid. Use of curly braces and semicolon is 
 * optional.
 *
 *     KF_SYNCHRONIZED(m, {
 *         // Critical region
 *     });
 *.
 *
 * @ingroup defs
 * @ingroup thread
 */

#define KF_SYNCHRONIZED(X, Y) {X .lock();\
{Y}\
X .unlock();} KF_NOP

namespace kfoundation {
  
  /**
   * Mutex, used to prevent multiple threads to enter a critical region.
   *
   * @ingroup thread
   * @headerfile Mutex.h <kfoundation/Mutex.h>
   */
  
  class Mutex {
  
  // --- NESTED TYPES --- //
    
    public: class MutexImplementation {
      public: virtual ~MutexImplementation();
      public: virtual void lock() = 0;
      public: virtual void unlock() = 0;
    };
    
    
  // --- FIELDS --- //
    
    private: MutexImplementation* _implementation;
    
    
  // -- (DE)CONSTRUCTORS --- //
    
    public: Mutex(bool isShared = false);
    public: ~Mutex();
    
    
  // --- METHODS --- //
    
    public: inline void lock();
    public: inline void unlock();
    
  };
  
  
// --- INLINE METHODS --- //
  
  /**
   * Locks the mutex.
   */
  
  inline void Mutex::lock() {
    _implementation->lock();
  }
  
  
  /**
   * Unlocks the mutex.
   */
  
  inline void Mutex::unlock() {
    _implementation->unlock();
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Mutex__) */