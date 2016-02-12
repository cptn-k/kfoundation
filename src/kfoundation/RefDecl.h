/*---[RefDecl.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::PtrBase::*
 |              kfoundation::Ref<T>::*
 |              kfoundation::SPtr<T>::*
 |              kfoundation::Ref<T>::*
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
#include "MemoryManager.h"

/**
 * Operates like a member function on a Ref<T> and returns a boolean. 
 * If `myObject.ISA(MyClass)` returns true, then myObject is an instance of
 * MyClass.
 *
 * @ingroup defs
 * @ingroup memory
 */

#define ISA(X) isa<X>()


/**
 * Operates like a member function on a Ref<T> and casts it to another type.
 * Usage:
 *
 *     Ref<MySuperClass> myCastedObject = myObject.AS(MySuperClass);
 *
 * @ingroup defs
 * @ingroup memory
 */

#define AS(X) cast<X>()

#define IS_NULL_REF(X) ((X).index == -1)
#define NOT_NULL_REF(X) ((X).index != -1)
#define SET_NULL_REF(X) ((X).index = -1)

namespace kfoundation {
  
//\/ Forward Declerations /\////////////////////////////////////////////////////

  class KFObject;
  class MasterMemoryManager;
  class MemoryManager;
  class UString;
  class ObjectSerializer;

  template<typename T> class Ref;
  template<typename T> class StaticRef;
  template<typename T> class RefConst;
  template<typename T> class StaticRefConst;


//\/ Global /\//////////////////////////////////////////////////////////////////

  bool operator==(const kf_uref_t& first, const kf_uref_t& second);


//\/ RefBase /\/////////////////////////////////////////////////////////////////

  class RefBase {

  // --- STATIC FIELDS --- //

    protected: static MasterMemoryManager* master;
    protected: static MemoryManager* defaultManager;
    protected: static MemoryManager::ObjectRecord** masterTable;
    public: static const kf_uref_t NULL_REF;


  // --- FIELDS --- //

    protected: kf_uref_t _ref;

#ifdef DEBUG
    protected: const KFObject* obj;
#endif


  // --- STATIC METHODS --- //

    public: static void setMasterMemoryManager(MasterMemoryManager& master);


  // --- ABSTRACT METHODS --- //

    public: virtual RefConst<UString> getTypeName() const = 0;


  // --- METHODS --- //

    protected: inline void retain();
    protected: inline void release();
    protected: inline void assignNull();
    protected: inline void assignCptr(const KFObject* cptr);
    protected: inline void assignRef(const kf_uref_t ref);
    public   : inline kf_uref_t get() const;

    public: int getRetainCount() const;
    public: bool isNull() const;
    public: bool isValid() const;
    public: template<typename TT> bool isa() const;

    public: void serialize(Ref<ObjectSerializer> builder) const;
    public: RefConst<UString> toString() const;


  // --- OPERATORS --- //

    public: bool operator==(const RefBase& other) const;
    public: bool operator==(const KFObject* const other) const;
    public: bool operator!=(const RefBase& other) const;
    public: bool operator!=(const KFObject* const other) const;

  };


//\/ Ref /\////////////////////////////////////////////////////////////////////


  /**
   * Reference to a class of given template type.
   * The template type should be a subclass of KFObject.
   * To use, try
   *
   *     Ref<MyClass> myObject = new MyClass();
   *
   * To create a null pointer, try
   *
   *     Ref<MyClass> myObject = NULL;
   *
   * or just
   *
   *     Ref<MyClass> myObject;
   *
   * After this, it can be used just like an ordinary poitner.
   *
   *     myObject->myMethod();
   *
   * `Ref` prevents segmentation fault situations to happen. If the object 
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
   *     Ref<MySuperClass> myCastedObject = myObject.AS(MySuperClass);
   *
   * Similiarily, there is an elegant way to check the type of a managed
   * pointer.
   *
   *     if(myObject.ISA(MySuperClass)) {
   *       ...
   *     }
   *
   * Referenced objects do not need to be explicitly destructed. `Ref` will
   * automatically retain and release the instance of the object it is pointing 
   * to whenever necessary, and calls the destructor when the object instance
   * is no longer needed.
   *
   * You can check the retain count using getRetainCount() method. Use
   * toString() method to print a description of this reference, plus the
   * corresponding retain count.
   *
   *     LOG << myObject.toString() << OVER;
   *
   * If you mean to print the content of the object being pointed to, rather
   * than the content of the pointer, make sure to dereference it.
   *
   *     LOG << *myObject << OVER;
   *
   * KFoundation managed pointers are designed to be fast and efficient.
   * The size of `Ref` is exactly 8 bytes --- the same as normal pointers on
   * most platforms. To make it safe, the validity of the pointer is checked
   * against the memory manager's registery on each access. To make it fast,
   * a novel fast algorithm with O(1) time complexity is developed to do the
   * task.
   *
   * @see RefConst - Reference to constant
   * @see StaticRef - Static reference
   * @see StaticRefConst - Static reference to constant
   *
   * @ingroup memory
   * @headerfile RefDecl.h <kfoundation/Ref.h>
   */

