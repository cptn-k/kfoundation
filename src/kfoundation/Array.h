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
  
  template<typename T>
  const kf_int32_t Array<T>::NOT_FOUND = -1;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  template<typename T>
  Array<T>::Array()
  {
    _capacity = KF_ARRAY_INITIAL_CAPACITY;
    _size = 0;
    _data = new T[_capacity];
  }
  
  
  template<typename T>
  Array<T>::Array(T* values, kf_int32_t size) {
    _capacity = size;
    _size = size;
    _data = new T[_capacity];
    memcpy(_data, values, sizeof(T) * size);
  }
  
  
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
  

  template<typename T>
  void Array<T>::remove(const kf_int32_t index) {
    if(index < _size - 1) {
      for(int i = index; i < _size - 1; i++) {
        _data[i] = _data[i + 1];
      }
    }
    _size--;
  }
  
  
  template<typename T>
  T& Array<T>::push() {
    if(_size == _capacity) {
      grow(_capacity * KF_ARRAY_GROWTH_RATE);
    }
    
    _size++;
    
    return _data[_size - 1];
  }
  
  
  template<typename T>
  void Array<T>::push(const T& value) {
    push() = value;
  }
  
  
  template<typename T>
  T Array<T>::pop() {
    if(_size == 0) {
      throw IndexOutOfBoundException("Can't pop because array is empty");
    }
    return _data[--_size];
  }
  
  
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
  
  
  template<typename T>
  void Array<T>::insert(const kf_int32_t index, const T& value) {
    insert(index) = value;
  }
  
  
  template<typename T>
  void Array<T>::clear() {
    _size = 0;
  }
  
  
  template<typename T>
  bool Array<T>::isEmpty() const {
    return _size == 0;
  }
  
  
  template<typename T>
  void Array<T>::setSize(kf_int32_t size) {
    if(_capacity < size) {
      grow(size);
    }    
    _size = size;
  }
  
  
  template<typename T>
  inline kf_int32_t Array<T>::getSize() const {
    return _size;
  }
  
  
  template<typename T>
  inline T& Array<T>::at(const kf_int32_t index) {
    if(index >= _size) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  template<typename T>
  inline const T& Array<T>::at(const kf_int32_t index) const {
    if(index >= _size) {
      throw IndexOutOfBoundException("Attempt to access element "
          + Int::toString(index) + " of an array of size "
          + Int::toString(_size));
    }
    return _data[index];
  }
  
  
  template<typename T>
  bool Array<T>::contains(const T& value) const {
    for(kf_int32_t i = _size - 1; i >= 0; i--) {
      if(_data[i] == value) {
        return true;
      }
    }
    return false;
  }
  
  
  template<typename T>
  kf_int32_t Array<T>::indexOf(const T& value) const {
    return indexOf(0, value);
  }
  
  
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
