/*---[NullPointerException.cpp]--------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: NullPointerException
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
  
  NullPointerException::NullPointerException()
  : InvalidPointerException("")
  {
    setName("NullPointerException");
  }
  
  NullPointerException::NullPointerException(string message)
    : InvalidPointerException(message)
  {
    setName("NullPointerException");
  }
  
} // namespace kfoundation;
