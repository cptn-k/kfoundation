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

// --- STATIC FIELDS --- //

  kf_uref_t KFObject::registerAndRetain(MemoryManager &manager) const {
    if(NOT_NULL_REF(_ref)) {
      manager.retain(_ref.index, _ref.key);
      return _ref;
    }

    ((KFObject*)this)->_ref = manager.add((KFObject*)this, true);
    return _ref;
  }


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