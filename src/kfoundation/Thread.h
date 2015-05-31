//
//  Runnable.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/19/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__Runnable__
#define __KFoundation__Runnable__

// Super
#include "ManagedObject.h"

namespace kfoundation {
  
  class KFException;
  
  class Thread : public ManagedObject {

  // --- NESTED TYPES --- //
    
    public: class ThreadImplementation {
      public: virtual ~ThreadImplementation();
      public: virtual void start() = 0;
      public: virtual bool isRunning() const = 0;
      public: virtual void setName(const string& str) = 0;
      public: virtual const string& getName() const = 0;
      public: virtual bool isTheCurrentThread() const = 0;
    };

    
  // --- STATIC METHODS --- //
    
    public: static string getNameOfCurrentThread();
    
    
  // --- STATIC FIELDS --- //
    
    private: static int _counter;
    
        
  // --- FIELDS --- //
    
    private: ThreadImplementation* _implementation;
    
    
  // --- CONSTRUCTOR --- //
    
    public: Thread();
    public: Thread(const string& name);
    public: ~Thread();
    
    
  // --- METHODS --- //
    
    public: virtual void run() = 0;
    public: void start();
    public: bool isRunning() const;
    public: const string& getName() const;
    public: bool isTheCurrentThread() const;
    
  }; // class Thread
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Runnable__) */