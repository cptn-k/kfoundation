/*---[MemoryManager.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: MemoryManager
 |  Strcut: ObjectRecord
 |          Statistics
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

#ifndef KFOUNDATION_MEMORYMANAGER
#define KFOUNDATION_MEMORYMANAGER

#include <pthread.h>

#include "definitions.h"
#include "ManagedObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  struct ObjectRecord  {
    ManagedObject* ptr;
    kf_int16_t retainCount;
    kf_int16_t key;
    kf_int8_t  manager;
    kf_int16_t index;
    kf_int32_t serialNumber;
    bool       isStatic;
    bool       isBeingDeleted;
  };
  
  
  struct Statistics {
    kf_int32_t nObjects;
    kf_int32_t nStaticObjects;
  };

  
  class MemoryManager : public SerializingStreamer {
    public: virtual ~MemoryManager();
    public: virtual const ObjectRecord& registerObject(ManagedObject* obj) = 0;
    public: virtual void retain(kf_int32_t index, kf_int16_t key) = 0;
    public: virtual void release(kf_int32_t index, kf_int16_t key) = 0;
    public: virtual void remove(kf_int32_t index, kf_int16_t key) = 0;
    public: virtual ObjectRecord* getTable() = 0;
    public: virtual kf_int32_t getTableSize() const = 0;
    public: virtual void trace(const pthread_t theadId) = 0;
    public: virtual void untrace() = 0;
    public: virtual Statistics getStats() const = 0;
  };
  
} // namespace kfoundation

#endif

