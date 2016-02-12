/*---[KFObject.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::KFObject::*
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
#include "RefDecl.h"
#include "Comparable.h"

namespace kfoundation {

//\/ KFObject /\///////////////////////////////////////////////////////////////

  /**
   * The root class for all classes using KFoundation framework. 
   * Only subclasses of KFObject can be used with KFoundation references.
   *
   * @headerfile KFObject.h <kfoundation/KFObject.h>
   */
  
  class KFObject : public Comparable<KFObject> {

  // --- FIELDS --- //
    
    public: kf_uref_t _ref;


  // --- CONSTRUCTORS --- //

    public: KFObject();
    public: virtual ~KFObject();

    
  // --- METHODS --- //

    public: virtual kf_uref_t registerToManager(MemoryManager& manager) const;
    public: inline kf_uref_t getRef() const;

    private: void* internalAlloc(const void* original, kf_int64_t originalSize,
             kf_int64_t newSize) const;

    protected: template<typename T> T* alloc() const;
    protected: template<typename T> T* alloc(const kf_int64_t count) const;

    protected: template<typename T> T* alloc(const T* original,
               kf_int64_t originalCount, kf_int64_t count) const;

    protected: template<typename T> void free(const T* mem) const;

    // Inherited from Comparable<KFObject>
    public: bool equals(RefConst<KFObject> other) const;

  };


  inline kf_uref_t KFObject::getRef() const {
    return _ref;
  }


  template<typename T>
  T* KFObject::alloc() const {
    return internalAlloc(NULL, -1, sizeof(T));
  }


  template<typename T>
  T* KFObject::alloc(const kf_int64_t count) const {
    return internalAlloc(NULL, -1, sizeof(T) * count);
  }


  template<typename T>
  T* KFObject::alloc(const T* original, kf_int64_t originalCount,
      kf_int64_t newCount) const
  {
    return (T*)internalAlloc(original, originalCount * sizeof(T),
        newCount * sizeof(T));
  }


  template<typename T>
  void KFObject::free(const T* mem) const {
    delete mem;
  }

} // namespace kfoundation

#endif