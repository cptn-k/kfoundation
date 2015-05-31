/*---[InvalidPointerException.h]-------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: InvalidPointerException
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

#ifndef ORG_KNORBA_COMMON_INVALIDPOINTEREXCEPTION_H
#define ORG_KNORBA_COMMON_INVALIDPOINTEREXCEPTION_H
   
#include "MemoryException.h"

namespace kfoundation {

  class InvalidPointerException : public MemoryException {
  public:
    InvalidPointerException(string message);
  };
  
} // namespace kfoundation


#endif
