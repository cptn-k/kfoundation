/*---[IOException.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::IOException::*
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

#ifndef __KFoundation__IOException__
#define __KFoundation__IOException__

#include "KFException.h"

namespace kfoundation {
  
  /**
   * Thrown to signal an IO-related exception.
   *
   * @ingroup io
   * @ingroup exceptions
   * @headerfile IOException.h <kfoundation/IOException.h>
   */
  
  class IOException : public KFException {
    public: IOException(const string& message);
  };
  
}

#endif /* defined(__KFoundation__IOException__) */
