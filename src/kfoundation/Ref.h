/*---[Ref.h]---------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Ref<T>::*
 |              kfoundation::SPtr<T>::*
 |              kfoundation::Ref<T>::*
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
#include "NullPointerException.h"
#include "MasterMemoryManager.h"
#include "KFObject.h"
#include "System.h"
#include "UString.h"

// Self
#include "RefDecl.h"

namespace kfoundation {

//\/ RefBase /\////////////////////////////////////////////////////////////////

  inline void RefBase::retain() {
    master->getManagerAtIndex(_ref.manager).retain(_ref.index, _ref.key);
  }


  inline void RefBase::release() {
    master->getManagerAtIndex(_ref.manager).release(_ref.index, _ref.key);
  }


  inline void RefBase::assignNull() {
    SET_NULL_REF(_ref);
  }


  inline void RefBase::assignCptr(const KFObject* cptr) {
    if(IS_NULL(master)) {
      setMasterMemoryManager(System::getMasterMemoryManager());
    }

    if(IS_NULL(cptr)) {
      assignNull();
    } else {
      _ref = cptr->registerAndRetain(*defaultManager);
    }

    #ifdef DEBUG
    obj = cptr;
    #endif
  }


  inline void RefBase::assignRef(const kf_uref_t ref) {
    if(IS_NULL_REF(ref)) {
      assignNull();
    } else {
      _ref = ref;
      retain();
    }

    #ifdef DEBUG
    obj = masterTable[_ref.manager][_ref.index].ptr;
    #endif
  }


  inline kf_uref_t RefBase::get() const {
    return _ref;
  }


  template<typename TT>
  bool RefBase::isa() const {
    if(IS_NULL_REF(_ref)) {
      return false;
    }
    return NOT_NULL(dynamic_cast<TT*>(masterTable[_ref.manager][_ref.index].ptr));
  }


//\/ Ref /\////////////////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructs a NULL-pointer to the given template class type.
   */
  
  template<typename T>
  Ref<T>::Ref() {
    assignNull();
  }


  template<typename T>
  Ref<T>::Ref(const kf_uref_t uref) {
    RefBase::assignRef(uref);
  }


  template<typename T>
  Ref<T>::Ref(const T* cptr) {
    assignCptr(cptr);
  }


  template<typename T>
  Ref<T>::Ref(const Ref<T>& other) {
    assignRef(other._ref);
  }


  /**
   * Deconstructor. If not a passive pointer, the pointed object will
   * be released.
   */
  
  template<typename T>
  Ref<T>::~Ref() {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
  }
  
  
// --- METHODS --- //

  template<typename T>
  template<typename TT>
  Ref<TT> Ref<T>::cast() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException(K"Cannot cast a null reference.");
    }

    if(isa<TT>()) {
      return Ref<TT>(_ref);
    }

    throw KFException(K"Cannot cast a reference of type \""
        + System::demangle(typeid(T).name()) + "\" to type \""
        + System::demangle(typeid(TT).name()) + "\"");
  }


  template<typename T>
  Ref<UString> Ref<T>::getTypeName() const {
    return System::demangle(typeid(T).name());
  }

  
// --- OPERATORS --- //

  /**
   * Dereference operator. Functions the same as normal C/C++ dereference
   * operator.
   */
  
  template<typename T>
  inline T& Ref<T>::operator*() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException();
    }
    return *((T*)(masterTable[_ref.manager][_ref.index].ptr));
  }

  
  /**
   * Structure dereference operator. Functions the same as normal C/C++ 
   * equivalant.
   */
  
  template<typename T>
  inline T* Ref<T>::operator->() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException();
    }
    return (T*)masterTable[_ref.manager][_ref.index].ptr;
  }

  
  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(const Ref<T>&).
   *
   * @return Self
   */
  
  template<typename T>
  inline Ref<T>& Ref<T>::operator=(const Ref<T>& other) {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
    assignRef(other._ref);
    return *this;
  }

  
  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(T* const&).
   *
   * @return Self
   */
  
  template<typename T>
  inline Ref<T>& Ref<T>::operator=(const T* obj) {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
    assignCptr(obj);
    return *this;
  }


