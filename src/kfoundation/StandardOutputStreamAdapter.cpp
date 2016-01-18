/*---[StandardOutputStreamAdapter.cpp]-------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::StandardOutputStreamAdapter::*
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

// Std
#include <ostream>

// Internal
#include "Ref.h"
#include "System.h"
#include "InputStream.h"
#include "KFException.h"

// Self
#include "StandardOutputStreamAdapter.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor, wraps the new object around a standard C++ `ostream` object.
   */
  
  StandardOutputStreamAdapter::StandardOutputStreamAdapter(std::ostream& os)
  : _os(os)
  {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  bool StandardOutputStreamAdapter::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  void StandardOutputStreamAdapter::write(const kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    _os.write((char*)buffer, nBytes);
  }
  
  
  void StandardOutputStreamAdapter::write(kf_octet_t byte) {
    _os.put(byte);
  }
  
  
  void StandardOutputStreamAdapter::write(Ref<InputStream> is) {
    kf_octet_t buffer[4096];
    while (!is->isEof()) {
      kf_int32_t s = is->read(buffer, 4069);
      write(buffer, s);
    }
  }
  
  
  void StandardOutputStreamAdapter::close() {
    // Nothing
  }


  void StandardOutputStreamAdapter::flush() {
    _os.put('\n');
    _os.flush();
  }
  
} // namespace kfoundation