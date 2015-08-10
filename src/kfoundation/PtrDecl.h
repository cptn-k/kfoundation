/*---[PtrDecl.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::PtrBase::*
 |              kfoundation::Ptr<T>::*
 |              kfoundation::SPtr<T>::*
 |              kfoundation::PPtr<T>::*
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

#ifndef KFOUNDATION_PTRDECL_H
#define KFOUNDATION_PTRDECL_H

#include "definitions.h"
#include "SerializingStreamer.h"

/**
 * Operates like a member function on a Ptr<T> and returns a boolean. 
 * If `myObject.ISA(MyClass)` returns true, then myObject is an instance of
 * MyClass.
 *
 * @ingroup defs
 * @ingroup memory
 */

#define ISA(X) isa<X>()


/**
 * Operates like a member function on a Ptr<T> and casts it to another type.
 * Usage:
 *
 *     Ptr<MySuperClass> myCastedObject = myObject.AS(MySuperClass);
 *
 * @ingroup defs
 * @ingroup memory
 */

#define AS(X) cast<X>()

#define RETAIN_COUNT_INVALID -1;
#define RETAIN_COUNT_NULL -2;
#define RETAIN_COUNT_STATIC -10;

using namespace std;

namespace kfoundation {
  
//// Forward Declerations /////////////////////////////////////////////////////

  class ManagedObject;
  class MasterMemoryManager;
  class MemoryManager;
  struct ObjectRecord;
  
  template<typename T>
  class PPtr;
  
  
//// PtrBase //////////////////////////////////////////////////////////////////
  
  class PtrBase {
  friend class MasterMemoryManager;
    
  // --- NESTED TYPES --- //
    
    protected: struct ObjectLocator {
      kf_int32_t objectIndex;
      kf_int16_t key;
      kf_int8_t managerIndex;
      bool autorelease: 1;
      bool trace: 1;
      bool selfDestruct: 1;
    };
    
    
  // --- STATIC FIELDS --- //
    
    protected: static MasterMemoryManager* master;
    protected: static ObjectRecord** objectTable;
    protected: static long int serial;
    
  
  // --- FIELDS --- //
    
    protected: ObjectLocator _locator;
    
  };
  

//\/ Ptr /\////////////////////////////////////////////////////////////////////
  
  /**
   * Managed pointer to a class of given template type.
   * The template type should be a subclass of ManagedObject.
   * To use, try
   *
   *     Ptr<MyClass> myObject = new MyClass();
   *
   * To create a null pointer, try
   *
   *     Ptr<MyClass> myObject = NULL;
   *
   * or just
   *
   *     Ptr<MyClass> myObject;
   *
   * After this, it can be used just like an ordinary poitner.
   *
   *     myObject->myMethod();
   *
   * `Ptr` prevents segmentation fault situations to happen. If the object 
   * being dereferenced is NULL, a NullPointerException will be thrown. If
   * the object is invalid, i.e. it is previously deconstructed, an
   * InvalidPointerException will be thrown.
   *
   * You can also manually check the validity of the pointer using isValid()
   * method. isValid() will also return false if the pointer is NULL. To check
   * the for NULL pointer, use isNull() method.
   *
   * There is an elegant way to type-case a managed pointer:
   *
   *     Ptr<MySuperClass> myCastedObject = myObject.AS(MySuperClass);
   *
   * Similiarily, there is an elegant way to check the type of a managed
   * pointer.
   *
   *     if(myObject.ISA(MySuperClass)) {
   *       LOG << "I Love KFoundation <3" << EL;
   *     }
   *
   * Managed objects do not need to be explicitly destructed. `Ptr` will 
   * automatically retain and release the instance of the object it is pointing 
   * to whenever necessary, and calls the destructor when the object instance
   * is no longer needed.
   *
   * You can check the retain count using getRetainCount() method. Inputting
   * a `Ptr` to stream or logger will print a more detailed description of it.
   *
   *     LOG << myObject << EL;
   *
   * If you mean to print the content of the object being pointed to, rather
   * than the content of the pointer, make sure to dereference it.
   *
   *     LOG << *myObject << EL;
   *
   * There are two variants of Ptr: Passive Pointer (PPtr) and Static Pointer
   * (SPtr). PPtr do not retain or release the object with the scope they are
   * defined. If you are sure within a cerain scope the retain count of an
   * object will not be changed, it is advisable to use PPtr to produce a 
   * faster program. SPtr makes the pointed object immortal. It should be used
   * for static class members.
   *
   * KFoundation managed pointers are designed to be fast and efficient.
   * The size of `Ptr` is exactly 8 bytes --- the same as normal pointers on
   * most platforms. To make it safe, the validity of the pointer is checked
   * against the memory manager's registery on each access. To make it fast,
   * a novel fast algorithm with O(1) time complexity is developed to do the
   * task.
   *
   * On rare ocasions it might be needed to manage the reference count manually.
   * In such cases you may use retain(), release() and replace() methods.
   * But unless absolutely necessary please refrain from using these methods.
   *
   * @ingroup memory
   * @headerfile Ptr.h <kfoundation/Ptr.h>
   */
  
