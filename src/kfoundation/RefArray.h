/*---[RefArray.h]------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::RefArray::*
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

#ifndef KFOUNDATION_MANAGED_ARRAY
#define KFOUNDATION_MANAGED_ARRAY

// Internal
#include "KFObject.h"
#include "Ref.h"
#include "SerializingStreamer.h"
#include "IndexOutOfBoundException.h"
#include "System.h"
#include "MemoryManager.h"
#include "Int32.h"
#include "ObjectSerializer.h"
#include "UString.h"

// Self
#include "RefArrayDecl.h"

#define KF_MAAGEDARRAY_INITIAL_CAPACITY 64
#define KF_MANAGEDARRAY_GROWTH_RATE 4


namespace kfoundation {
  
//\/ ManagedArrayIterator /\///////////////////////////////////////////////////

  template<typename T>
  RefArrayBase<T>::Iterator::Iterator(RefConst< RefArrayBase<T> > source,
      T* first, kf_int32_t size)
  : _source(source),
    _first(first),
    _last(first + size),
    _pos(first)
  {
    ((T*)this)->operator=(*_pos);
  }


  template<typename T>
  void RefArrayBase<T>::Iterator::first() {
    _pos = _first;
    ((T*)this)->operator=(*_pos);
  }


  template<typename T>
  void RefArrayBase<T>::Iterator::next() {
    _pos++;
    ((T*)this)->operator=(*_pos);
  }


  template<typename T>
  bool RefArrayBase<T>::Iterator::hasMore() const {
    return _pos < _last;
  }
  
  
  template<typename T>
  kf_int32_t RefArrayBase<T>::Iterator::getIndex() const {
    return (kf_int32_t)(_pos - _first);
  }


//\/ RefArrayBase /\///////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor, creates an empty new array with the given initial capacity.
   * The capacity will grow exponentially as needed.
   *
   * @param initialCapacity Initial capacity.
   */
  
  template<typename T>
  RefArrayBase<T>::RefArrayBase(kf_int32_t size) {
    _size = size;
    _capacity = size;
    _data = new T[_capacity];
  }
  
  
  /**
   * Default constructor, creates an empty new array with default initial
   * capacity.
   */
  
  template<typename T>
  RefArrayBase<T>::RefArrayBase() {
    _size = 0;
    _capacity = KF_MAAGEDARRAY_INITIAL_CAPACITY;
    _data = new T[_capacity];
  }
  
  
  /**
   * Deconstructor. All elements will be released upon deconstruction.
   */
  
  template<typename T>
  RefArrayBase<T>::~RefArrayBase() {
    delete[] _data;
  }
  

