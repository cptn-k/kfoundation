/*---[Thread.cpp]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Thread::*
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
#include <pthread.h>

// Internal
#include "KFException.h"
#include "System.h"
#include "Logger.h"
#include "Int.h"
#include "Ref.h"
#include "UString.h"

// Self
#include "Thread.h"

namespace kfoundation {
  
//\/ Thread::ThreadImplementation /\///////////////////////////////////////////
  
  Thread::ThreadImplementation::~ThreadImplementation() {
    // Nothing;
  }
  
  
//\/ __k_ThreadImplementation /\///////////////////////////////////////////////

  class __k_ThreadImplementation : public Thread::ThreadImplementation {
    private: pthread_attr_t _attrib;
    private: pthread_t _handle;
    private: Thread& _owner;
    private: bool _isRunning;
    private: RefConst<UString> _name;
    private: static void* runner(void* arg);
    public: __k_ThreadImplementation(Thread& owner);
    public: ~__k_ThreadImplementation();
    public: void start();
    public: bool isRunning() const;
    public: void setName(RefConst<UString> name);
    public: RefConst<UString> getName() const;
    public: bool isTheCurrentThread() const;
  };
  
  
  void* __k_ThreadImplementation::runner(void* arg) {
    __k_ThreadImplementation* obj = (__k_ThreadImplementation*)arg;


  /// TODO UString needs to be null-terminated.
  /// TODO remove hash and code-point count overhead from UString


  #ifdef KF_MAC
    pthread_setname_np((char*)obj->_name->getOctets());
  #elif defined(KF_LINUX)
    pthread_setname_np(pthread_self(), (char*)obj->_name->getOctets());
  #else
    #error "platform not supported"
  #endif
    
    obj->_isRunning = true;
    obj->_owner.run();
    obj->_isRunning = false;

    kf_uref_t ref = obj->_owner.getRef();

    System::getMasterMemoryManager().getManagerAtIndex(ref.index)
        .release(ref.index, ref.key);

    return 0;
  }
  
  
  __k_ThreadImplementation::__k_ThreadImplementation(Thread& owner)
  : _owner(owner)
  {
    pthread_attr_init(&_attrib);
    pthread_attr_setdetachstate(&_attrib, PTHREAD_CREATE_DETACHED);
    _isRunning = false;
  }
  
  
  __k_ThreadImplementation::~__k_ThreadImplementation() {
    pthread_attr_destroy(&_attrib);
  }
  
  
  void __k_ThreadImplementation::start() {
    pthread_create(&_handle, &_attrib, &__k_ThreadImplementation::runner, this);
  }
  
  
  bool __k_ThreadImplementation::isRunning() const {
    return _isRunning;
  }
  
  
  void __k_ThreadImplementation::setName(RefConst<UString> str) {
    _name = str;
  }
  
  
  RefConst<UString> __k_ThreadImplementation::getName() const {
    return _name;
  }
  
  
  bool __k_ThreadImplementation::isTheCurrentThread() const {
    return pthread_equal(pthread_self(), _handle);
  }
  
  
//\/ Thread /\/////////////////////////////////////////////////////////////////
  
// --- STATIC METHODS --- //
  
  /**
   * Returns the name of the thread that this method is invoked on.
   */
  
  Ref<UString> Thread::getNameOfCurrentThread() {
    char name[100];
    pthread_getname_np(pthread_self(), name, 100);
    return new UString(name);
  }
  
  
// --- STATIC FIELDS --- //
  
  int Thread::_counter = 1;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Default constructor.
   * Assigns a default name to this thread as "KFoundation Thread N" where N
   * is a incremental counter.
   */
  
  Thread::Thread() {
    _counter++;
    _implementation = new __k_ThreadImplementation(*this);
    _implementation->setName(K"KFoundation Thread " + _counter);
  }
  
  
  /**
   * Constructs a named thread.
   *
   * @see getName()
   * @see getNameOfCurrentThread()
   */
  
  Thread::Thread(RefConst<UString> name) {
    _counter++;
    _implementation = new __k_ThreadImplementation(*this);
    _implementation->setName(name);
  }
  
  
  /**
   * Deconstructor.
   */
  
  Thread::~Thread() {
    delete _implementation;
  }
  
  
// --- METHODS --- //
  
  /**
   * Creates a new thread, invoking the run() method.
   * This object is retained once the thread is started and released once it is
   * ended.
   */
  
  void Thread::start() {
    if(isRunning()) {
      return;
    }

    kf_uref_t ref = getRef();

    System::getMasterMemoryManager().getManagerAtIndex(ref.index)
      .retain(ref.index, ref.key);

    _implementation->start();
  }
  
  
  /**
   * Checks if this thread is running.
   * A thead is in running state after the start() method is called, and before
   * run() method ends.
   */
  
  bool Thread::isRunning() const {
    return _implementation->isRunning();
  }
  
  
  /**
   * Returns the name of this thread.
   */
  
  RefConst<UString> Thread::getName() const {
    return _implementation->getName();
  }
  
  
  /**
   * Returns true if this method is invoked on the thread represented by this
   * object. If invoked from another thread, returns false.
   */
  
  bool Thread::isTheCurrentThread() const {
    return _implementation->isTheCurrentThread();
  }
  
  
  /**
   * @fn kfoundation::Thread::run()
   *
   * The implementation of this method will be called once a new thread is 
   * started. When this method exits, the thread is considered to be ended, 
   * and it will be destroyed.
   */
  
} // kfoundation