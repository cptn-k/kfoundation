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

#include "System.h"
#include "ObjectSerializer.h"

#include "MemoryManager.h"

namespace kfoundation {
  
  /**
   * Deconstructor.
   */
  
  MemoryManager::~MemoryManager() {
    // Nothing;
  }
  
}

/**
 * @fn kfoundation::MemoryManager::registerObject(ManagedObject*)
 *
 * Creates a new entry for the object at the given memory location.
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
 * @fn kfoundation::MemoryManager::remove(kf_int32_t, kf_int16_t)
 * 
 * Unmanages the object at the given index if the key matches, 
 * otherwise, throws InvalidPointerException. The index will be
 * reused in the future.
 */

/**
 * @fn kfoundation::MemoryManager::getTable()
 * 
 * Returns the memory locatoin of the begining of this manager's object table.
 * The object table is a one dimentional array of ObjectRecord.
 *
 * @see getTableSize()
 */

/**
 * @fn kfoundation::MemoryManager::getTableSize()
 * 
 * Returns the number of available table records. Since some records can be 
 * unused, this is usually not equal but larger than the the number of objects
 * managed by this manager.
 *
 * @see getTable()
 */