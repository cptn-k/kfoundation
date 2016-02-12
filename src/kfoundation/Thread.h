/*---[Thread.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::Thread::*
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

#ifndef __KFoundation__Runnable__
#define __KFoundation__Runnable__

// Internal
#include "definitions.h"
#include "RefDecl.h"

// Super
#include "KFObject.h"

namespace kfoundation {
  
  class KFException;
  class UString;
  class Condition;
  
  
  /**
   * An object-oriented, cross-platform abstraction for thread. 
   * To use, implement the run() method and call start().
   *
   * @ingroup thread
   * @headerfile Thread.h <kfoundation/Thread.h>
   */
  
  class Thread : public KFObject {

  // --- NESTED TYPES --- //
    
    public: class ThreadImplementation {
      public: virtual ~ThreadImplementation();
      public: virtual void start() = 0;
      public: virtual bool isRunning() const = 0;
      public: virtual void setName(RefConst<UString> name) = 0;
      public: virtual void setTerminateCondition(Ref<Condition> c) = 0;
      public: virtual RefConst<UString> getName() const = 0;
      public: virtual bool isTheCurrentThread() const = 0;
    };

    
  // --- STATIC METHODS --- //
    
    public: static Ref<UString> getNameOfCurrentThread();
    
    
  // --- STATIC FIELDS --- //
    
    private: static kf_int32_t _counter;
    
        
  // --- FIELDS --- //
    
    private: ThreadImplementation* _implementation;

    
  // --- CONSTRUCTOR --- //
    
    public: Thread();
    public: Thread(RefConst<UString> name);
    public: ~Thread();
    
    
  // --- METHODS --- //
    
    public: virtual void run() = 0;
    public: void start();
    public: bool isRunning() const;
    public: void notifyOnTerminate(Ref<Condition> c);
    public: RefConst<UString> getName() const;
    public: bool isTheCurrentThread() const;
    
  }; // class Thread
  
} // namespace kfoundation

#endif /* defined(__KFoundation__Runnable__) */