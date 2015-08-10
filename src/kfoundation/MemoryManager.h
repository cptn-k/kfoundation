/*---[MemoryManager.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : struct kfoundation::ObjectRecord
 |              struct kfoundation::Statistics
 |              class kfoundation::MemoryManager
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

#ifndef KFOUNDATION_MEMORYMANAGER
#define KFOUNDATION_MEMORYMANAGER

#include <pthread.h>

#include "definitions.h"
#include "ManagedObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  /**
   * Structure of memory manager's table records.
   */
  
  struct ObjectRecord  {
    ManagedObject* ptr;        ///< Memory location of the target object
    kf_int16_t retainCount;    ///< Retain count
    kf_int16_t key;            ///< Key
    kf_int8_t  manager;        ///< The ID of the manager owning this table
    kf_int16_t index;          ///< Index of this record
    kf_int32_t serialNumber;   ///< Unique serial number for this object
    bool       isStatic;       ///< `true' if the object is static
    bool       isBeingDeleted; ///< Flag for internal use
  };
  
  
  struct Statistics {
    kf_int32_t nObjects;
    kf_int32_t nStaticObjects;
  };

  
  /**
   * Abstract interface to be implemented by all memory managers.
   * @ingroup memory
   */
  
  class MemoryManager {
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

