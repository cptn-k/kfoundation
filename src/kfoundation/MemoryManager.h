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

#include "definitions.h"

namespace kfoundation {

  class KFObject;

  /**
   * Abstract interface to be implemented by all memory managers.
   *
   * @ingroup memory
   * @headerfile MemoryManager.h <kfoundation/MemoryManager.h>
   */
  
  class MemoryManager {
    public: struct ObjectRecord  {
      kf_uref_t      ref;
      KFObject*      ptr;            ///< Memory location of the target object
      kf_uref_t      owner;          ///< Owner
      kf_int16_t     retainCount;    ///< Retain count
      kf_int64_t     size;           ///< Size
    };

    protected: void setObjectRef(KFObject* const obj, const kf_uref_t ref) const;
    public: virtual ~MemoryManager();
    public: virtual kf_uref_t  add(KFObject* obj, bool retain) = 0;
    public: virtual void*      alloc(kf_int64_t s) = 0;
    public: virtual void       retain(const kf_int32_t index, const kf_int16_t key) = 0;
    public: virtual void       release(const kf_int32_t index, const kf_int16_t key) = 0;
    public: virtual KFObject*  getObject(const kf_int32_t index, const kf_int16_t key) = 0;
    public: virtual kf_int16_t getRetainCount(const kf_uref_t ref) const = 0;
    public: virtual kf_int32_t getObjectCount() const = 0;
  };

} // namespace kfoundation

#endif