/*---[ManagedArrayDecl.h]--------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::RefArray::*
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

// Internal
#include "KFObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  /**
   * One-dimentional indexed collection of `KFObject`s. This class only
   * maintains pointers to the given objects. In contrast the Array class
   * maintains actual values. Items will be retained once added and released 
   * once removed. All objects will be released open deconstruction of this
   * class.
   *
   * @ingroup containers
   * @ingroup memory
   * @headerfile RefArray.h <kfoundation/RefArray.h>
   * @see Array
   */
  
  template<typename T>
  class RefArrayBase : public KFObject, public SerializingStreamer {
    
  // --- NESTED TYPES --- //
    
    public: class Iterator : public T {

    // --- FIELDS --- //

      private: RefConst< RefArrayBase<T> > _source;
      private: T* _first;
      private: T* _last;
      private: T* _pos;


    // --- CONSTRUCTOR --- //

      public: Iterator(RefConst< RefArrayBase<T> > source, T* first,
          kf_int32_t size);


    // --- FIELDS --- //

      public: void first();
      public: void next();
      public: bool hasMore() const;
      public: kf_int32_t getIndex() const;
      
    };

  
  // --- FIELDS --- //
    
    private: T* _data;
    private: kf_int32_t _size;
    private: kf_int32_t _capacity;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: RefArrayBase(kf_int32_t initialCapacity);
    public: RefArrayBase();
    public: ~RefArrayBase();

  
  // --- METHODS --- //
    
    private: void grow(kf_int32_t newCapacity);
    public: void remove(const kf_int32_t index);
    public: void push(T value);
    public: T pop();
    public: void insert(const kf_int32_t index, T value);
    public: void clear();
    public: bool isEmpty() const;
    public: void setSize(kf_int32_t size);
    public: inline kf_int32_t getSize() const;
    public: inline T& at(const kf_int32_t index) const;
    public: inline T& first();
    public: inline T& last();
    public: kf_int32_t indexOf(T value) const;
    public: kf_int32_t indexOf(kf_int32_t offset, T value) const;
    public: Iterator getIterator() const;

    // From Srializing Streamer
    public: virtual void serialize(Ref<ObjectSerializer> builder) const;
    
  }; // class Array


  template<typename T>
  class RefArray : public RefArrayBase< Ref<T> > {
    public: RefArray(kf_int32_t size);
    public: RefArray();
  };


  template<typename T>
  class RefConstArray : public RefArrayBase< RefConst<T> > {
    public: RefConstArray(kf_int32_t size);
    public: RefConstArray();
  };

} // namespace kfoundation

#endif /* defined(KFOUNDATION_MANAGED_ARRAY_DECL) */