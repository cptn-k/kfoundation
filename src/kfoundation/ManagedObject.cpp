/*---[ManagedObject.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: ManagedObject
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

// Internal
#include "Ptr.h"
#include "MasterMemoryManager.h"
#include "System.h"

// Self
#include "ManagedObject.h"

namespace kfoundation {

//\/ ManagedObject /\//////////////////////////////////////////////////////////
  
  ManagedObject::ManagedObject()
  : _ptr(registerPtr())
  {
    // Nothing;
  }
  
  
  ManagedObject::ManagedObject(kf_octet_t manager, kf_int32_t index)
  : _ptr(manager, index)
  {
    // Nothing;
  }

  
  ManagedObject::~ManagedObject() {
    _ptr.unmanage();
  }
  
  
  Ptr<ManagedObject> ManagedObject::registerPtr() {
    const ObjectRecord& rec =
        System::getMasterMemoryManager().registerObject(this);
    return Ptr<ManagedObject>(rec.manager, rec.index);
  }
  
  
  PPtr<ManagedObject> ManagedObject::getPtr() const {
    return _ptr;
  }
  
  
//\/ PoolObject /\/////////////////////////////////////////////////////////////
  
  PoolObject::PoolObject(kf_octet_t manager, kf_int32_t index)
  : ManagedObject(manager, index)
  {
    // Nothing;
  }
  
} // namespace kfoundation
