/*---[PtrDecl.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: PtrBase
 |         Ptr<T>
 |         PPtr<T>
 |         SPtr<T>
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

#define ISA(X) isa<X>()
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
    
    #ifdef DEBUG
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
  
  template<typename T>
  class SPtr : public Ptr<T> {
    public: SPtr();
    public: SPtr(T* obj);
    public: SPtr(const Ptr<T>& obj);
    public: void setSelfDestruct();
    public: ~SPtr();
  };
  
  
//\/ PPtr /\///////////////////////////////////////////////////////////////////
  
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
