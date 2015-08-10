/*---[OutOfMemoryException.h]----------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: OutOfMemoryException
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

#ifndef ORG_KNORBA_COMMON_OUTOFMEMORYEXCEPTION_H
#define ORG_KNORBA_COMMON_OUTOFMEMORYEXCEPTION_H 

#include "MemoryException.h"

namespace kfoundation {

  /**
   * Thrown on out of memory.
   *
   * @ingroup exceptions
   * @ingroup memory
   */
  
  class OutOfMemoryException : public MemoryException {
  public:
    OutOfMemoryException(string message);
  };
  
} // namespace kfoundation

#endif