  template<typename T>
  class Ref : public RefBase {

  // --- (DE)CONSTRUCTORS --- //
    
    public: Ref();
    public: Ref(const kf_uref_t uref);
    public: Ref(const T* cptr);
    public: Ref(const Ref<T>& other);
    public: ~Ref();

    
  // --- METHODS --- //

    public: template<typename TT> Ref<TT> cast() const;

    // Inherited from PtrBase
    public: RefConst<UString> getTypeName() const;


  // --- OPERATORS --- //

    public: inline T& operator*() const;
    public: inline T* operator->() const;
    public: inline Ref<T>& operator=(const Ref<T>& other);
    public: inline Ref<T>& operator=(const T* cptr);

  }; // class Ref


//\/ ConstPtr /\///////////////////////////////////////////////////////////////

  /**
   * Reference to constant vairation of Ref. The result of derefencing this
   * class is a constant image of the referenced object.
   *
   * RefConst can be assigned from Ref.
   *
   *    Ref<MyClass> ref = new MyClass();
   *    RefConst<MyClass> constRef = ref;
   *.
   * @ingroup memory
   * @headerfile RefDecl.h <kfoundation/Ref.h>
   */

  template<typename T>
  class RefConst : public RefBase {

  // --- (DE)CONSTRUCTORS --- //

    public: RefConst();
    public: RefConst(const kf_uref_t uref);
    public: RefConst(const T* cptr);
    public: RefConst(const Ref<T>& other);
    public: RefConst(const RefConst<T>& other);
    public: ~RefConst();


  // --- METHODS --- //

    public: template<typename TT> RefConst<TT> cast() const;

    // Inherited from PtrBase
    public: RefConst<UString> getTypeName() const;

    
  // --- OPERATOR --- //

    public: inline const T& operator*() const;
    public: inline const T* operator->() const;
    public: inline RefConst<T>& operator=(const T* cptr);
    public: inline RefConst<T>& operator=(const Ref<T>& other);
    public: inline RefConst<T>& operator=(const RefConst<T>& other);

  };


//\/ StaticRef /\//////////////////////////////////////////////////////////////

  /**
   * Static variant of Ref. Uses the default static memory manager instead of
   * the default reference counting manager. Objects referenced by this
   * class will never be deleted.
   *
   * Use to define static fields.
   *
   *     class MyClass : public KFObject {
   *       private: static StaticRef<UString> STATIC_FIELD;
   *     }
   *.
   * @see StaticRefConst
   * @ingroup memory
   * @headerfile RefDecl.h <kfoundation/Ref.h>
   */

  template<typename T>
  class StaticRef : public Ref<T> {

  // --- (DE)CONSTRUCTORS --- //

    public: StaticRef();
    public: StaticRef(T* cptr);

  };


//\/ StaticRefConst /\/////////////////////////////////////////////////////////

  /**
   * Reference to constant variant of StaticRef. Use to define static constant
   * fields.
   *
   *     class MyClass : public KFObject {
   *       public: static const StaticRefConst<UString> STR;
   *     }
   *.
   * @see StaticRef
   * @ingroup memory
   * @headerfile RefDecl.h <kfoundation/Ref.h>
   */

  template<typename T>
  class StaticRefConst : public RefConst<T> {

  // --- (DE)CONSTRUCTORS --- //

    public: StaticRefConst(T* cptr);

  };

} // namespace kfoundation

#endif // defined(KFOUNDATION_PTRDECL_H)