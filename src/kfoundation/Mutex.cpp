/*---[Mutex.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Mutex::*
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


#include <pthread.h>

#include "Mutex.h"

namespace kfoundation {
  
//\/ Mutex::MutexImplementation /\/////////////////////////////////////////////
  
  Mutex::MutexImplementation::~MutexImplementation() {
    // Nothing;
  }
  
  
//\/ __k_MutexImplementation /\////////////////////////////////////////////////
  
  class __k_MutexImplementation : public Mutex::MutexImplementation {
    
  // --- FIELDS --- //
    
    private: pthread_mutexattr_t _attr;
    private: pthread_mutex_t _m;
    
    
  // -- (DE)CONSTRUCTORS --- //
    
    public: __k_MutexImplementation(bool isShared);
    public: ~__k_MutexImplementation();
    
    
  // --- METHODS --- //
    
    public: void lock();
    public: void unlock();
    
  };
  
  
  // --- (DE)CONSTUCTORS --- //
  
  __k_MutexImplementation::__k_MutexImplementation(bool isShared) {
    pthread_mutexattr_init(&_attr);
    if(isShared) {
      pthread_mutexattr_setpshared(&_attr, PTHREAD_PROCESS_SHARED);
    }
    pthread_mutex_init(&_m, &_attr);
  }
  
  
  __k_MutexImplementation::~__k_MutexImplementation() {
    pthread_mutex_destroy(&_m);
    pthread_mutexattr_destroy(&_attr);
  }
  
  
  // --- METHODS --- //
  
  void __k_MutexImplementation::lock() {
    pthread_mutex_lock(&_m);
  }
  
  
  void __k_MutexImplementation::unlock() {
    pthread_mutex_unlock(&_m);
  }

  
//\/ Mutex /\//////////////////////////////////////////////////////////////////
  
// --- (DE)CONSTUCTORS --- //
  
  /**
   * Constructor.
   *
   * @param isShared Should be set true if the mutex is shared between multiple
   *                 dynamic libraries (shared objects). Default value is 
   *                 `false`.
   */
  
  Mutex::Mutex(bool isShared) {
    _implementation = new __k_MutexImplementation(isShared);
  }
  
  
  /**
   * Deconstructor.
   */
  
  Mutex::~Mutex() {
    delete _implementation;
  }
  
} // namespace kfoundation