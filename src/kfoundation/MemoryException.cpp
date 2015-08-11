/*---[MemoryException.cpp]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::MemoryException::*
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

#include "MemoryException.h"

namespace kfoundation {

  /**
   * Constructor.
   */
  
  MemoryException::MemoryException(string message)
    : KFException(message)
  {
    setName("MemoryException");
  }

} // namespace kfoundation
