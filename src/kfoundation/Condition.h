//
//  Condition.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/19/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Condition__
#define __KFoundation__Condition__

namespace kfoundation {
  
  class Condition {
  
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
    
    public: inline void block(kf_int64_t timeout);
    public: inline void block();
    public: inline void release();
    public: inline void releaseAll();
    public: inline bool isBlocked() const;
    
  };
  
  
// --- INLINE METHODS --- //
  
  inline void Condition::block(kf_int64_t timeout) {
    _implementation->block(timeout);
  }
  
  
  inline void Condition::block() {
    _implementation->block();
  }
  
  
  inline void Condition::release() {
    _implementation->release();
  }
  
  
  inline void Condition::releaseAll() {
    _implementation->releaseAll();
  }
  
  
  inline bool Condition::isBlocked() const {
    return _implementation->isBlocked();
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Condition__) */
