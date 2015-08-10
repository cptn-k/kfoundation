/*---[ObjectPoolMemoryManagerDecl.h]---------------------------m(._.)m--------*\
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

#ifndef __KFoundation__ObjectPoolMemoryManager__
#define __KFoundation__ObjectPoolMemoryManager__

// Posix
#include <pthread.h>

// Internal
#include "SerializingStreamer.h"
#include "ManagedObject.h"

// Super
#include "MemoryManager.h"

namespace kfoundation {
  
  template<typename T>
  class PPtr;
  
  /**
   * Reuses the objects in a preallocated pool whenever a new instance is
   * needed. When an object is no longer needed, it will not be deleted, 
   * instead, it's PoolObject::finalize() method will be called to clean it
   * up for next use. When pool is full and a new instance is needed, 
   * the pool size will be automatically increased.
   *
   * Call get() method to obtain a clean instance to use.
   *
   * @ingroup memory
   */
  
  template<typename T>
  class ObjectPoolMemoryManager
  : public MemoryManager, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: int _size;
    private: int _growthRate;
    private: int _count;
    private: int _serialCounter;
    private: int _id;
    private: int _next;
    private: pthread_mutexattr_t _mutexAttrib;
    private: pthread_mutex_t _mutex;
    private: ObjectRecord* _table;
    private: MasterMemoryManager* _master;
    private: bool _trace;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ObjectPoolMemoryManager(const int initialCapacity,
        const int growthRate);
    
    public: ~ObjectPoolMemoryManager();
    
    
  // --- METHODS --- //
    
    private: void grow();
    private: string toString(int index);
    public: Ptr<T> get();
    
    // Inherited from MemoryManager
    public: const ObjectRecord& registerObject(ManagedObject* obj);
    public: void retain(kf_int32_t index, kf_int16_t key);
    public: void release(kf_int32_t index, kf_int16_t key);
    public: void remove(kf_int32_t index, kf_int16_t key);
    public: ObjectRecord* getTable();
    public: kf_int32_t getTableSize() const;
    public: Statistics getStats() const;
    public: void trace(const pthread_t threadId);
    public: void untrace();
    
    // Inherited from Serializing Streamer
    public: void serialize(PPtr<ObjectSerializer> seralizer) const;
    
  };
} // kfoundation


#endif /* defined(__KFoundation__ObjectPoolMemoryManager__) */
