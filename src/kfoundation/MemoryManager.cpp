/*---[MemoryManager.cpp]---------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::MemoryManager::~MemoryManager()
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

#include "Ref.h"
#include "System.h"
#include "ObjectSerializer.h"
#include "KFObject.h"

#include "MemoryManager.h"

namespace kfoundation {

  MemoryManager::~MemoryManager() {
    KF_NOP;
  }

  void MemoryManager::setObjectRef(KFObject *const obj,
      const kf_uref_t ref) const
  {
    obj->_ref = ref;
  }

} // namespace kfoundation

/**
 * @fn kfoundation::MemoryManager::add(KFObject* obj, bool retain)
 *
 * Creates a new entry for the object at the given memory location.
 *
 * @param obj - The object to manage
 * @param retain - If set true, the initial retain count of the object will be
 *     set to 1.
 */

/**
 * @fn kfoundation::MemoryManager::retain(kf_int32_t, kf_int16_t)
 *
 * Retains the object associated with the given index if key maches,
 * otherwise throws InvalidPointerException.
 */

/**
 * @fn kfoundation::MemoryManager::release(kf_int32_t, kf_int16_t)
 *
 * Releases the object associated with the given index if the key
 * matches, otherwise throws InvalidPointerException. If the retain 
 * count drops to zero the object will be deleted.
 */

/**
 * @fn kfoundation::MemoryManager::getObject(kf_int32_t, kf_int16_t)
 * 
 * Returns the object referenced by the given index, if the given key
 * matches the corresponding record.
 */

/**
 * @fn kfoundation::MemoryManager::getReatinCount(kf_int32_t, kf_int16_t)
 *
 * Returns the retain count of the object referenced by the given index, if the
 * given key matches the corresponding record.
 */

/**
 * @fn kfoundation::MemoryManager::getObjectCount()
 *
 * Returns the number of alive objects currently managed by this manager.
 */