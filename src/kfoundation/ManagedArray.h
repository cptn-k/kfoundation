//
//  ManagedArray.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/27/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
// --- (DE)CONSTRUCTORS --- //
  
  template<typename T>
  ManagedArray<T>::ManagedArray(kf_int32_t initialCapacity) {
    _size = 0;
    _capacity = initialCapacity;
    _data = new Ptr<T>[_capacity];
  }
  
  
  template<typename T>
  ManagedArray<T>::ManagedArray() {
    _size = 0;
    _capacity = KF_MAAGEDARRAY_INITIAL_CAPACITY;
    _data = new Ptr<T>[_capacity];
  }
  
  
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
  
  
  template<typename T>
  void ManagedArray<T>::push(PPtr<T> value) {
    if(_size == _capacity) {
      grow(_capacity * KF_MANAGEDARRAY_GROWTH_RATE);
    }
    
    _data[_size++] = value;
  }
  
  
  template<typename T>
  Ptr<T> ManagedArray<T>::pop() {
    if(_size == 0) {
      throw IndexOutOfBoundException("Can't pop because array is empty");
    }
    return _data[--_size] /* will be released by receiver */;
  }
  
  
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
  
  
  template<typename T>
  void ManagedArray<T>::clear() {
    for(int i = 0; i < _size; i++) {
      _data[i] = NULL;
    }
    _size = 0;
  }
  
  
  template<typename T>
  bool ManagedArray<T>::isEmpty() const {
    return _size == 0;
  }
  
  
  template<typename T>
  void ManagedArray<T>::setSize(kf_int32_t size) {
    if(size > _capacity) {
      grow(size);
    }
    _size = size;
  }
  
  
  template<typename T>
  inline kf_int32_t ManagedArray<T>::getSize() const {
    return _size;
  }
  
  
  template<typename T>
  inline Ptr<T>& ManagedArray<T>::at(const kf_int32_t index) {
    if(index >= _size || index < 0) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  template<typename T>
  kf_int32_t ManagedArray<T>::indexOf(PPtr<T> value) const {
    return indexOf(0, value);
  }
  
  
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
