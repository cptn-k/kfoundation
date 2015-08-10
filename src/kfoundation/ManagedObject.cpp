/*---[ManagedObject.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::ManagedObject::*
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
 *//////////////////////////////////////////////////////////////////////////////

// Internal
#include "Ptr.h"
#include "MasterMemoryManager.h"
#include "System.h"

// Self
#include "ManagedObject.h"

namespace kfoundation {

//\/ ManagedObject /\//////////////////////////////////////////////////////////
  
  /**
   * Default constructor.
   */
  
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

  
  /**
   * Deconstructor. Upon distruction makes sure this object is undregistered
   * from its corresponding memory manager.
   */
  
  ManagedObject::~ManagedObject() {
    _ptr.unmanage();
  }
  
  
  Ptr<ManagedObject> ManagedObject::registerPtr() {
    const ObjectRecord& rec =
        System::getMasterMemoryManager().registerObject(this);
    return Ptr<ManagedObject>(rec.manager, rec.index);
  }
  
  
  /**
   * Returns a managed poitner to this object.
   */
  
  PPtr<ManagedObject> ManagedObject::getPtr() const {
    return _ptr;
  }
  
  
//\/ PoolObject /\/////////////////////////////////////////////////////////////
  
  PoolObject::PoolObject(kf_octet_t manager, kf_int32_t index)
  : ManagedObject(manager, index)
  {
    // Nothing;
  }
  
  /**
   * @fn kfoundation::PoolObject::finalize()
   * Cleansup this object after each use. This method should be implemented by
   * subclasses in place of destructor.
   */
  
} // namespace kfoundation
