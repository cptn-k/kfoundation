/*---[RefCountMemoryManager.h]---------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::RefCountMemoryManager::*
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

#ifndef KFOUNDATION_REFCOUNTMEMORYMANAGER
#define KFOUNDATION_REFCOUNTMEMORYMANAGER

#include <pthread.h>

#include "MemoryManager.h"
#include "SerializingStreamer.h"
#include "KFObject.h"

namespace kfoundation {

  class UString;

  /**
   * Reference counting memory manager.
   *
   * @ingroup memory
   * @headerfile RefCountMemoryManager.h <kfoundation/RefCountMemoryManager.h>
   */
  
  class RefCountMemoryManager
  : public MemoryManager, public SerializingStreamer
  {
    
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
    private: bool _isStatic;


  // --- (DE)CONSTRUCOTRS --- //
    
    public: RefCountMemoryManager(MasterMemoryManager* master, bool isStatic = false);
    public: ~RefCountMemoryManager();
    
    
  // --- METHODS --- //
    
    private: void grow();

    // Inherited from MemoryManager
    public: kf_uref_t  add(KFObject* obj, bool retain);
    public: void*      alloc(kf_int64_t s);
    public: void       retain(const kf_int32_t index, const kf_int16_t key);
    public: void       release(const kf_int32_t index, const kf_int16_t key);
    public: KFObject*  getObject(const kf_int32_t index, const kf_int16_t key);
    public: kf_int16_t getRetainCount(const kf_uref_t ref) const;
    public: kf_int32_t getObjectCount() const;

    // Inherited from Serializing Streamer
    public: void serialize(Ref<ObjectSerializer> seralizer) const;
    
  };

} // namespace kfoundation

#endif /* defined(KFOUNDATION_REFCOUNTMEMORYMANAGER) */