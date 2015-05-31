/*---[ObjectPoolMemoryManager.h]-------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: ObjectPoolMemoryManagerDecl<T>
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

#ifndef KFOUNDATION_OBJECTPOOLMEMORYMANAGER_H
#define KFOUNDATION_OBJECTPOOLMEMORYMANAGER_H

// Std
#include <cstdlib>

// Internal
#include "InvalidPointerException.h"
#include "MasterMemoryManager.h"
#include "Logger.h"
#include "ObjectSerializer.h"
#include "Ptr.h"
#include "System.h"

// Self
#include "ObjectPoolMemoryManagerDecl.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  template<typename T>
  ObjectPoolMemoryManager<T>::ObjectPoolMemoryManager(const int initialCapacity,
      const int growthRate)
  {
    pthread_mutexattr_init(&_mutexAttrib);
    pthread_mutexattr_setpshared(&_mutexAttrib, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&_mutex, &_mutexAttrib);
    _size  = initialCapacity;
    _growthRate = growthRate;
    _count = 0;
    _serialCounter = 0;
    _next  = 0;
    _table = new ObjectRecord[_size];
    _master = &System::getMasterMemoryManager();
    _id    = _master->registerManager(this);
    _trace = false;
    
    for(int i = 0; i < _size; i++) {
      ObjectRecord& record = _table[i];
      record.retainCount = -1;
      record.manager = _id;
      record.index = i;
      record.serialNumber = -1;
      record.isStatic = false;
      record.ptr = new T(_id, i);
    }
  }
  
  
  template<typename T>
  ObjectPoolMemoryManager<T>::~ObjectPoolMemoryManager() {
    pthread_mutex_destroy(&_mutex);
    
    Statistics stats = getStats();
    LOG << "Terminating memory manager " << _id << ". Orphan Objects: "
        << stats.nObjects << " of which " << stats.nStaticObjects
        << " are static." << EL;
    
    for(int i = 0; i < _size; i++) {
      ((PoolObject*)_table[i].ptr)->finalize();
      delete _table[i].ptr;
    }
    _master->unregisterManager(_id);
    delete[] _table;
  }
  
  
// --- METHODS --- //
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::grow() {
    int newSize = _size * _growthRate;
    ObjectRecord* newTable = new ObjectRecord[newSize];
    memcpy((void*)newTable, (void*)_table, sizeof(ObjectRecord) * _size);
    
    delete[] _table;
    _table = newTable;
    
    LOG << "ObjectPool " << _id << " resized from " << _size
        << " to " << newSize << EL;
    
    for(int i = _size; i < newSize; i++) {
      ObjectRecord& record = newTable[i];
      record.retainCount = -1;
      record.manager = _id;
      record.index = i;
      record.serialNumber = -1;
      record.isStatic = false;
      record.ptr = new T(_id, i);
    }
    
    _size = newSize;
    _master->updataTable(_id);
  }
  
  
  template<typename T>
  string ObjectPoolMemoryManager<T>::toString(int index) {
    const ObjectRecord& rec = _table[index];
    char buffer[400];
    sprintf(buffer, "[serial: %d, index: %d, type: %s, retainCount: %d, "
            "isStatic: %d, key: %d]",
            rec.serialNumber, rec.index,
            System::demangle(typeid(T).name()).c_str(),
            rec.retainCount, rec.isStatic, rec.key);
    return string(buffer);
  }
  
  
  template<typename T>
  Ptr<T> ObjectPoolMemoryManager<T>::get() {
    pthread_mutex_lock(&_mutex);
    int index = _next;
    ObjectRecord& rec = _table[index];
    rec.retainCount = 1;
    rec.serialNumber = _serialCounter++;
    rec.key = rand() % 65530 - 32760;
    rec.manager = _id;
    rec.isStatic = false;
    
    _count++;
    
    if(_count == _size) {
      grow();
    }
    
    while(_table[_next].retainCount != -1) {
      _next++;
      if(_next == _size) {
        _next = 0;
      }
    }
    pthread_mutex_unlock(&_mutex);
    
    
    if(_trace) {
      LOG << "Get: (" << index << ") "<< toString(index) << " Next: " << _next << EL;
    }
    
    Ptr<T> ptr(_id, index);
    ptr.setAutorelease(true);
    return ptr;
  }
  
  
  template<typename T>
  const ObjectRecord&
  ObjectPoolMemoryManager<T>::registerObject(ManagedObject* obj) {
    throw KFException("Operation not supported");
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::retain(kf_int32_t index, kf_int16_t key) {
    pthread_mutex_lock(&_mutex);
    
    ObjectRecord& record = _table[index];
    if(record.key != key) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException("The pointer being retained is invalid: "
                                    + Int(_id) + ":" + Int(index));
    }
    
    if(!record.isStatic) {
      record.retainCount++;
    }
    
    pthread_mutex_unlock(&_mutex);
    
    if(_trace) {
      LOG << "Retained: " << toString(record.index) << EL;
    }
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::release(kf_int32_t index, kf_int16_t key) {    
#ifdef DEBUG
    string recStr;
    if(_trace) {
      recStr = toString(index);
    }
#endif
    
    // synchronized {
    pthread_mutex_lock(&_mutex);
    ObjectRecord& record = _table[index];
    if(record.key != key) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException("The pointer being released is invalid: "
                                    + Int(_id) + ":" + Int(index));
    }
    if(!record.isStatic) {
      record.retainCount--;
      if(record.retainCount == 0) {
        record.retainCount = -1;
        record.key = rand() % 65530 - 32760;
        _count--;
      } else if(record.retainCount < 0) {
        pthread_mutex_unlock(&_mutex);
        throw InvalidPointerException("Object is released too many times: "
                                      + Int(_id) + ":" + Int(index));
      }
    }
    pthread_mutex_unlock(&_mutex);
    // } synchronized
    
    if(record.retainCount == -1) {
      dynamic_cast<PoolObject*>(record.ptr)->finalize();
    }
    
#ifdef DEBUG
    if(_trace) {
      string x;
      if(record.retainCount == -1) {
        x = " X ";
      }
      LOG << "Released: " << recStr << " --> " << record.retainCount << x << EL;
    }
#endif
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::remove(kf_int32_t index, kf_int16_t key) {
    ObjectRecord& record = _table[index];
    if(key == record.key) {
      record.retainCount = -1;
      record.key = rand() % 65530 - 32760;
    }
    if(_trace) {
      LOG << "Removed: " << toString(record.index) << EL;
    }
  }

  template<typename T>
  ObjectRecord* ObjectPoolMemoryManager<T>::getTable() {
    return _table;
  }
  
  
  template<typename T>
  kf_int32_t ObjectPoolMemoryManager<T>::getTableSize() const {
    return _size;
  }
  
  
  template<typename T>
  Statistics ObjectPoolMemoryManager<T>::getStats() const {
    Statistics stats;
    stats.nObjects = 0;
    stats.nStaticObjects = 0;
    for(int i = 0; i < _size; i++) {
      if(_table[i].retainCount != -1) {
        if(_table[i].isStatic) {
          stats.nStaticObjects++;
        } else {
          stats.nObjects++;
        }
      }
    }
    return stats;
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::trace(const pthread_t threadId) {
    _trace = true;
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::untrace() {
    _trace = false;
  }
  
  
  template<typename T>
  void ObjectPoolMemoryManager<T>::serialize(PPtr<ObjectSerializer> seralizer)
  const
  {
    seralizer->object("ObjectPoolMemoryManager")
      ->attribute("id", _id)
      ->attribute("count", _count);
    
    int c = _size;
    
    for(int i = 0; i < c; i++) {
      const ObjectRecord& rec = _table[i];
      if(rec.retainCount != -1) {
        seralizer->member("[" + Int(i) + "]")->object("ObjectRecord")
          ->attribute("type", System::demangle(typeid(*rec.ptr).name()))
          ->attribute("retainCount", rec.retainCount)
          ->attribute("isStatic", rec.isStatic)
          ->attribute("index", rec.index)
          ->attribute("key", rec.key)
          ->attribute("serialNumber", rec.serialNumber)
          ->endObject();
      }
    }
    
    seralizer->endObject();
  }
  
} // namespace kfoundation

#endif // defined(KFOUNDATION_OBJECTPOOLMEMORYMANAGER_H)
