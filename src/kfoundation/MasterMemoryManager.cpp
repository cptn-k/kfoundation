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
  
  
  MasterMemoryManager::~MasterMemoryManager() {
    for(int i = 0; i < N_MAX_MANAGERS; i++) {
      if(_managers[i] != NULL) {
        delete _managers[i];
      }
    }
    delete[] _recordTable;
  }
  
  
// --- METHODS --- //
  
  const ObjectRecord& MasterMemoryManager::registerObject(ManagedObject* ptr) {
    return _managers[0]->registerObject(ptr);
  }
  
  
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
  
  
  void MasterMemoryManager::unregisterManager(int index) {
    _recordTable[index] = NULL;
    _managers[index] = NULL;
    _nManagers--;
  }
  
  
  kf_octet_t MasterMemoryManager::getNManagers() {
    return _nManagers;
  }
  
  
  MemoryManager* MasterMemoryManager::getManagerAtIndex(int index) {
    if(_managers[index] == NULL) {
      throw KFException("Attemp to access an invalid manager: "
                        + Int::toString(index));
    }
    return _managers[index];
  }
  
  
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
  
  
  void MasterMemoryManager::dump() const {
    for(int i = 0; i < _nManagers; i++) {
      if(_managers[i] != NULL) {
        LOG << "Manager[" << i << "]:  " << *_managers[i] << EL;
      }
    }
  }
  
  
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