//\/ PtrConst /\///////////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructs a NULL-pointer to the given template class type.
   */

  template<typename T>
  inline RefConst<T>::RefConst() {
    assignNull();
  }


  template<typename T>
  RefConst<T>::RefConst(const kf_uref_t uref) {
    assignRef(uref);
  }


  template<typename T>
  RefConst<T>::RefConst(const T* obj) {
    assignCptr(obj);
  }


  template<typename T>
  RefConst<T>::RefConst(const Ref<T>& other) {
    assignRef(other.get());
  }


  template<typename T>
  RefConst<T>::RefConst(const RefConst<T>& other) {
    assignRef(other._ref);
  }


  /**
   * Deconstructor. If not a passive pointer, the pointed object will
   * be released.
   */

  template<typename T>
  RefConst<T>::~RefConst() {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
  }


// --- METHODS --- //


  template<typename T>
  template<typename TT>
  RefConst<TT> RefConst<T>::cast() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException(K"Cannot cast a null reference.");
    }

    if(isa<TT>()) {
      return RefConst<TT>(_ref);
    }

    throw KFException(K"Cannot cast a reference of type \""
      + System::demangle(typeid(T).name()) + "\" to type \""
      + System::demangle(typeid(TT).name()) + "\"");
  }


  template<typename T>
  Ref<UString> RefConst<T>::getTypeName() const {
    return K"const " + System::demangle(typeid(T).name());
  }


// --- OPERATORS --- //

  /**
   * Dereference operator. Functions the same as normal C/C++ dereference
   * operator.
   */

  template<typename T>
  inline const T& RefConst<T>::operator*() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException();
    }
    return *((T*)(masterTable[_ref.manager][_ref.index].ptr));
  }


  /**
   * Structure dereference operator. Functions the same as normal C/C++
   * equivalant.
   */

  template<typename T>
  inline const T* RefConst<T>::operator->() const {
    if(IS_NULL_REF(_ref)) {
      throw NullPointerException();
    }
    return (T*)masterTable[_ref.manager][_ref.index].ptr;
  }


  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(T* const&).
   *
   * @return Self
   */

  template<typename T>
  inline RefConst<T>& RefConst<T>::operator=(const T* obj) {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
    assignCptr(obj);
    return *this;
  }


  /**
   * Replaces the pointed object with a new one, releases the previous object,
   * and retains the new one. Internally, it calls replace(const Ref<T>&).
   *
   * @return Self
   */

  template<typename T>
  inline RefConst<T>& RefConst<T>::operator=(const Ref<T>& other) {
    if(IS_NULL_REF(_ref)) {
      release();
    }
    assignRef(other.get());
    return *this;
  }


  template<typename T>
  inline RefConst<T>& RefConst<T>::operator=(const RefConst<T>& other) {
    if(NOT_NULL_REF(_ref)) {
      release();
    }
    assignRef(other._ref);
    return *this;
  }


//\/ StaticRef /\//////////////////////////////////////////////////////////////

  template<typename T>
  StaticRef<T>::StaticRef() {
    RefBase::assignNull();
  }


  template<typename T>
  StaticRef<T>::StaticRef(T* cptr) {
    if(IS_NULL(RefBase::master)) {
      RefBase::setMasterMemoryManager(System::getMasterMemoryManager());
    }
    if(IS_NULL(cptr)) {
      RefBase::assignNull();
    } else {
      RefBase::assignRef(cptr->registerAndRetain(RefBase::master->getStaticManager()));
    }
  }


//\/ StaticRefConst /\/////////////////////////////////////////////////////////

  template<typename T>
  StaticRefConst<T>::StaticRefConst(T* cptr) {
    if(IS_NULL(RefBase::master)) {
      RefBase::setMasterMemoryManager(System::getMasterMemoryManager());
    }
    if(IS_NULL(cptr)) {
      RefBase::assignNull();
    } else {
      RefBase::assignRef(cptr->registerAndRetain(RefBase::master->getStaticManager()));
    }
  }

} // namespace kfoundation

#endif // defined(KFOUNDATION_PTR_H)