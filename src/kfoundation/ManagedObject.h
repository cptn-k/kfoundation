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

#ifndef ORG_KNORBA_COMMON_MANAGEDOBJECT_H
#define ORG_KNORBA_COMMON_MANAGEDOBJECT_H

#include "definitions.h"
#include "PtrDecl.h"

namespace kfoundation {

//\/ ManagedObject /\//////////////////////////////////////////////////////////
  
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
  
  class PoolObject : public ManagedObject {
    public: PoolObject(kf_octet_t manager, kf_int32_t index);
    public: virtual void finalize() = 0;
  };

} // namespace kfoundation

#endif
