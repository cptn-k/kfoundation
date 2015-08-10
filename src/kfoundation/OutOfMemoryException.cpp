/*---[OutOfMemoryException.cpp]--------------------------------m(._.)m--------*\
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

#include "OutOfMemoryException.h"

namespace kfoundation {
  
  /**
   * Constructor.
   *
   * @param message The message to be displayed once the exception is thrown.
   */
  
  OutOfMemoryException::OutOfMemoryException(string message)
    : MemoryException(message)
  {
    setName("OutOfMemoryException");
  }
  
} // namespace kfoundation