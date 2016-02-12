/*---[KFObject.h]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::KFObject::*
 |              kfoundation::PoolObject::*
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 */////////////////////////////////////////////////////////////////////////////

// Std
#include <cstring>
#include <cstdlib>

// Internal
#include "Ref.h"
#include "MasterMemoryManager.h"
#include "System.h"

// Self
#include "KFObject.h"

namespace kfoundation {

//\/ KFObject /\///////////////////////////////////////////////////////////////

// --- CONSTRUCTORS --- //

  KFObject::KFObject()
  : _ref(RefBase::NULL_REF)
  {
    // Nothing;
  }
  
  
  KFObject::~KFObject() {
    // Nothing
  }
  

// --- METHODS --- //

  kf_uref_t KFObject::registerToManager(MemoryManager &manager) const {
    if(NOT_NULL_REF(_ref)) {
      return _ref;
    }

    ((KFObject*)this)->_ref = manager.add((KFObject*)this, false);
    return _ref;
  }

  /**
   * Checks if this object is the same as the one reffered by the given
   * refrence.
   */

  bool KFObject::equals(RefConst<KFObject> other) const {
    return _ref == other.get();
  }


  void* KFObject::internalAlloc(const void *original, kf_int64_t originalSize, kf_int64_t newSize) const {
    void* mem  = malloc(newSize);

    if(NOT_NULL(original)) {
      memcpy(mem, original, originalSize);
    }

    return mem;
  }

} // namespace kfoundation