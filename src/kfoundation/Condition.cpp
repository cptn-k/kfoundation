/*---[Condition.cpp]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Condition::*
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

// POSIX
#include <sys/time.h>
#include <pthread.h>

// Internal
#include "definitions.h"

// Self
#include "Condition.h"

#ifdef MEGA
#undef MEGA
#endif

#define MEGA 1000000

namespace kfoundation {
  
//\/ Condition::ConditionImplementation /\/////////////////////////////////////
  
  Condition::ConditionImplementation::~ConditionImplementation() {
    // Nothing;
  }
  
  
//\/ __k_ConditionImplementation /\////////////////////////////////////////////
  
  class __k_ConditionImplementation : public Condition::ConditionImplementation
  {
    
  // --- FIELDS --- //
    
    private: pthread_mutex_t _m;
    private: pthread_mutexattr_t _mAttr;
    private: pthread_condattr_t _attr;
    private: pthread_cond_t _cond;
    private: int _isBlocked;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: __k_ConditionImplementation(bool isShared);
    public: ~__k_ConditionImplementation();
    
    
  // --- METHODS --- //
    
    public: void block(kf_int64_t timeout);
    public: void block();
    public: void release();
    public: void releaseAll();
    public: bool isBlocked() const;
    
  };
  
  
// --- (DE)CONSTRUCTORS --- //
  
  __k_ConditionImplementation::__k_ConditionImplementation(bool isShared) {
    pthread_mutexattr_init(&_mAttr);
    if(isShared) {
      pthread_mutexattr_setpshared(&_mAttr, PTHREAD_PROCESS_SHARED);
    }
    pthread_mutex_init(&_m, &_mAttr);
    
    pthread_condattr_init(&_attr);
    if(isShared) {
      pthread_condattr_setpshared(&_attr, PTHREAD_PROCESS_SHARED);
    }
    pthread_cond_init(&_cond, &_attr);
    
    _isBlocked = 0;
  }
  
  
  __k_ConditionImplementation::~__k_ConditionImplementation() {
    pthread_cond_destroy(&_cond);
    pthread_condattr_destroy(&_attr);
    pthread_mutex_destroy(&_m);
    pthread_mutexattr_destroy(&_mAttr);
  }
  
  
// --- METHODS --- //
  
  void __k_ConditionImplementation::block(kf_int64_t timeout) {
    timespec ts;
    ts.tv_sec = timeout/1000;
    ts.tv_nsec = (timeout%1000)*1000000;
    
    _isBlocked++;
    pthread_mutex_lock(&_m);
    pthread_cond_timedwait(&_cond, &_m, &ts);
    pthread_mutex_unlock(&_m);
    _isBlocked--;
  }
  
  
  void __k_ConditionImplementation::block() {
    _isBlocked++;
    pthread_mutex_lock(&_m);
    pthread_cond_wait(&_cond, &_m);
    pthread_mutex_unlock(&_m);
    _isBlocked--;
  }
  
  
  void __k_ConditionImplementation::release() {
    pthread_mutex_lock(&_m);
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_m);
  }
  
  
  void __k_ConditionImplementation::releaseAll() {
    pthread_mutex_lock(&_m);
    pthread_cond_broadcast(&_cond);
    pthread_mutex_unlock(&_m);
  }
  
  
  bool __k_ConditionImplementation::isBlocked() const {
    return _isBlocked == 0;
  }
  
  
//\/ Condition /\//////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //

  Condition::Condition(bool isShared) {
    _implementation = new __k_ConditionImplementation(isShared);
  }
  
  
  Condition::~Condition() {
    delete _implementation;
  }
  
} // kfoundation