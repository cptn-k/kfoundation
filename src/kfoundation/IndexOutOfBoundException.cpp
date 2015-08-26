/*---[IndexOutOfBoundException.cpp]----------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::IndexOutOfBoundException::*
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

#include "IndexOutOfBoundException.h"

namespace kfoundation {
  
  /**
   * Constructor.
   *
   * @param message A message describing the cause of the exception.
   */
  
  IndexOutOfBoundException::IndexOutOfBoundException(string message)
    : MemoryException(message)
  {
    setName("IndexOutOfBoundException");
  }
  
} // namespace kfoundation