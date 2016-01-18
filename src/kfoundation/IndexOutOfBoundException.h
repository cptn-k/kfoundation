/*---[IndexOutOfBoundException.h]------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::IndexOutOfBoundException::*
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

#ifndef ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H
#define ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H

#include "MemoryException.h"

namespace kfoundation {

  /**
   * Thrown to signal access to a nonexisting element in an array.
   *
   * @ingroup containers
   * @ingroup exceptions
   * @headerfile IndexOutOfBoundException.h <kfoundation/IndexOutOfBoundException.h>
   */
  
  class IndexOutOfBoundException : public MemoryException {
  public:
    IndexOutOfBoundException(RefConst<UString> message);
  };

} // namespace kfoundation

#endif