// --- METHODS --- //
  
  template<typename T>
  void RefArrayBase<T>::grow(kf_int32_t newCapacity) {
    _capacity = newCapacity;
    T* newData = new T[_capacity];
    for(int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
  }

  
  /**
   * Removes and releases the element at the given index.
   *
   * @param index The index of the element to be removed.
   */
  
  template<typename T>
  void RefArrayBase<T>::remove(kf_int32_t index) {
    if(index < _size - 1) {
      for(int i = index; i < _size - 1; i++) {
        _data[i] = _data[i + 1];
      }
    }
    _size--;
    _data[_size] = NULL;
  }
  
  
  /**
   * Adds the given pointer to the end of the array and retains it.
   *
   * @param value The pointer to be pushed.
   */
  
  template<typename T>
  void RefArrayBase<T>::push(T value) {
    if(_size == _capacity) {
      grow(_capacity * KF_MANAGEDARRAY_GROWTH_RATE);
    }
    
    _data[_size++] = value;
  }
  
  
  /**
   * Returns the pointer at highest index of the array, and decreases its size
   * by one. The pointer will not be released internally, it will be released
   * by the Ref object returned.
   *
   * @return The popped pointer.
   * @throw Throws IndexOutOfBoundException if the array is empty.
   */
  
  template<typename T>
  T RefArrayBase<T>::pop() {
    if(_size == 0) {
      throw IndexOutOfBoundException(K"Can't pop because array is empty");
    }
    T d = _data[_size - 1];
    _data[_size - 1] = NULL;
    _size--;
    return d;
  }
  
  
  /**
   * Inserts the given pointer to at the given index. Previous values at the
   * given index and above will be shifted to higher indexes.
   *
   * @param index The index to be inserted at.
   * @param value The pointer to be inserted.
   */
  
  template<typename T>
  void RefArrayBase<T>::insert(kf_int32_t index, T value) {
    if(_size == _capacity) {
      grow(_capacity * KF_MANAGEDARRAY_GROWTH_RATE);
    }
    
    for(int i = _size - 1; i >= index; i--) {
      _data[i + 1] = _data[i];
    }
    
    _data[index] = value;
    
    _size++;
  }
  
  
  /**
   * Resets the size of the array to zero and releases all existing elements.
   */
  
  template<typename T>
  void RefArrayBase<T>::clear() {
    for(int i = 0; i < _size; i++) {
      _data[i] = NULL;
    }
    _size = 0;
  }
  
  
  /**
   * Checks if the array is empty.
   */
  
  template<typename T>
  bool RefArrayBase<T>::isEmpty() const {
    return _size == 0;
  }
  
  
  /**
   * Sets the size of the array as specified. If the new size is larger than 
   * previous one the added elements are set to NULL. If the new size is smaller
   * the removed elements are released. If the request size is larger than
   * the capacity, the capacity will grow as needed.
   *
   * @param size The array's new size.
   */
  
  template<typename T>
  void RefArrayBase<T>::setSize(kf_int32_t size) {
    if(size > _capacity) {
      grow(size);
    }
    
    for(int i = size; i < _size; i++) {
      _data[i] = NULL;
    }
    
    for(int i = _size; i < size; i++) {
      _data[i] = NULL;
    }
    
    _size = size;
  }
  
  
  /**
   * Returns the size of the array.
   */
  
  template<typename T>
  inline kf_int32_t RefArrayBase<T>::getSize() const {
    return _size;
  }
  
  
  /**
   * Returns reference to the pointer at the given index of the array.
   *
   * @param index The index of the element to be accessed.
   * @throw Throws IndexOutOfBoundException if the requested index is bigger or
   *        equal the size of the array.
   */
  
  template<typename T>
  inline T& RefArrayBase<T>::at(const kf_int32_t index) const {
    if(index >= _size || index < 0) {
      throw IndexOutOfBoundException(K"Attempt to access element "
          + index + " of an array of size "
          + _size);
    }
    return _data[index];
  }


  template<typename T>
  inline T& RefArrayBase<T>::first() {
    if(_size == 0) {
      throw IndexOutOfBoundException(K"Attempt to acees the first element of"
          " an empty array.");
    }

    return _data[0];
  }


  template<typename T>
  inline T& RefArrayBase<T>::last() {
    if(_size == 0) {
      throw IndexOutOfBoundException(K"Attempt to access the last element of"
          " an empty array.");
    }

    return _data[_size - 1];
  }
  
  
  /**
   * Searches for the first occurance of the given pointer in the array.
   *
   * @note This method only compares pointers not the pointed values.
   * @param value The pointer to search for.
   * @return The index of the first occurance of the given pointer, or KF_NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t RefArrayBase<T>::indexOf(T value) const {
    return indexOf(0, value);
  }
  
  
  /**
   * Searches for the occurance of the first occurance of the given pointer
   * after the given offset in the array.
   * 
   * @note This method only compares pointers not the pointed values.
   * @param value The pointer to search for.
   * @return The index of the desired element, or KF_NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t RefArrayBase<T>::indexOf(const kf_int32_t offset, T value)
  const
  {
    for(kf_int32_t i = offset; i < _size; i++) {
      if(_data[i] == value) {
        return i;
      }
    }
    
    return KF_NOT_FOUND;
  }


  template<typename T>
  typename RefArrayBase<T>::Iterator RefArrayBase<T>::getIterator() const {
    return Iterator(this, _data, _size);
  }
  
  
  template<typename T>
  void RefArrayBase<T>::serialize(Ref<ObjectSerializer> builder) const {
    builder->collection();
    size_t s = _size;
    for(int i = 0; i < s; i++) {
      T d = _data[i];
      if(d.isNull()) {
        builder->null();
      } else if(d.template ISA(SerializingStreamer)) {
        builder->object(dynamic_cast<const SerializingStreamer&>(*d));
      } else if(d.template ISA(Streamer)) {
        const Streamer& streamer = dynamic_cast<const Streamer&>(*d);
        builder->object(d.getTypeName())
          ->attribute(K"value", streamer.toString())
          ->endObject();
      } else {
        builder->object(d.getTypeName())->endObject();
      }
    }
    builder->endCollection();
  }


//\/ RefArray /\///////////////////////////////////////////////////////////////

  template<typename T>
  RefArray<T>::RefArray(kf_int32_t size)
  : RefArrayBase< Ref<T> >(size)
  {
    // Nothing;
  }


  template<typename T>
  RefArray<T>::RefArray()
  : RefArrayBase< Ref<T> >()
  {
    // Nothing;
  }



//\/ RefConstArray /\//////////////////////////////////////////////////////////

  template<typename T>
  RefConstArray<T>::RefConstArray(kf_int32_t size)
  :RefArrayBase< RefConst<T> >(size)
  {
    // Nothing
  }


  template<typename T>
  RefConstArray<T>::RefConstArray()
  :RefArrayBase< RefConst<T> >()
  {
    // Nothing
  }

} // namespace kfoundation

#endif /* defined(KFOUNDATION_MANAGED_ARRAY) */