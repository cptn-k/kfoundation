/*---[ManagedArrayDecl.h]--------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::ManagedArray::*
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

#ifndef KFOUNDATION_MANAGED_ARRAY_DECL
#define KFOUNDATION_MANAGED_ARRAY_DECL

#include <typeinfo>
#include "SerializingStreamer.h"

namespace kfoundation {
  
  using namespace std;
  
  /**
   * One-dimentional indexed collection of `ManagedObject`s. This class only
   * maintains pointers to the given objects. In contrast the Array class
   * maintains actual values. Items will be retained once added and released 
   * once removed. All objects will be released open deconstruction of this
   * class.
   *
   * @ingroup containers
   * @ingroup memory
   * @headerfile ManagedArray.h <kfoundation/ManagedArray.h>
   * @see Array
   */
  
  template<typename T>
  class ManagedArray : public ManagedObject, public SerializingStreamer {
    
  // --- NESTED TYPES --- //
    
    public: typedef Ptr< ManagedArray<T> > Ptr_t;
    public: typedef Ptr< ManagedArray<T> > PPtr_t;
    
    
  // --- STATIC FIELDS --- //
    
    public: static const kf_int32_t NOT_FOUND;
    
  
  // --- FIELDS --- //
    
    private: Ptr<T>* _data;
    private: kf_int32_t _size;
    private: kf_int32_t _capacity;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ManagedArray(kf_int32_t initialCapacity);
    public: ManagedArray();
    public: ~ManagedArray();
    
  
  // --- METHODS --- //
    
    private: void grow(kf_int32_t newCapacity);
    public: void remove(const kf_int32_t index);
    public: void push(PPtr<T> value);
    public: Ptr<T> pop();
    public: void insert(const kf_int32_t index, Ptr<T> value);
    public: void clear();
    public: bool isEmpty() const;
    public: void setSize(kf_int32_t size);
    public: inline kf_int32_t getSize() const;
    public: inline Ptr<T>& at(const kf_int32_t index);
    public: kf_int32_t indexOf(PPtr<T> value) const;
    public: kf_int32_t indexOf(kf_int32_t offset, PPtr<T> value) const;
        
    // From Srializing Streamer
    public: virtual void serialize(PPtr<ObjectSerializer> builder) const;
    
  }; // class Array
    
} // namespace kfoundation

#endif /* defined(KFOUNDATION_MANAGED_ARRAY_DECL) */
