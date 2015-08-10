/*---[ManagedObject.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::ManagedObject::*
 |              kfoundation::PoolObject::*
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

#ifndef ORG_KNORBA_COMMON_MANAGEDOBJECT_H
#define ORG_KNORBA_COMMON_MANAGEDOBJECT_H

#include "definitions.h"
#include "PtrDecl.h"

namespace kfoundation {

//\/ ManagedObject /\//////////////////////////////////////////////////////////
  
  /**
   * The root class for all classes using KFoundation framework. 
   * Only subclasses of ManagedObject can be accessed via managed pointers.
   *
   * @headerfile ManagedObject.h <kfoundation/ManagedObject.h>
   */
  
  class ManagedObject {
    
  // --- FIELDS --- //
    
    private: Ptr<ManagedObject> _ptr;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ManagedObject();
    public: ManagedObject(kf_octet_t manager, kf_int32_t index);
    public: virtual ~ManagedObject();
    
    
  // --- METHODS --- //
    
    private: Ptr<ManagedObject> registerPtr();
    public: PPtr<ManagedObject> getPtr() const;
    
  };
  
  
//\/ PoolObject /\/////////////////////////////////////////////////////////////
  
  
  /**
   * Superclass for all objects that are meant to be allocated by a pool memory
   * manager. These objects will never be destructed. Instead, they should
   * implement the finalize() method to cleanup themselves after each use.
   *
   * @headerfile ManagedObject.h <kfoundation/ManagedObject.h>
   */
  
  class PoolObject : public ManagedObject {
    public: PoolObject(kf_octet_t manager, kf_int32_t index);
    public: virtual void finalize() = 0;
  };

} // namespace kfoundation

#endif
