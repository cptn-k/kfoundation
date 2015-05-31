//
//  Mutex.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/19/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Mutex__
#define __KFoundation__Mutex__

#define KF_SYNCHRONIZED(X, Y) {X .lock();\
{Y}\
X .unlock();} KF_NOP

namespace kfoundation {
  
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
  
  inline void Mutex::lock() {
    _implementation->lock();
  }
  
  
  inline void Mutex::unlock() {
    _implementation->unlock();
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Mutex__) */
