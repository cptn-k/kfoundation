/*---[RefCountMemoryManager.cpp]-------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: RefCountMemoryManager
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

// std
#include <cstdlib>
#include <cassert>

// Internal
#include "Int.h"
#include "InvalidPointerException.h"
#include "MasterMemoryManager.h"
#include "Logger.h"
#include "ObjectSerializer.h"
#include "Ptr.h"

// Self
#include "RefCountMemoryManager.h"

namespace kfoundation {
  
// --- STATIC FIELDS --- //
  
  const int RefCountMemoryManager::INITIAL_SIZE = 64;
  const int RefCountMemoryManager::GROWTH_RATE = 4;
  

// --- (DE)CONSTRUCTORS --- //
  
  RefCountMemoryManager::RefCountMemoryManager(MasterMemoryManager* master) {
    pthread_mutexattr_init(&_attribs);
    pthread_mutexattr_setpshared(&_attribs, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&_mutex, &_attribs);
    _size  = INITIAL_SIZE;
    _count = 0;
    _next  = 0;
    _counter = 0;
    
    _table = new ObjectRecord[_size];
    for(int i = 0; i < _size; i++) {
      _table[i].ptr = 0;
    }
    
    _id    = master->registerManager(this);
    _master = master;
    _trace = false;
    _isClosed = false;
  }
  
  
  RefCountMemoryManager::~RefCountMemoryManager() {
    pthread_mutex_destroy(&_mutex);
    pthread_mutexattr_destroy(&_attribs);
    
    Statistics stats = getStats();
    LOG << "Terminating memory manager " << _id << ". Orphan Objects: "
        << stats.nObjects << " of which " << stats.nStaticObjects
        << " are static." << EL;
    
    _master->unregisterManager(_id);
    delete[] _table;
  }
  
  
// --- METHODS --- //
  
  void RefCountMemoryManager::grow() {
    int newSize = _size * GROWTH_RATE;
    ObjectRecord* newTable = new ObjectRecord[newSize];
    memset(newTable, 0, sizeof(ObjectRecord) * newSize);
    memcpy((void*)newTable, (void*)_table, sizeof(ObjectRecord) * _size);
    delete[] _table;
    
    LOG << "RefCountMemoryManager " << _id << " resized from " << _size
        << " to " << newSize << EL;
    
    _size = newSize;
    _table = newTable;
    _master->updataTable(_id);
  }
  
  
  string RefCountMemoryManager::toString(int index) {
    const ObjectRecord& rec = _table[index];
    string typeName = "(deleted)";
    if(rec.ptr != NULL) {
      typeName = System::demangle(typeid(*rec.ptr).name());
    }
    
    char buffer[400];
    sprintf(buffer, "[manager: %d, index: %d, type: %s, "
                    "retainCount: %d, isStatic: %d, key: %d, serial: %d]",
            rec.manager, rec.index, typeName.c_str(), rec.retainCount,
            rec.isStatic, rec.key, rec.serialNumber);
    return string(buffer);
  }
  
  
  const ObjectRecord& RefCountMemoryManager::registerObject(ManagedObject* obj)
  {
    if(_isClosed) {
      throw MemoryException("Cannot register new object to a closed manager");
    }
    
    pthread_mutex_lock(&_mutex);
    
    int index = _next;
    
    ObjectRecord& record = _table[index];
    record.retainCount = 1;
    record.index = index;
    record.serialNumber = _counter;
    record.ptr = obj;
    record.key = rand() % 65530 - 32760;
    record.manager = _id;
    record.isStatic = false;
    record.isBeingDeleted = false;

    _count++;
    
    if(_count == _size) {
      grow();
      record = _table[index];
    }
    
    while(_table[_next].ptr) {
      _next++;
      if(_next == _size) {
        _next = 0;
      }
    }

    pthread_mutex_unlock(&_mutex);
    
    if(_trace) {
      LOG << "Registered: " << toString(_next) << " Next: " << _next
          << EL;
    }
    
    return record;
  }
  
  
  void RefCountMemoryManager::retain(kf_int32_t index, kf_int16_t key) {
    pthread_mutex_lock(&_mutex);
    ObjectRecord& record = _table[index];
    if(record.key != key) {
      pthread_mutex_unlock(&_mutex);
      untrace();
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
  
  
  void RefCountMemoryManager::release(kf_int32_t index, kf_int16_t key) {
    bool doDelete = false;
    
    #ifdef DEBUG
    string recStr;
    if(_trace) {
      recStr = toString(index);
    }
    #endif
    
    // synchronized {
    pthread_mutex_lock(&_mutex);
    ObjectRecord* tbl = _table;
    ObjectRecord* record = _table +index;
    
    if(record->key != key) {
      pthread_mutex_unlock(&_mutex);
      untrace();
      throw InvalidPointerException("The pointer being released is invalid: "
                                    + Int(_id) + ":" + Int(index));
    }
    
    if(!record->isStatic) {
      record->retainCount--;
      if(record->retainCount == 0 && !record->isBeingDeleted) {
        doDelete = true;
        record->isBeingDeleted = true;
      } else if(record->retainCount < 0) {
        pthread_mutex_unlock(&_mutex);
        throw InvalidPointerException("Object is released too many times: "
                                      + Int(_id) + ":" + Int(index));
      }
    }
    assert(tbl == _table);
    pthread_mutex_unlock(&_mutex);
    // } synchronized
    
    #ifdef DEBUG
    if(_trace) {
      LOG << "Released: " << recStr << " --> " << record->retainCount << EL;
    }
    #endif
    
    if(doDelete) {
      delete record->ptr;
      record->ptr = NULL;
    }
  }
  
  
  void RefCountMemoryManager::remove(kf_int32_t index, kf_int16_t key) {
    ObjectRecord& record = _table[index];
    if(record.key != key) {
      throw InvalidPointerException("The pointer being removed is invalid: "
          + Int(_id) + ":" + Int(index));
    }
    
    if(record.ptr != NULL) {
      pthread_mutex_lock(&_mutex);
      record.ptr = NULL;
      record.retainCount = 0;
      pthread_mutex_unlock(&_mutex);
    }
    
    _count--;
    
    if(!record.isStatic) {
      record.key = rand() % 65530 - 32760;
    }
    
    if(_trace) {
      LOG << "Removed: " << toString(record.index) << EL;
    }
  }
  
  
  ObjectRecord* RefCountMemoryManager::getTable() {
    return _table;
  }
  
  
  kf_int32_t RefCountMemoryManager::getTableSize() const {
    return _size;
  }
  
  
  void RefCountMemoryManager::trace(const pthread_t threadId) {
    _trace = true;
  }
  
  
  void RefCountMemoryManager::untrace() {
    _trace = false;
  }
  
  
  Statistics RefCountMemoryManager::getStats() const {
    Statistics stats;
    stats.nObjects = 0;
    stats.nStaticObjects = 0;
    for(int i = 0; i < _size; i++) {
      if(_table[i].ptr != NULL) {
        if(_table[i].isStatic) {
          stats.nStaticObjects++;
        }
        stats.nObjects++;
      }
    }
    return stats;
  }
  
  
  kf_int16_t RefCountMemoryManager::update(kf_int16_t index, kf_int16_t key) {
    for(int i = 0; i < _size; i++) {
      if(_table[i].index == index && _table[i].key == key) {
        return i;
      }
    }
    return -1;
  }
  
  
  int RefCountMemoryManager::migrate(MasterMemoryManager& master) {
    int size = 0;
    for(int i = 0; i < _size; i++) {
      if(_table[i].ptr != NULL) {
        size = i + 1;
      }
    }
    
    _id = master.registerManager(this);
    
    ObjectRecord* newTable = new ObjectRecord[size];
    for(int i = 0; i < size; i++) {
      newTable[i] = _table[i];
      newTable[i].manager = _id;
    }
    
    _size = size;
    _table = newTable;
    _master = &master;
    _master->updataTable(_id);
    
    return _id;
  }
  
  
  void RefCountMemoryManager::finalize() {
    for(int i = 0; i < _size; i++) {
      ObjectRecord& record = _table[i];
      if(record.ptr != NULL && record.isStatic) {
        delete record.ptr;
        record.ptr = NULL;
        record.retainCount = 0;
      }
    }
  }
  
  
  void RefCountMemoryManager::serialize(PPtr<ObjectSerializer> seralizer) const
  {
    seralizer->object("RefCountMemoryManager")
             ->attribute("id", _id)
             ->attribute("count", _count);
    
    int c = _size;
    
    for(int i = 0; i < c; i++) {
      const ObjectRecord& rec = _table[i];
      if(rec.ptr != NULL) {
        seralizer->member("[" + Int(i) + "]")->object("ObjectRecord")
                 ->attribute("type", System::demangle(typeid(*rec.ptr).name()))
                 ->attribute("retainCount", rec.retainCount)
                 ->attribute("isStatic", rec.isStatic)
                 ->attribute("index", rec.index)
                 ->attribute("key", rec.key)
                 ->endObject();
      }
    }
    
    seralizer->endObject();
  }
  
  
} // namespace kfoundation
