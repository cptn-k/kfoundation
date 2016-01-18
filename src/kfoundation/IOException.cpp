/*---[IOException.cpp]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::IOException::*
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

#include "Ref.h"
#include "UString.h"
#include "IOException.h"

namespace kfoundation {
  
  /**
   * Constructor.
   *
   * @param message A message explaining the cause of exception.
   */
  
  IOException::IOException(RefConst<UString> message)
  : KFException(message)
  {
    setName(K"IOException");
  }
  
} // kfoundation