  template<typename T>
  class Ptr : public PtrBase, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    
    #ifdef DEBUG
    private: long int _serial;
    private: void* _obj;
    #endif
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: inline Ptr();
    public: Ptr(T* obj, bool trace = false);
    public: Ptr(const kf_int8_t managerIndex, const kf_int32_t objectIndex);
    public: inline Ptr(const Ptr<T>& other);    
    public: virtual ~Ptr();

    
  // --- METHODS --- //
    
    #if defined(DEBUG) || defined(__doxygen__)
    public: Ptr<T>& trace();
    public: Ptr<T>& untrace();
    public: Ptr<T>& del();
    private: void printEvent(const string& verb, bool stack = true) const;
    #endif
    
    public: Ptr<T>& retain();
    public: Ptr<T>& release();
    public: inline Ptr<T>& replace(T* const& replacement);
    public: inline Ptr<T>& replace(const Ptr<T>& replacement);
    public: inline void unmanage() const;
    public: inline void setAutorelease(bool value);

    public: int getRetainCount() const;
    public: inline bool isNull() const;
    public: inline bool isValid() const;
    public: bool isAutoRelease() const;

    public: inline T& operator*() const;
    public: T* toPurePtr() const;
    public: inline T* operator->() const;
    public: inline Ptr<T>& operator=(const Ptr<T>& other);
    public: inline Ptr<T>& operator=(T* const& obj);
    public: bool operator==(const Ptr<T>& other) const;
    public: bool operator==(const T* ptr) const;
    public: bool operator!=(const Ptr<T>& other) const;
    public: bool operator!=(const T* ptr) const;    
    public: inline ObjectLocator getLocator() const;
    
    public: template<typename K>
    inline bool isa() const;
    
    public: template<typename K>
    Ptr<K> cast() const;
    
    public: string toShortString() const;
    
    // Inherited from SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> builder) const;
    
  }; // class Ptr

  
//\/ SPtr /\///////////////////////////////////////////////////////////////////
  
  /**
   * Static pointer, makes the pointed object immortal. When an object is
   * pointed to by an SPtr it will be marked so that it never be deleted. This
   * is necessary when defining static class fields.
   * 
   * A normal Ptr can be assigned to SPtr and viceversa, however the immortal
   * properties of the object will never be changed.
   *
   * @ingroup memory
   * @headerfile Ptr.h <kfoundation/Ptr.h>
   */
  
  template<typename T>
  class SPtr : public Ptr<T> {
    public: SPtr();
    public: SPtr(T* obj);
    public: SPtr(const Ptr<T>& obj);
    public: void setSelfDestruct();
    public: ~SPtr();
  };
  
  
//\/ PPtr /\///////////////////////////////////////////////////////////////////
  
  /**
   * Passive pointer, will not release and retain automatically. This is useful
   * to produce a faster code in situations that you are sure the retain count
   * of an object is the same in the begining and the end of the scope of
   * difinition of the pointer.
   *
   * @ingroup memory
   * @headerfile Ptr.h <kfoundation/Ptr.h>
   */
  
  template<typename T>
  class PPtr : public Ptr<T> {
    public: PPtr();
    public: PPtr(const PPtr<T>& pptr);
    public: PPtr(const Ptr<T>& aptr);
    public: PPtr(const SPtr<T>& sptr);
    public: PPtr(T* const obj);
  };
  
} // namespace kfoundation


#endif // defined(KFOUNDATION_PTRDECL_H)
