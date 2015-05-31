/*---[InvalidPointerException.cpp]-----------------------------m(._.)m--------*\
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

#include "InvalidPointerException.h"

namespace kfoundation {
  
  InvalidPointerException::InvalidPointerException(string message)
    : MemoryException(message)
  {
    setName("InvalidPointerException");
  }
  
} // namespace kfoundation