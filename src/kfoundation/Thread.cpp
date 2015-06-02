//
//  Thread.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/19/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// POSIX
#include <pthread.h>

// Internal
#include "KFException.h"
#include "System.h"
#include "Logger.h"
#include "Int.h"
#include "Ptr.h"

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
    private: string _name;
    private: static void* runner(void* arg);
    public: __k_ThreadImplementation(Thread& owner);
    public: ~__k_ThreadImplementation();
    public: void start();
    public: bool isRunning() const;
    public: void setName(const string& str);
    public: const string& getName() const;
    public: bool isTheCurrentThread() const;
  };
  
  
  void* __k_ThreadImplementation::runner(void* arg) {
    __k_ThreadImplementation* obj = (__k_ThreadImplementation*)arg;
    
    PPtr<Thread> thread = obj->_owner.getPtr().AS(Thread);
    
  #ifdef KF_MAC
    pthread_setname_np(obj->_name.c_str());
  #elif defined(KF_LINUX)
    pthread_setname_np(pthread_self(), obj->_name.c_str());
  #else
    #error "platform not supported"
  #endif
    
    obj->_isRunning = true;
    obj->_owner.run();
    obj->_isRunning = false;
    
    if(thread.isValid()) {
      thread.release();
    }
    
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
  
  
  void __k_ThreadImplementation::setName(const string& str) {
    _name = str;
  }
  
  
  const string& __k_ThreadImplementation::getName() const {
    return _name;
  }
  
  
  bool __k_ThreadImplementation::isTheCurrentThread() const {
    return pthread_equal(pthread_self(), _handle);
  }
  
  
//\/ Thread /\/////////////////////////////////////////////////////////////////
  
// --- STATIC METHODS --- //
  
  string Thread::getNameOfCurrentThread() {
    char name[100];
    pthread_getname_np(pthread_self(), name, 100);
    return string(name);
  }
  
  
// --- STATIC FIELDS --- //
  
  int Thread::_counter = 1;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  Thread::Thread() {
    _counter++;
    _implementation = new __k_ThreadImplementation(*this);
    _implementation->setName("KFoundation Thread " + Int::toString(_counter));
  }
  
  
  Thread::Thread(const string& name) {
    _counter++;
    _implementation = new __k_ThreadImplementation(*this);
    _implementation->setName(name);
  }
  
  
  Thread::~Thread() {
    delete _implementation;
  }
  
  
// --- METHODS --- //
  
  void Thread::start() {
    if(isRunning()) {
      return;
    }
    
    getPtr().retain();
    
    _implementation->start();
  }
  
  
  bool Thread::isRunning() const {
    return _implementation->isRunning();
  }
  
  
  const string& Thread::getName() const {
    return _implementation->getName();
  }
  
  
  bool Thread::isTheCurrentThread() const {
    return _implementation->isTheCurrentThread();
  }
  
} // kfoundation
