/*---[Streamer.cpp]-------------------------------------------m(._.)m--------*\
|
|  KnoRBA Common / Streamer
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////


// Internal
#include "Ref.h"
#include "BufferOutputStream.h"
#include "UString.h"

// Self
#include "Streamer.h"

namespace kfoundation {
  
  /**
   * Converts the result of invocation of `printToStream(ostream&)` to a
   * `std::string` object.
   */

  RefConst<UString> Streamer::toString() const {
    Ref<BufferOutputStream> bos = new BufferOutputStream();
    printToStream(bos.AS(OutputStream));
    return new UString(bos->getData(), bos->getSize());
  }
  
} // namespace kfoundation