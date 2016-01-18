/*---[StandardInputStreamAdapter.cpp]--------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::StandardInputStreamAdapter::*
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


// Internal
#include "System.h"

// Self
#include "StandardInputStreamAdapter.h"


namespace kfoundation {
  
// --- (DE)CONSTGRUCTORS --- //
  
  /**
   * Constructor, wraps the new instance around the given `istream` object.
   */
  
  StandardInputStreamAdapter::StandardInputStreamAdapter(std::istream& stream)
  : _is(stream)
  {
    // nothing
  }
  
  
// --- METHODS --- //
  
  
  kf_int32_t StandardInputStreamAdapter::read(kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    _is.read((char*)buffer, nBytes);
    return (kf_int32_t)_is.gcount();
  }
  
  
  kf_int16_t StandardInputStreamAdapter::read() {
    return _is.get();
  }
  
  
  kf_int16_t StandardInputStreamAdapter::peek() {
    return _is.peek();
  }
  
  
  kf_int32_t StandardInputStreamAdapter::skip(kf_int32_t nBytes) {
    _is.ignore(nBytes);
    return (kf_int32_t)_is.gcount();
  }
  
  
  bool StandardInputStreamAdapter::isEof() {
    return _is.eof();
  }
  
  
  bool StandardInputStreamAdapter::isMarkSupported() {
    return true;
  }
  
  
  void StandardInputStreamAdapter::mark() {
    _mark = _is.tellg();
  }
  
  
  void StandardInputStreamAdapter::reset() {
    _is.seekg(_mark);
  }
  
  
  bool StandardInputStreamAdapter::isBigEndian() {
    return System::isBigEndian();
  }
  
  
} // namespace kfoundation