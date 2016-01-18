/*---[ArrayDecl.h]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::Array::*
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

#ifndef KFOUNDATION_ARRAY_DECL
#define KFOUNDATION_ARRAY_DECL

#include "KFObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {

  /**
   * A resizable, one-dimensional indexed container. This class is not designed
   * to contain
   * managed objects. For managed objects, use RefArray.
   *
   * @ingroup containers
   * @see RefArray
   * @headerfile Array.h <kfoundation/Array.h>
   */
  
  template<typename T>
  class Array : public KFObject {
    
  // --- NESTED TYPES --- //
    
    public: typedef Ref< Array<T> > Ref_t;
    public: typedef RefConst< Array<T> > RefConst_t;

    public: class Iterator {

    // --- FIELDS --- //

      private: RefConst< Array<T> > _source;
      private: T* _begin;
      private: T* _end;
      private: T* _p;


    // --- CONSTRUCTOR --- //

      public: Iterator(RefConst< Array<T> > owner, T* begin);

      public: T& first();
      public: T& next();
      public: T& get() const;
      public: kf_int32_t getIndex() const;
      public: bool hasMore() const;
      
    };
    
    
  // --- FIELDS --- //
    
    private: T*         _data;
    private: kf_int32_t _size;
    private: kf_int32_t _capacity;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Array();
    public: Array(const kf_int32_t size);
    public: Array(T*, kf_int32_t size);
    public: ~Array();
    
    
  // --- METHODS --- //
    
    private: void grow(kf_int32_t newCapacity);
    public:  void consolidate();
    public:  void remove(const kf_int32_t index);
    public:  void push(const T& value);
    public:  T& push();
    public:  T pop();
    public:  T& insert(const kf_int32_t index);
    public:  void insert(const kf_int32_t index, const T& value);
    public:  void clear();
    public:  bool isEmpty() const;
    public:  void setSize(kf_int32_t size);
    public:  inline kf_int32_t getSize() const;
    public:  inline T& at(const kf_int32_t index);
    public:  inline const T& at(const kf_int32_t index) const;
    public:  bool contains(const T& value) const;
    public:  kf_int32_t indexOf(const T& value) const;
    public:  kf_int32_t indexOf(const kf_int32_t offset, const T& value) const;
    public:  Iterator getIterator() const;
    
  }; // class Array
  
} // namespace kfoundation

#endif /* KFOUNDATION_ARRAY_DECL */