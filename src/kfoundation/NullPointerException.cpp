/*---[NullPointerException.cpp]--------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::NullPointerException::*
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

#include "NullPointerException.h"

namespace kfoundation {
  
  /**
   * Default constructor.
   */
  
  NullPointerException::NullPointerException()
  : InvalidPointerException("")
  {
    setName("NullPointerException");
  }
  
  
  /**
   * Message constructor.
   */
  
  NullPointerException::NullPointerException(string message)
    : InvalidPointerException(message)
  {
    setName("NullPointerException");
  }
  
} // namespace kfoundation;
