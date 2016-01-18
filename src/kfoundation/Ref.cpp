/*---[Ref.cpp]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::PtrBase::*
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

// Std
#include <cstring>

// Internal
#include "definitions.h"
#include "System.h"
#include "MasterMemoryManager.h"
#include "Int.h"
#include "LongInt.h"
#include "ObjectSerializer.h"
#include "StringPrintWriter.h"
#include "UString.h"
#include "StringPrintWriter.h"

// Self
#include "Ref.h"

namespace kfoundation {

  bool operator==(const kf_uref_t& first, const kf_uref_t& second) {
    return memcmp(&first, &second, sizeof(kf_uref_t)) == 0;
  }

//\/ PtrBase /\////////////////////////////////////////////////////////////////

  MasterMemoryManager* RefBase::master = NULL;
  MemoryManager* RefBase::defaultManager = NULL;
  MemoryManager::ObjectRecord** RefBase::masterTable = NULL;
  const kf_uref_t RefBase::NULL_REF = (kf_uref_t){0, 0, -1, 0};

  void RefBase::setMasterMemoryManager(MasterMemoryManager& m) {
    master = &m;
    masterTable = m.getMasterTable();
    defaultManager = &m.getDefaultManager();
  }

  
  /**
   * Returns the retain count for the pointed object. If this pointer is invaild
   * (or NULL) the returned value will be negative.
   */

  int RefBase::getRetainCount() const {
    if(_ref.index == -1) {
      return -1;
    }

    return master->getManagerAtIndex(_ref.manager).getRetainCount(_ref);
  }


  /**
   * Checks if this is a NULL-pointer.
   */

  bool RefBase::isNull() const {
    return _ref.index == -1;
  }


  /**
   * Checks if this is a valid pointed. A pointer is valid if it is not NULL and
   * it points to an existing object.
   */

  bool RefBase::isValid() const {
    if(_ref.index == -1) {
      return false;
    }

    return masterTable[_ref.manager][_ref.index].ref.key == _ref.key;
  }
  

  /**
   * Serializing method.
   */

  void RefBase::serialize(Ref<ObjectSerializer> builder) const {
    builder->object(K"Ref")
      ->attribute(K"type", getTypeName())
      ->attribute(K"managerIndex", (int)_ref.manager)
      ->attribute(K"obejctIndex", _ref.index)
      ->attribute(K"key", _ref.key)
      ->attribute(K"retainCount", getRetainCount());

    if(isValid()) {
      builder->attribute(K"memory",
          LongInt((kf_int64_t)masterTable[_ref.manager][_ref.index].ptr)
          .toHexString());
    }

    builder->endObject();
  }

  
  RefConst<UString> RefBase::toString() const {
    StringPrintWriter pw;
    pw << getTypeName() << "@[" << (kf_int8_t)_ref.manager << ':'
      << _ref.index << " +" << getRetainCount() << ']';
    return pw.toString();
  }
  

// --- OPERATORS --- //

  /**
   * Equality operator.
   */

  bool RefBase::operator==(const RefBase& other) const {
    return _ref == other._ref;
  }


  /**
   * Equality operator between a managed pointer and a classic pointer.
   */

  bool RefBase::operator==(const KFObject* const ptr) const {
    if(IS_NULL(ptr)) {
      return _ref.index == -1;
    }
    return ptr->getRef() == _ref;
  }


  /**
   * Inequality operator.
   */

  bool RefBase::operator!=(const RefBase& other) const {
    return !(_ref == other._ref);
  }


  /**
   * Inequality operator between a managed pointer and a class pointer.
   */

  bool RefBase::operator!=(const KFObject* const ptr) const {
    if(IS_NULL(ptr)) {
      return _ref.index != -1;
    }
    return !(ptr->getRef() == _ref);
  }

} // namespace kfoundation