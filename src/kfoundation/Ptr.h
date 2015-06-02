/*---[Ptr.h]---------------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: Ptr
 |         PPtr
 |         SPtr
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

#ifndef KFOUNDATION_PTR_H
#define KFOUNDATION_PTR_H

// Std
#include <typeinfo>

// Internal
#include "ObjectSerializer.h"
#include "NullPointerException.h"
#include "MasterMemoryManager.h"
#include "MemoryManager.h"
#include "Logger.h"
#include "ManagedObject.h"
#include "System.h"

// Self
#include "PtrDecl.h"

namespace kfoundation {

  
//\/ Ptr /\////////////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  template<typename T>
  inline Ptr<T>::Ptr() {
    _locator.objectIndex = -1;
    _locator.autorelease = true;
    _locator.trace = false;
    #ifdef DEBUG
    _serial = serial++;
    _obj = NULL;
    #endif
  }
  
  
  template<typename T>
  Ptr<T>::Ptr(T* obj, bool trace) {
    #ifdef DEBUG
    _obj = obj;
    _serial = serial++;
    #endif
    
    if(!obj) {
      _locator.objectIndex = -1;
    } else {
      if(!master) {
        throw KFException("Master manager is not initialized.");
      }      
      _locator = obj->getPtr().getLocator();
    }
    
    _locator.autorelease = true;
    _locator.trace = trace;
    
    #ifdef DEBUG
    if(_locator.trace) {
      printEvent("created");
    }
    #endif
  }
  
  
  template<typename T>
  Ptr<T>::Ptr(const kf_int8_t managerIndex, const kf_int32_t objectIndex) {
    ObjectRecord* record = *(objectTable + managerIndex) + objectIndex;
    _locator.objectIndex = objectIndex;
    _locator.key = record->key;
    _locator.managerIndex = managerIndex;
    _locator.trace = false;
    _locator.autorelease = false;

    #ifdef DEBUG
    _obj = record->ptr;
    _serial = serial++;
    #endif
  }
  
  
  template<typename T>
  inline Ptr<T>::Ptr(const Ptr<T>& other) {
    #ifdef DEBUG
    _obj = other._obj;
    _serial = serial++;
    #endif
    
    _locator = other._locator;
    _locator.autorelease = true;
    
    #ifdef DEBUG
    if(_locator.trace) {
      printEvent("copied");
    }
    #endif
  }

  
  template<typename T>
  Ptr<T>::~Ptr() {
    if(_locator.autorelease) {
      #ifdef DEBUG
      if(_locator.trace) {
        printEvent("autoreleased", false);
      }
      #endif
      
      release();
    }
  }
  
  
// --- METHODS --- //
  
#ifdef DEBUG
  template<typename T>
  Ptr<T>& Ptr<T>::trace() {
    cout << "Tracing: " << *this << endl;
    _locator.trace = true;
    return *this;
  }
  
  
  template<typename T>
  Ptr<T>& Ptr<T>::untrace() {
    cout << "Untracing: " << *this << endl;
    _locator.trace = false;
    return *this;
  }
  
  
  template<typename T>
  Ptr<T>& Ptr<T>::del() {
    if(_locator.autorelease) {
      #ifdef DEBUG
      if(_locator.trace) {
        printEvent("manual resleased");
      }
      #endif
      
      _locator.autorelease = false;
      
      release();
    }
    return *this;
  }
#endif
  
  
  template<typename T>
  Ptr<T>& Ptr<T>::retain() {
    if(_locator.objectIndex == -1) {
      return *this;
    }
    
    master->getManagerAtIndex(_locator.managerIndex)
          ->retain(_locator.objectIndex, _locator.key);
    
    #ifdef DEBUG
    if(_locator.trace) {
      printEvent("retained");
    }
    #endif
    
    return *this;
  }

  
  template<typename T>
  Ptr<T>& Ptr<T>::release() {
    if(_locator.objectIndex == -1) {
      return *this;
    }

    master->getManagerAtIndex(_locator.managerIndex)
          ->release(_locator.objectIndex, _locator.key);
    
    #ifdef DEBUG
    if(_locator.trace) {
      printEvent("released");
    }
    #endif

    return *this;
  }
  
  
  template<typename T>
  inline Ptr<T>& Ptr<T>::replace(T* const& replacement) {
    #ifdef DEBUG
    _obj = replacement;
    if(_locator.trace) {
      printEvent("is being replaced");
    }
    #endif
    
    if(_locator.objectIndex != -1) {
      release();
    }
    
    if(!master) {
      throw KFException("Master manager is not initialized.");
    }
    
    if(!replacement) {
      _locator.objectIndex = -1;
    } else {
      _locator = replacement->getPtr().getLocator();
      _locator.autorelease = true;
    }
    
    return *this;
  }

  
  template<typename T>
  inline Ptr<T>& Ptr<T>::replace(const Ptr<T>& replacement) {
    #ifdef DEBUG
    _obj = replacement._obj;
    if(_locator.trace) {
      printEvent("is being replaced");
    }
    #endif
    
    const bool ar = _locator.autorelease;
    
    if(ar && _locator.objectIndex != -1) {
      release();
    }
    
    _locator = replacement.getLocator();
    _locator.autorelease = ar;
    
    if(ar && _locator.objectIndex != -1) {
      retain();
    }
    
    return *this;
  }
  
  
  template<typename T>
  void Ptr<T>::unmanage() const {
    master->getManagerAtIndex(_locator.managerIndex)
          ->remove(_locator.objectIndex, _locator.key);
  }
  
  
  template<typename T>
  inline void Ptr<T>::setAutorelease(bool value) {
    _locator.autorelease = value;
    
    #ifdef DEBUG
    if(_locator.trace) {
      if(value) {
        printEvent("set autorelease");
      } else {
        printEvent("autorelease disabled");
      }
    }
    #endif
  }
  
  
  template<typename T>
  int Ptr<T>::getRetainCount() const {
    if(!_locator.objectIndex) {
      return RETAIN_COUNT_NULL;
    }
    
    ObjectRecord* record = *(objectTable + _locator.managerIndex)
        + _locator.objectIndex;
    
    if(record->ptr == NULL || record->retainCount == 0) {
      return RETAIN_COUNT_INVALID;
    }
    
    if(record->isStatic) {
      return RETAIN_COUNT_STATIC;
    }
    
    return record->retainCount;
  }
  

  template<typename T>
  inline bool Ptr<T>::isNull() const {
    return _locator.objectIndex == -1;
  }
  
  
  template<typename T>
  inline bool Ptr<T>::isValid() const {
    if(_locator.objectIndex == -1) {
      return false;
    }
    
    ObjectRecord* record = *(objectTable + _locator.managerIndex)
        + _locator.objectIndex;
    
    return record->key == _locator.key;
  }
  
  
  template<typename T>
  bool Ptr<T>::isAutoRelease() const {
    return _locator.autorelease;
  }

  
  template<typename T>
  inline T& Ptr<T>::operator*() const {
    if(_locator.objectIndex == -1) {
      throw NullPointerException("Attempt to dereference a null pointer: "
          + toShortString());
    }
    
    ObjectRecord* record = *(objectTable + _locator.managerIndex)
        + _locator.objectIndex;
    
    if(record->key != _locator.key) {
      throw InvalidPointerException("Attempt to dereference an invalid pointer: "
          + toShortString());
    }
    
    return *((T*)record->ptr);
  }

  
  template<typename T>
  inline T* Ptr<T>::toPurePtr() const {
    if(_locator.objectIndex == -1) {
      return NULL;
    }
    
    ObjectRecord* record = *(objectTable + _locator.managerIndex)
      + _locator.objectIndex;
    
    if(record->key != _locator.key) {
      return NULL;
    }
    
    return (T*)record->ptr;
  }

  
  template<typename T>
  inline T* Ptr<T>::operator->() const {
    if(_locator.objectIndex == -1) {
      throw NullPointerException("Attempt to dereference a null pointer: "
                                 + toShortString());
    }
    
    ObjectRecord* record = *(objectTable + _locator.managerIndex)
        + _locator.objectIndex;
    
    if(record->key != _locator.key) {
      throw InvalidPointerException("Attempt to dereference an invalid "
                                    "pointer: " + toShortString());
    }
    
    return (T*)record->ptr;
  }

  
  template<typename T>
  inline Ptr<T>& Ptr<T>::operator=(const Ptr<T>& other) {
    return replace(other);
  }

  
  template<typename T>
  inline Ptr<T>& Ptr<T>::operator=(T* const& obj) {
    return replace(obj);
  }

  
  template<typename T>
  bool Ptr<T>::operator==(const Ptr<T>& other) const {
    return _locator.objectIndex == other._locator.objectIndex
        && _locator.managerIndex == other._locator.managerIndex
        && _locator.key == other._locator.key;
  }
  
  
  template<typename T>
  bool Ptr<T>::operator==(const T* ptr) const {
    return toPurePtr() == ptr;
  }
  
  
  template<typename T>
  bool Ptr<T>::operator!=(const Ptr<T>& other) const {
    return _locator.objectIndex != other._locator.objectIndex
        || _locator.managerIndex != other._locator.managerIndex
        || _locator.key != other._locator.key;
  }
  
  
  template<typename T>
  bool Ptr<T>::operator!=(const T* ptr) const {
    return toPurePtr() != ptr;
  }

  
  template<typename T>
  inline PtrBase::ObjectLocator Ptr<T>::getLocator() const {
    return _locator;
  }
  
  
  template<typename T>
  template<typename K>
  inline bool Ptr<T>::isa() const {
    if(_locator.objectIndex == -1) {
      return false;
    }
    
    return dynamic_cast<K*>(toPurePtr()) != NULL;
  }
  
  template<typename T>
  template<typename K>
  Ptr<K> Ptr<T>::cast() const {
    if(_locator.objectIndex == -1) {
      return Ptr<K>();
    }
    
    if(!isa<K>()) {
      if(!isValid()) {
        throw KFException("The pointer being casted is invalid: "
            + toShortString());
      }
      
      throw KFException("Invalid cast from "
          + System::demangle(typeid(T).name()) + " to "
          + System::demangle(typeid(K).name()));
    }
    
    Ptr<K> p(_locator.managerIndex, _locator.objectIndex);
    
#ifdef DEBUG
    if(_locator.trace) {
      p.trace();
      printEvent("casted to " + System::demangle(typeid(p).name()));
    }
#endif
    
    return p.retain();
  }
 
#ifdef DEBUG
  template<typename T>
  void Ptr<T>::printEvent(const string& verb, bool stack) const {
    cout << "! " << (int)_locator.managerIndex << ":" << _locator.objectIndex
         << System::demangle(typeid(*this).name()) << " " << verb
         << " rt:" << getRetainCount() << endl;
    
    if(!stack) {
      return;
    }
    
    void* addrList[20];
    int addrLen = System::backtrace(addrList, 20);
    if(addrLen == 0) {
      return;
    }
    
    char** symbolList = System::backtraceSymbols(addrList, addrLen);
    for(int i = 1; i < addrLen; i++) {
      KFException::StackTraceItem item =
          KFException::StackTraceItem::fromStackTraceString(symbolList[i]);
      cout << "    /\\ " << item.getSymbol() << endl;
    }
    free(symbolList);
    
    cout << endl;
  }
#endif

  
  template<typename T>
  string Ptr<T>::toShortString() const {
    return System::demangle(typeid(T).name()) + "@["
        + Int::toString(_locator.managerIndex)
        + ":" + Int::toString(_locator.objectIndex) + "]";
  }
  
  
  template<typename T>
  void Ptr<T>::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("Ptr")
      ->attribute("type", System::demangle(typeid(*this).name()))
      ->attribute("managerIndex", (int)_locator.managerIndex)
      ->attribute("obejctIndex", _locator.objectIndex)
      ->attribute("key", _locator.key)
      ->attribute("retainCount", getRetainCount())
      ->attribute("autoRelease", _locator.autorelease)
      ->attribute("serialNumber", serial);
    
    if(isValid()) {
      builder->attribute("memory", LongInt::toHexString(toPurePtr()));
    }
    
    builder->endObject();
  }
  
  
//\/ SPtr /\///////////////////////////////////////////////////////////////////
  
  template<typename T>
  SPtr<T>::SPtr()
  : Ptr<T>()
  {
    Ptr<T>::_locator.selfDestruct = false;
  }
  
  
  template<typename T>
  SPtr<T>::SPtr(T* obj)
  : Ptr<T>(obj)
  {
    ObjectRecord* record = *(Ptr<T>::objectTable + Ptr<T>::_locator.managerIndex)
        + Ptr<T>::_locator.objectIndex;
    record->isStatic = true;
    record->retainCount = -10;
    Ptr<T>::_locator.autorelease = false;
    Ptr<T>::_locator.selfDestruct = false;
    System::getMasterMemoryManager().registerSPtr(this);
  }
  
  
  template<typename T>
  SPtr<T>::SPtr(const Ptr<T>& obj)
  : Ptr<T>(obj)
  {
    ObjectRecord* record = *(Ptr<T>::objectTable + Ptr<T>::_locator.managerIndex)
        + Ptr<T>::_locator.objectIndex;
    record->isStatic = true;
    record->retainCount = -10;
    Ptr<T>::_locator.autorelease = false;
    Ptr<T>::_locator.selfDestruct = false;
    System::getMasterMemoryManager().registerSPtr(this);
  }
  
  
  template<typename T>
  void SPtr<T>::setSelfDestruct() {
    Ptr<T>::_locator.selfDestruct = true;
  }
  
  
  template<typename T>
  SPtr<T>::~SPtr<T>() {
    if(!Ptr<T>::isNull() && Ptr<T>::_locator.selfDestruct) {
      delete Ptr<T>::toPurePtr();
    }
  }

  
//\/ PPtr /\///////////////////////////////////////////////////////////////////
  
  template<typename T>
  PPtr<T>::PPtr()
  :Ptr<T>()
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  template<typename T>
  PPtr<T>::PPtr(const PPtr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  template<typename T>
  PPtr<T>::PPtr(const Ptr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  template<typename T>
  PPtr<T>::PPtr(const SPtr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  template<typename T>
  PPtr<T>::PPtr(T* const obj)
  : Ptr<T>(obj)
  {
     // Nothing;
  }
  
} // namespace kfoundation



#endif // defined(KFOUNDATION_PTR_H)
