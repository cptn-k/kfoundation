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

// Std
#include <sstream>

// Internal
#include "System.h"
#include "LongInt.h"

// Self
#include "Streamer.h"

namespace kfoundation {
  
  /**
   * Converts the result of invocation of `printToStream(ostream&)` to a
   * `std::string` object.
   */

  string Streamer::toString() const {
    stringstream stream;
    this->printToStream(stream);
    return stream.str();
  }
  
} // namespace kfoundation
