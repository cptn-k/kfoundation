/*---[MasterMemoryManager.h]-----------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: MasterMemoryManager
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

#ifndef KFOUNDATION_MASTERMEMORYMANAGER
#define KFOUNDATION_MASTERMEMORYMANAGER

#include "MemoryManager.h"
#include "PtrDecl.h"

namespace kfoundation {
  
  class ManagedObject;
  
  
  /**
   * Manages all the memory managers used in a process.
   * Only once instance of this class exists per process.
   * This instance can be obtained via System::getMasterMemoryManager().
   * 
   * This object always owns an instance of RefCountMemoryManager as its 
   * default manager. To access use `getManagerAtIndex(0)`.
   *
   * @ingroup memory
   */
  
  class MasterMemoryManager {
    
  // --- FIELDS --- //
    
    private: ObjectRecord** _recordTable;
    private: MemoryManager** _managers;
    private: int _nManagers;
    
    private: PtrBase* _statics[100];
    private: int _nStatics;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: MasterMemoryManager();
    public: ~MasterMemoryManager();
    
    
  // --- METHODS --- //
    
    public: const ObjectRecord& registerObject(ManagedObject* ptr);
    public: int registerManager(MemoryManager* manager);
    public: void unregisterManager(int index);
    public: kf_octet_t getNManagers();
    public: MemoryManager* getManagerAtIndex(int index);
    public: ObjectRecord** getTable();
    public: void updataTable(int index);
    public: void migrate(MasterMemoryManager& other);
    public: void registerSPtr(PtrBase* p);
    public: int update(kf_int16_t index, kf_int16_t key);
    public: void dump() const;
    public: void printStats() const;
    public: void finalize();
  };
  
}

#endif /* defined(KFOUNDATION_MASTERMEMORYMANAGER) */
