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

/**
 *
 * @file
 * Implementation of `kfoundation::Streamer` class.
 *
 */

// Std
#include <sstream>

// Internal
#include "System.h"
#include "LongInt.h"

// Self
#include "Streamer.h"

namespace kfoundation {
  
  string Streamer::toString() const {
    stringstream stream;
    this->printToStream(stream);
    return stream.str();
  }
  
} // namespace kfoundation
