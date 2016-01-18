/*---[RefCountMemoryManager.cpp]-------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::RefCountMemoryManager::*
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
#include <cstring>
#include <cassert>

// Internal
#include "Int.h"
#include "InvalidPointerException.h"
#include "MasterMemoryManager.h"
#include "Logger.h"
#include "ObjectSerializer.h"
#include "Ref.h"
#include "UString.h"
#include "StringPrintWriter.h"

// Self
#include "RefCountMemoryManager.h"

namespace kfoundation {
  
// --- STATIC FIELDS --- //
  
  const int RefCountMemoryManager::INITIAL_SIZE = 64;
  const int RefCountMemoryManager::GROWTH_RATE = 4;
  

// --- (DE)CONSTRUCTORS --- //

  
  /**
   * Constructor.
   */
  
  RefCountMemoryManager::RefCountMemoryManager(MasterMemoryManager* master,
      bool isStatic)
  {
    pthread_mutexattr_init(&_attribs);
    pthread_mutexattr_setpshared(&_attribs, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&_mutex, &_attribs);

    _size  = INITIAL_SIZE;
    _count = 0;
    _next  = 0;
    _counter = 0;
    _isStatic = isStatic;
    
    _table = new ObjectRecord[_size];
    for(int i = 0; i < _size; i++) {
      _table[i].ptr = 0;
    }

    _id = master->addManager(this, _table, _size);
    _master = master;
    _trace = false;
    _isClosed = false;
  }
  
  
  /**
   * Deconstructor.
   */
  
  RefCountMemoryManager::~RefCountMemoryManager() {
    pthread_mutex_destroy(&_mutex);
    pthread_mutexattr_destroy(&_attribs);
    
    LOG << "Terminating memory manager " << _id << ". Alive objects: "
      << getObjectCount() << OVER;

    if(_isStatic) {
      for(int i = _size - 1; i >= 0; i--) {
        if(NOT_NULL(_table[i].ptr)) {
          delete _table[i].ptr;
          _table[i].ptr = NULL;
          _count--;
        }
      }
    }

    _master->removeManager(_id);
    delete[] _table;
  }
  
  
// --- METHODS --- //
  
  void RefCountMemoryManager::grow() {
    int newSize = _size * GROWTH_RATE;
    ObjectRecord* newTable = new ObjectRecord[newSize];
    memset(newTable, 0, sizeof(ObjectRecord) * newSize);
    memcpy((void*)newTable, (void*)_table, sizeof(ObjectRecord) * _size);
    delete[] _table;
    
//    LOG << "RefCountMemoryManager " << _id << " resized from " << _size
//        << " to " << newSize << ENDS;

    _size = newSize;
    _table = newTable;
    _master->updateManager(_id, _table, _size);
  }
  
  
  void* RefCountMemoryManager::alloc(kf_int64_t s) {
    return malloc(s);
  }


  kf_uref_t RefCountMemoryManager::add(KFObject* obj, bool retain) {
    if(_isClosed) {
      throw MemoryException(K"Cannot register new object to a closed manager");
    }

    pthread_mutex_lock(&_mutex);

    int index = _next;
    kf_uref_t ref = (kf_uref_t){0, _id, index, rand() % 65530 - 32760};
    *(_table + index) = (ObjectRecord){ref, obj, RefBase::NULL_REF, retain?1:0, 0};

    _count++;

    if(_count == _size) {
      grow();
    }

    while(NOT_NULL(_table[_next].ptr)) {
      _next++;
      if(_next == _size) {
        _next = 0;
      }
    }

    pthread_mutex_unlock(&_mutex);

    return ref;
  }

  
  void RefCountMemoryManager::retain(const kf_int32_t index,
      const kf_int16_t key)
  {
    if(_isStatic) {
      return;
    }

    pthread_mutex_lock(&_mutex);
    ObjectRecord* record = _table + index;
    if(record->ref.key != key) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException(K"The pointer being retained is invalid: "
          + (kf_int32_t)_id + ':' + index);
    }

    record->retainCount++;
    pthread_mutex_unlock(&_mutex);
  }
  
  
  void RefCountMemoryManager::release(const kf_int32_t index,
      const kf_int16_t key)
  {
    if(_isStatic) {
      return;
    }

    bool doDelete = false;
    
    pthread_mutex_lock(&_mutex); // synchronized {
    ObjectRecord* record = _table + index;
    
    if(record->ref.key != key) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException(K"The pointer being released is invalid: "
          + (kf_int32_t)_id + ':' + index);
    }
    
    record->retainCount--;

    if(record->retainCount == 0) {
      doDelete = true;
    } else if(record->retainCount < 0) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException(K"Object is released too many times: "
          + (kf_int32_t)_id + ':' + index);
    }
    pthread_mutex_unlock(&_mutex); // } synchronized

    if(doDelete) {
      _count--;
      delete record->ptr;
      record->ptr = NULL;
    }
  }


  KFObject* RefCountMemoryManager::getObject(const kf_int32_t index,
      const kf_int16_t key)
  {
    ObjectRecord* record = _table + index;

    if(record->ref.key != key) {
      pthread_mutex_unlock(&_mutex);
      throw InvalidPointerException(K"Invalid object reference "
          + (kf_int32_t)_id + ':' + index);
    }

    return record->ptr;
  }


  kf_int16_t RefCountMemoryManager::getRetainCount(const kf_uref_t ref) const {
    return _table[ref.index].retainCount;
  }


  kf_int32_t RefCountMemoryManager::getObjectCount() const {
    return _count;
  }
  

  /**
   * Serializing method.
   */
  
  void RefCountMemoryManager::serialize(Ref<ObjectSerializer> seralizer) const
  {
    seralizer->object(K"RefCountMemoryManager")
        ->attribute(K"id", _id)
        ->attribute(K"count", _count);

    seralizer->member(K"records")->collection();

    int c = _size;
    for(int i = 0; i < c; i++) {
      const ObjectRecord& rec = _table[i];
      if(rec.ptr != NULL) {
        seralizer->object(K"ObjectRecord")
            ->attribute(K"type", System::demangle(typeid(*rec.ptr).name()))
            ->attribute(K"index", rec.ref.index)
            ->attribute(K"key", rec.ref.key)
            ->attribute(K"retainCount", rec.retainCount)
            ->endObject();
      }
    }

    seralizer->endCollection();
    seralizer->endObject();
  }
  
} // namespace kfoundation