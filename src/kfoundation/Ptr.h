/*---[Ptr.h]---------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Ptr<T>::*
 |              kfoundation::SPtr<T>::*
 |              kfoundation::PPtr<T>::*
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
  
  /**
   * Constructs a NULL-pointer to the given template class type.
   */
  
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
  
  
  /**
   * Converts a given C pointer to managed pointer.
   *
   * @param obj Pointer to a ManagedObject or one of its subclasses, or NULL.
   * @param trace Used for debugging. Causes a lot of information to be printed
   *              everytime the pointer is modified.
   */
  
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

  
  /**
   * Copy constructor.
   * @param other The other pointer to be copied.
   */
  
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

  
  /**
   * Deconstructor. If not a passive pointer, the pointed object will
   * be released.
   */
  
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
  
#if defined(DEBUG) || defined(__doxygen__)
  
  /**
   * Enables trace mode. Causes a lot of information to be printed every time
   * this pointer is modified. This method is only available when compiled with
   * DEBUG macro defined.
   *
   * @return Self
   * @see untrace()
   */
  
  template<typename T>
  Ptr<T>& Ptr<T>::trace() {
    cout << "Tracing: " << *this << endl;
    _locator.trace = true;
    return *this;
  }
  
  
  /**
   * Disables trace mode. This method is only available when compiled with
   * DEBUG macro defined.
   *
   * @return Self
   * @see trace()
   */
  
  template<typename T>
  Ptr<T>& Ptr<T>::untrace() {
    cout << "Untracing: " << *this << endl;
    _locator.trace = false;
    return *this;
  }
  
  
  /**
   * Manually releases the pointed object, and makes the pointer passive for
   * the rest of its life time. This method is only available with DEBUG macro
   * defined.
   *
   * @return Self
   */
  
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
  
  /**
   * Increases the retain count for the object pointed by this pointer by one.
   *
   * @return Self
   */
  
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

  
  /**
   * Decreases the retain count for the object pointed by this pinter by one.
   * If the retain count reaches zero, the object will be deleted.
   *
   * @return Self
   */
  
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
  
  
  /**
   * Changes the object pointed to by this pointer, releases the previous object
   * and retains the new one.
   *
   * @param replacement The new objcet to point to. It should be an instance of
   *                    ManagedObject or one of its subclasses, or NULL.
   * @return Self
   */
  
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

  
  /**
   * Changes the object pointed to by this pointer, releases the previous object
   * and retains the new one.
   *
   * @param replacement A pointer to the replacement object.
   * @return Self
   */
  
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
  
  
  /**
   * Returns the retain count for the pointed object. If this pointer is invaild
   * (or NULL) the returned value will be negative.
   */
  
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
  

  /**
   * Checks if this is a NULL-pointer.
   */
  
  template<typename T>
  inline bool Ptr<T>::isNull() const {
    return _locator.objectIndex == -1;
  }
  
  
  /**
   * Checks if this is a valid pointed. A pointer is valid if it is not NULL and
   * it points to an existing object.
   */
  
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

  
  /**
   * Dereference operator. Functions the same as normal C/C++ dereference
   * operator.
   */
  
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

  
  /**
   * Returns the actual memory location the pointed object is stored.
   */
  
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

  
  /**
   * Structure dereference operator. Functions the same as normal C/C++ 
   * equivalant.
   */
  
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

  
  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(const Ptr<T>&).
   *
   * @return Self
   */
  
  template<typename T>
  inline Ptr<T>& Ptr<T>::operator=(const Ptr<T>& other) {
    return replace(other);
  }

  
  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(T* const&).
   *
   * @return Self
   */
  
  template<typename T>
  inline Ptr<T>& Ptr<T>::operator=(T* const& obj) {
    return replace(obj);
  }

  
  /**
   * Equality operator.
   */
  
  template<typename T>
  bool Ptr<T>::operator==(const Ptr<T>& other) const {
    return _locator.objectIndex == other._locator.objectIndex
        && _locator.managerIndex == other._locator.managerIndex
        && _locator.key == other._locator.key;
  }
  
  
  /**
   * Equality operator between a managed pointer and a classic pointer.
   */
  
  template<typename T>
  bool Ptr<T>::operator==(const T* ptr) const {
    return toPurePtr() == ptr;
  }
  
  
  /**
   * Inequality operator.
   */
  
  template<typename T>
  bool Ptr<T>::operator!=(const Ptr<T>& other) const {
    return _locator.objectIndex != other._locator.objectIndex
        || _locator.managerIndex != other._locator.managerIndex
        || _locator.key != other._locator.key;
  }
  
  
  /**
   * Inequality operator between a managed pointer and a class pointer.
   */
  
  template<typename T>
  bool Ptr<T>::operator!=(const T* ptr) const {
    return toPurePtr() != ptr;
  }

  
  template<typename T>
  inline PtrBase::ObjectLocator Ptr<T>::getLocator() const {
    return _locator;
  }
  
  
  /**
   * Checks if the pointed object is an instance of the given template argument.
   * 
   *     if(myPtr.isa<MyClass>()) { ... }
   *
   * A more elegant syntax is provided by ISA(X) macro.
   *
   *     if(myPtr.ISA(MyClass)) { ... }
   *
   */
  
  template<typename T>
  template<typename K>
  inline bool Ptr<T>::isa() const {
    if(_locator.objectIndex == -1) {
      return false;
    }
    
    return dynamic_cast<K*>(toPurePtr()) != NULL;
  }
  
  
  /**
   * Casts the pointed object to the type given as template argument. 
   *
   *     Ptr<MySuperClass> castedPtr = myPtr.cast<MySuperClass>();
   *
   * A more elegant syntax is available through AS(X) macro.
   *
   *     Ptr<MySuperClass> castedPtr = myPtr.AS(MySuperClass);
   *
   */
  
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

  
  /**
   * Returns a short string representation of this pointer.
   */
  
  template<typename T>
  string Ptr<T>::toShortString() const {
    return System::demangle(typeid(T).name()) + "@["
        + Int::toString(_locator.managerIndex)
        + ":" + Int::toString(_locator.objectIndex) + "]";
  }
  
  
  /**
   * Serializing method.
   */
  
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
  
  /**
   * Default constructor, creates a static NULL-pointer.
   */
  
  template<typename T>
  SPtr<T>::SPtr()
  : Ptr<T>()
  {
    Ptr<T>::_locator.selfDestruct = false;
  }
  
  
  /**
   * Constructs a static pointer to the object at the given memory location.
   * This constructor is called when initializing object as follows.
   *
   *     public: static SPtr<MyClass> MY_STATIC_OBJECT = new MyClass();
   *
   * @param obj Should be pointing to a valid instance of ManagedObject or
   *            one of its subclasses, or NULL.
   */
  
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
  
  
  /**
   * Copy constructor. If the given parameter is not static, it will be made 
   * static.
   */
  
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
  
  /**
   * Default constructor, creates a passive NULL-pointer. Passive pointers do
   * not automatically retain or release.
   */
  
  template<typename T>
  PPtr<T>::PPtr()
  :Ptr<T>()
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  /**
   * Copy constructor.
   *
   * @param obj The pointer to be copied.
   */
  
  template<typename T>
  PPtr<T>::PPtr(const PPtr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  /**
   * Copy constructor from nonpassive pointer.
   *
   * @param obj The pointer to be copied.
   */
  
  template<typename T>
  PPtr<T>::PPtr(const Ptr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  /**
   * Copy constructor from static pointer.
   *
   * @param obj The pointer to be copied.
   */
  
  template<typename T>
  PPtr<T>::PPtr(const SPtr<T>& obj)
  : Ptr<T>(obj)
  {
    Ptr<T>::_locator.autorelease = false;
  }
  
  
  /**
   * Creates a passive pointer to the object at the given memory location.
   *
   * @param obj Should be a valid instance of ManagedObject or one of its
   *            subclasses, or NULL.
   */
  
  template<typename T>
  PPtr<T>::PPtr(T* const obj)
  : Ptr<T>(obj)
  {
     // Nothing;
  }
  
} // namespace kfoundation


#endif // defined(KFOUNDATION_PTR_H)
