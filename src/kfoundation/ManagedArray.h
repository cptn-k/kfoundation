/*---[ManagedArray.h]------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::ManagedArray::*
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
#include "ManagedObject.h"
#include "PtrDecl.h"
#include "SerializingStreamer.h"
#include "IndexOutOfBoundException.h"
#include "System.h"
#include "MemoryManager.h"
#include "Int.h"
#include "ObjectSerializer.h"
#include "Logger.h"

// Self
#include "ManagedArrayDecl.h"

#define KF_MAAGEDARRAY_INITIAL_CAPACITY 64
#define KF_MANAGEDARRAY_GROWTH_RATE 4


namespace kfoundation {
  
  using namespace std;

  /**
   * Flag returned by search methods when the desired item is not found.
   */
  
  template<typename T>
  const kf_int32_t ManagedArray<T>::NOT_FOUND = -1;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor, creates an empty new array with the given initial capacity.
   * The capacity will grow exponentially as needed.
   *
   * @param initialCapacity Initial capacity.
   */
  
  template<typename T>
  ManagedArray<T>::ManagedArray(kf_int32_t initialCapacity) {
    _size = 0;
    _capacity = initialCapacity;
    _data = new Ptr<T>[_capacity];
  }
  
  
  /**
   * Default constructor, creates an empty new array with default initial
   * capacity.
   */
  
  template<typename T>
  ManagedArray<T>::ManagedArray() {
    _size = 0;
    _capacity = KF_MAAGEDARRAY_INITIAL_CAPACITY;
    _data = new Ptr<T>[_capacity];
  }
  
  
  /**
   * Deconstructor. All elements will be released upon deconstruction.
   */
  
  template<typename T>
  ManagedArray<T>::~ManagedArray() {
    delete[] _data;
  }
  

// --- METHODS --- //
  
  template<typename T>
  void ManagedArray<T>::grow(kf_int32_t newCapacity) {
    _capacity = newCapacity;
    Ptr<T>* newData = new Ptr<T>[_capacity];
    for(int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    LOG << "ManagedArray resized to " << _capacity << EL;
    delete[] _data;
    _data = newData;
  }

  
  /**
   * Removes and releases the element at the given index.
   *
   * @param index The index of the element to be removed.
   */
  
  template<typename T>
  void ManagedArray<T>::remove(kf_int32_t index) {
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
  void ManagedArray<T>::push(PPtr<T> value) {
    if(_size == _capacity) {
      grow(_capacity * KF_MANAGEDARRAY_GROWTH_RATE);
    }
    
    _data[_size++] = value;
  }
  
  
  /**
   * Returns the pointer at highest index of the array, and decreases its size
   * by one. The pointer will not be released internally, it will be released
   * by the Ptr object returned.
   *
   * @return The popped pointer.
   * @throw Throws IndexOutOfBoundException if the array is empty.
   */
  
  template<typename T>
  Ptr<T> ManagedArray<T>::pop() {
    if(_size == 0) {
      throw IndexOutOfBoundException("Can't pop because array is empty");
    }
    return _data[--_size] /* will be released by receiver */;
  }
  
  
  /**
   * Inserts the given pointer to at the given index. Previous values at the
   * given index and above will be shifted to higher indexes.
   *
   * @param index The index to be inserted at.
   * @param value The pointer to be inserted.
   */
  
  template<typename T>
  void ManagedArray<T>::insert(kf_int32_t index, Ptr<T> value) {
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
  void ManagedArray<T>::clear() {
    for(int i = 0; i < _size; i++) {
      _data[i] = NULL;
    }
    _size = 0;
  }
  
  
  /**
   * Checks if the array is empty.
   */
  
  template<typename T>
  bool ManagedArray<T>::isEmpty() const {
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
  void ManagedArray<T>::setSize(kf_int32_t size) {
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
  inline kf_int32_t ManagedArray<T>::getSize() const {
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
  inline Ptr<T>& ManagedArray<T>::at(const kf_int32_t index) {
    if(index >= _size || index < 0) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  /**
   * Searches for the first occurance of the given pointer in the array.
   *
   * @note This method only compares pointers not the pointed values.
   * @param value The pointer to search for.
   * @return The index of the first occurance of the given pointer, or NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t ManagedArray<T>::indexOf(PPtr<T> value) const {
    return indexOf(0, value);
  }
  
  
  /**
   * Searches for the occurance of the first occurance of the given pointer
   * after the given offset in the array.
   * 
   * @note This method only compares pointers not the pointed values.
   * @param value The pointer to search for.
   * @return The index of the desired element, or NOT_FOUND.
   */
  
  template<typename T>
  kf_int32_t ManagedArray<T>::indexOf(const kf_int32_t offset, PPtr<T> value)
  const
  {
    for(kf_int32_t i = offset; i < _size; i++) {
      if(_data[i] == value) {
        return i;
      }
    }
    
    return NOT_FOUND;
  }
  
  
  template<typename T>
  void ManagedArray<T>::serialize(PPtr<ObjectSerializer> builder) const {
    builder->collection();
    size_t s = _size;
    for(int i = 0; i < s; i++) {
      ManagedObject* obj = _data[i].toPurePtr();
      if(INSTANCEOF(obj, SerializingStreamer)) {
        dynamic_cast<SerializingStreamer*>(obj)->serialize(builder);
      } else if(INSTANCEOF(obj, Streamer)) {
        builder->object("Item")
               ->attribute("value", dynamic_cast<Streamer*>(obj)->toString())
               ->endObject();
      } else if(obj == NULL) {
        builder->null()->endObject();
      } else {
        builder->object(System::demangle(typeid(T).name()))->endObject();
      }
    }
    builder->endCollection();
  }
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_MANAGED_ARRAY) */
