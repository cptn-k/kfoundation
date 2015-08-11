/*---[MemoryException.h]---------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::MemoryException::*
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

#ifndef ORG_KNORBA_COMMON_MEMORYEXCEPTION_H
#define ORG_KNORBA_COMMON_MEMORYEXCEPTION_H 

#include "KFException.h"

namespace kfoundation {

  /**
   * Used to throw exeptions related to memory.
   *
   * @ingroup memory
   * @ingroup exceptions
   * @headerfile MemoryException.h <kfoundation/MemoryException.h>
   */
  
  class MemoryException : public KFException {
    public: MemoryException(string message);
  };
  
} // namespace kfoundation

#endif
