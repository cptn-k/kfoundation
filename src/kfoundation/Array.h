/*---[Array.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Array::*
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

#ifndef KFOUNDATION_ARRAY
#define KFOUNDATION_ARRAY

#include <vector>
#include <cstring>

#include "ManagedObject.h"
#include "PtrDecl.h"
#include "SerializingStreamer.h"
#include "IndexOutOfBoundException.h"
#include "System.h"
#include "MemoryManager.h"
#include "Int.h"
#include "ObjectSerializer.h"

#include "ArrayDecl.h"

#define KF_ARRAY_INITIAL_CAPACITY 64
#define KF_ARRAY_GROWTH_RATE 4


namespace kfoundation {
  
  using namespace std;
    
// --- STATIC FIELDS --- //
  
  /**
   * Flag returned by search methods when the desired item is not found.
   */
  
  template<typename T>
  const kf_int32_t Array<T>::NOT_FOUND = -1;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Default constructor.
   */
  
  template<typename T>
  Array<T>::Array()
  {
    _capacity = KF_ARRAY_INITIAL_CAPACITY;
    _size = 0;
    _data = new T[_capacity];
  }
  
  
  /**
   * Constructs a new Array copying the indicated number of items from the
   * given C-style array.
   *
   * @param values Location of the memory containing values to be copied.
   * @param size The number of items to be copied.
   */
  
  template<typename T>
  Array<T>::Array(T* values, kf_int32_t size) {
    _capacity = size;
    _size = size;
    _data = new T[_capacity];
    memcpy(_data, values, sizeof(T) * size);
  }
  
  
  /**
   * Deconstructor.
   */
  
  template<typename T>
  Array<T>::~Array() {
    delete[] _data;
  }
  

// --- METHODS --- //
  
  template<typename T>
  void Array<T>::grow(kf_int32_t newCapacity) {
    _capacity = newCapacity;
    T* newStorage = new T[_capacity];
    memcpy(newStorage, _data, sizeof(T) * _size);
    delete[] _data;
    _data = newStorage;
  }

  
  /**
   * Removes the item at the given index. If there are items at higher indexes,
   * they will be shifted down by one.
   */

  template<typename T>
  void Array<T>::remove(const kf_int32_t index) {
    if(index < _size - 1) {
      for(int i = index; i < _size - 1; i++) {
        _data[i] = _data[i + 1];
      }
    }
    _size--;
  }
  
  
  /**
   * Expands the array by one and returns the reference to the newly added
   * element. Usage:
   *
   *      array->push() = value;
   *
   */
  
  template<typename T>
  T& Array<T>::push() {
    if(_size == _capacity) {
      grow(_capacity * KF_ARRAY_GROWTH_RATE);
    }
    
    _size++;
    
    return _data[_size - 1];
  }
  
  
  /**
   * Expands the array by one and sets the newly added item to the given value.
   *
   * @param value Value to be pushed.
   */
  
  template<typename T>
  void Array<T>::push(const T& value) {
    push() = value;
  }
  
  
  /**
   * Returns the value of the last element in the array, and decreases its size
   * by one.
   *
   * @throw Throws IndexOutOfBoundException if the array is empty.
   */
  
  template<typename T>
  T Array<T>::pop() {
    if(_size == 0) {
      throw IndexOutOfBoundException("Can't pop because array is empty");
    }
    return _data[--_size];
  }
  
  
  /**
   * Used to insert a new element. All elements at the given index and above
   * will be shifted one index higher. Usage:
   * 
   *      array->insert(index) = value;
   *
   * @param index The index to insert at.
   */
  
  template<typename T>
  T& Array<T>::insert(const kf_int32_t index) {
    if(_size == _capacity) {
      grow(_capacity * KF_ARRAY_GROWTH_RATE);
    }
    
    for(int i = _size - 1; i >= index; i--) {
      _data[i + 1] = _data[i];
    }
    
    _size++;
    
    return _data[index];
  }
  
  
  /**
   * Used to insert the given value at the given index. All the existing
   * at the index and higher will be shifted one index higher.
   *
   * @param index The index to insert at.
   * @param value The value to be inserted.
   */
  
  template<typename T>
  void Array<T>::insert(const kf_int32_t index, const T& value) {
    insert(index) = value;
  }
  
  
  /**
   * Resets the size of this array to zero.
   */
  
  template<typename T>
  void Array<T>::clear() {
    _size = 0;
  }
  
  
  /**
   * Checks if this array is empty.
   */
  
  template<typename T>
  bool Array<T>::isEmpty() const {
    return _size == 0;
  }
  
  
  /**
   * Adjusts the size of array to the given value.
   */
  
  template<typename T>
  void Array<T>::setSize(kf_int32_t size) {
    if(_capacity < size) {
      grow(size);
    }    
    _size = size;
  }
  
  
  /**
   * Returns the number of elements in this array.
   */
  
  template<typename T>
  inline kf_int32_t Array<T>::getSize() const {
    return _size;
  }
  
  
  /**
   * Returns a reference to the value at the given index.
   *
   * @param index The index of the item to be accessed.
   * @throw Throws IndexOutOfBoundsException if requested index is bigger or
   *        equal the size of the array.
   */
  
  template<typename T>
  inline T& Array<T>::at(const kf_int32_t index) {
    if(index >= _size) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  /**
   * Returns a reference to the value at the given index.
   *
   * @param index The index of the item to be accessed.
   * @throw Throws IndexOutOfBoundsException if requested index is bigger or
   *        equal the size of the array.
   */
  
  template<typename T>
  inline const T& Array<T>::at(const kf_int32_t index) const {
    if(index >= _size) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  /**
   * Checks if this array contains the given value.
   *
   * @param value The value to search for.
   */
  
  template<typename T>
  bool Array<T>::contains(const T& value) const {
    for(kf_int32_t i = _size - 1; i >= 0; i--) {
      if(_data[i] == value) {
        return true;
      }
    }
    return false;
  }
  
  
  /**
   * Used to search for a value in the array.
   *
   * @param value The value to search for.
   * @return If found, the index of the first occurance of the given value,
   *         otherwise, NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t Array<T>::indexOf(const T& value) const {
    return indexOf(0, value);
  }
  
  
  /**
   * Searchs for the occurance of the given value starting from the given offset.
   *
   * @param offset The index to start the search from.
   * @param value The value to search for.
   * @return If found, the index of the first occurance of the given value,
   *         otherwise, NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t Array<T>::indexOf(const kf_int32_t offset, const T& value) const
  {
    for(kf_int32_t i = offset; i < _size; i++) {
      if(_data[i] == value) {
        return i;
      }
    }
    
    return NOT_FOUND;
  }
  
} // namespace kfoundation

#endif /* KFOUNDATION_ARRAY */
