/*---[MasterMemoryManager.cpp]---------------------------------m(._.)m--------*\
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

// Std
#include <cassert>

// Internal
#include "Logger.h"
#include "RefCountMemoryManager.h"
#include "KFException.h"
#include "MemoryException.h"
#include "Int.h"

// Self
#include "MasterMemoryManager.h"

#define N_MAX_MANAGERS 128

namespace kfoundation {
  
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Default constructor.
   */
  
  MasterMemoryManager::MasterMemoryManager() {
    _recordTable = new ObjectRecord*[N_MAX_MANAGERS];
    memset(_recordTable, 0, sizeof(ObjectRecord*)*N_MAX_MANAGERS);
    
    _managers = new MemoryManager*[N_MAX_MANAGERS];
    memset(_managers, 0, sizeof(MemoryManager*)*N_MAX_MANAGERS);
    
    _nManagers = 0;
    MemoryManager* first = new RefCountMemoryManager(this);
    _nStatics = 0;
    
    assert(first == getManagerAtIndex(0));
    
    PtrBase::master = this;
    PtrBase::objectTable = _recordTable;
  }
  
  
  /**
   * Deconstructor.
   */
  
  MasterMemoryManager::~MasterMemoryManager() {
    for(int i = 0; i < N_MAX_MANAGERS; i++) {
      if(_managers[i] != NULL) {
        delete _managers[i];
      }
    }
    delete[] _recordTable;
  }
  
  
// --- METHODS --- //
  
  /**
   * Registeres a new object to the default manager.
   */
  
  const ObjectRecord& MasterMemoryManager::registerObject(ManagedObject* ptr) {
    return _managers[0]->registerObject(ptr);
  }
  
  
  /**
   * Registers a new memory manager and assignes it with a unique ID.
   *
   * @param manager The manager to be registered.
   * @return The ID assigned to the given manager.
   */
  
  int MasterMemoryManager::registerManager(MemoryManager *manager) {
    int index = -1;
    for(int i = 0; i < N_MAX_MANAGERS; i++) {
      if(_managers[i] == NULL) {
        index = i;
        break;
      }
    }
    
    if(index == -1) {
      throw KFException("Could not register new memory manager. "
                        "Master's table is full.");
    }
    
    _recordTable[index] = manager->getTable();
    _managers[index] = manager;
    _nManagers++;
    return index;
  }
  
  
  /**
   * Unregisters a memory manager given its index.
   *
   * @param index The index of the memory manager to be unregistered.
   */
  
  void MasterMemoryManager::unregisterManager(int index) {
    _recordTable[index] = NULL;
    _managers[index] = NULL;
    _nManagers--;
  }
  
  
  /**
   * Returns the number of registered managers.
   */
  
  kf_octet_t MasterMemoryManager::getNManagers() {
    return _nManagers;
  }
  
  
  /**
   * Returns the manager at the given index.
   */
  
  MemoryManager* MasterMemoryManager::getManagerAtIndex(int index) {
    if(_managers[index] == NULL) {
      throw KFException("Attemp to access an invalid manager: "
                        + Int::toString(index));
    }
    return _managers[index];
  }
  
  
  /**
   * This should be called whenever a manager reallocates its table. 
   * MasterMemoryManager keeps a separate record of the all memory manager 
   * tables. If its location is changed for any reason, this function should
   * be called to so that master updates its internal reference.
   */
  
  void MasterMemoryManager::updataTable(int index) {
    _recordTable[index] = _managers[index]->getTable();
  }
  
  
  void MasterMemoryManager::migrate(MasterMemoryManager& other) {
    int newId = dynamic_cast<RefCountMemoryManager*>(_managers[0])->migrate(other);
    
    for(int i = 0; i < _nStatics; i++) {
      _statics[i]->_locator.managerIndex = newId;
    }
    
    PtrBase::master = &other;
    PtrBase::objectTable = other._recordTable;
    _managers[0] = NULL;
  }
  
  
  void MasterMemoryManager::registerSPtr(PtrBase *p) {
    if(_nStatics == 100) {
      throw MemoryException("Too many static pointers registered.");
    }
    
    _statics[_nStatics] = p;
    _nStatics++;
  }
  
  
  int MasterMemoryManager::update(kf_int16_t index, kf_int16_t key) {
    for(int i = 0; i < _nManagers; i++) {
      if(index < _managers[i]->getTableSize()) {
        if(_recordTable[i][index].key == key) {
          return i;
        }
      }
    }
    return -1;
  }
  
  
  /**
   * Prints a list of all managed objects.
   */
  
  void MasterMemoryManager::dump() const {
    throw KFException("Operation not implemented.");
    
//    for(int i = 0; i < _nManagers; i++) {
//      if(_managers[i] != NULL) {
//        // Something here
//      }
//    }
  }
  
  
  /**
   * Prints a report of memory usage.
   */
  
  void MasterMemoryManager::printStats() const {
    for(int i = 0; i < _nManagers; i++) {
      if(_managers[i] == NULL) {
        continue;
      }
      
      Statistics stats = _managers[i]->getStats();
      LOG << "Manager[" << i << "]:  Alive objects: "
          << stats.nObjects - stats.nStaticObjects << " automatic + "
          << stats.nStaticObjects << " static" << EL;
    }
  }
  
  
  void MasterMemoryManager::finalize() {
    dynamic_cast<RefCountMemoryManager*>(_managers[0])->finalize();
  }
  
} // namespace kfoundation
