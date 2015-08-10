/*---[RefCountMemoryManager.h]---------------------------------m(._.)m--------*\
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

#ifndef KFOUNDATION_REFCOUNTMEMORYMANAGER
#define KFOUNDATION_REFCOUNTMEMORYMANAGER

#include <pthread.h>

#include "MemoryManager.h"
#include "SerializingStreamer.h"
#include "ManagedObject.h"

namespace kfoundation {
  
  /**
   * Reference counting memory manager.
   *
   * @ingroup memory
   */
  
  class RefCountMemoryManager
  : public MemoryManager, public SerializingStreamer {
    
  // --- STATIC FIELDS --- //
    
    private: const static int INITIAL_SIZE;
    private: const static int GROWTH_RATE;
    
    
  // --- FIELDS --- //
    
    private: int _size;
    private: int _count;
    private: int _id;
    private: int _next;
    private: int _counter;
    private: pthread_mutex_t _mutex;
    private: pthread_mutexattr_t _attribs;
    private: ObjectRecord*   _table;
    private: MasterMemoryManager* _master;
    private: bool _trace;
    private: bool _isClosed;
    
  // --- (DE)CONSTRUCOTRS --- //
    
    public: RefCountMemoryManager(MasterMemoryManager* master);
    public: ~RefCountMemoryManager();
    
    
  // --- METHODS --- //
    
    private: void grow();
    private: string toString(int index);
    public: int migrate(MasterMemoryManager& other);
    
    // Inherited from MemoryManager
    public: const ObjectRecord& registerObject(ManagedObject* obj);
    public: void retain(kf_int32_t index, kf_int16_t key);
    public: void release(kf_int32_t index, kf_int16_t key);
    public: void remove(kf_int32_t index, kf_int16_t key);
    public: ObjectRecord* getTable();
    public: kf_int32_t getTableSize() const;
    public: void trace(const pthread_t threadId);
    public: void untrace();
    public: Statistics getStats() const;
    public: kf_int16_t update(kf_int16_t index, kf_int16_t key);
    public: void finalize();

    // Inherited from Serializing Streamer
    public: void serialize(PPtr<ObjectSerializer> seralizer) const;
    
  };
  
}


#endif /* defined(KFOUNDATION_REFCOUNTMEMORYMANAGER) */
