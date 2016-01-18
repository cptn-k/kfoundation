/*---[MasterMemoryManager.cpp]---------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::MasterMemoryManager::*
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
#include <cstring>

// Internal
#include "Logger.h"
#include "RefCountMemoryManager.h"
#include "KFException.h"
#include "MemoryException.h"
#include "Int.h"
#include "UString.h"

// Self
#include "MasterMemoryManager.h"

#define N_MAX_MANAGERS 128

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Default constructor.
   */
  
  MasterMemoryManager::MasterMemoryManager() {
    _recordTable = new MemoryManager::ObjectRecord*[N_MAX_MANAGERS];
    memset(_recordTable, 0, sizeof(MemoryManager::ObjectRecord*)*N_MAX_MANAGERS);
    
    _managers = new MemoryManager*[N_MAX_MANAGERS];
    memset(_managers, 0, sizeof(MemoryManager::MemoryManager*)*N_MAX_MANAGERS);

    _nManagers = 0;
    _staticManager  = new RefCountMemoryManager(this, true);
    _defaultManager = new RefCountMemoryManager(this, false);
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
   * Registers a new memory manager and assignes it with a unique ID.
   *
   * @param manager The manager to be registered.
   * @return The ID assigned to the given manager.
   */
  
  int MasterMemoryManager::addManager(MemoryManager *manager,
      MemoryManager::ObjectRecord* table, kf_int32_t size)
  {
    int index = -1;
    for(int i = 0; i < N_MAX_MANAGERS; i++) {
      if(_managers[i] == NULL) {
        index = i;
        break;
      }
    }
    
    if(index == -1) {
      throw KFException(K"Could not register new memory manager. "
          "Master's table is full.");
    }
    
    _recordTable[index] = table;
    _managers[index] = manager;
    _nManagers++;
    return index;
  }


  void MasterMemoryManager::updateManager(int index,
      MemoryManager::ObjectRecord* table, kf_int32_t size)
  {
    _recordTable[index] = table;
  }
  
  
  /**
   * Unregisters a memory manager given its index.
   *
   * @param index The index of the memory manager to be unregistered.
   */
  
  void MasterMemoryManager::removeManager(int index) {
    _recordTable[index] = NULL;
    _managers[index] = NULL;
    _nManagers--;
  }
  
  
  /**
   * Returns the number of registered managers.
   */
  
  kf_int8_t MasterMemoryManager::getManagerCount() const {
    return _nManagers;
  }
  
  
  /**
   * Returns the manager at the given index.
   */
  
  MemoryManager& MasterMemoryManager::getManagerAtIndex(int index) const {
    if(_managers[index] == NULL) {
      throw KFException(K"Attemp to access an invalid manager: " + index);
    }
    return *_managers[index];
  }
  
  
  void MasterMemoryManager::migrate(MasterMemoryManager& other) {
    RefBase::setMasterMemoryManager(other);
  }
  

  /**
   * Prints a report of memory usage.
   */
  
  void MasterMemoryManager::printStats() const {
    for(int i = 0; i < _nManagers; i++) {
      if(_managers[i] == NULL) {
        continue;
      }

      LOG << "Manager[" << i << "]: " << _managers[i]->getObjectCount()
          << " objects" << OVER;
    }
  }

} // namespace kfoundation