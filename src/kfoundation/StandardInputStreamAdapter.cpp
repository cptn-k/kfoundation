//
//  StandardInputStreamAdapter.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Std
#include <istream>

// Internal
#include "System.h"

// Self
#include "StandardInputStreamAdapter.h"


namespace kfoundation {
  
// --- (DE)CONSTGRUCTORS --- //
  
  StandardInputStreamAdapter::StandardInputStreamAdapter(istream& stream)
  : _is(stream)
  {
    mark();
  }
  
  
// --- METHODS --- //
  
  
  kf_int32_t StandardInputStreamAdapter::read(kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    _is.read((char*)buffer, nBytes);
    return (kf_int32_t)_is.gcount();
  }
  
  
  int StandardInputStreamAdapter::read() {
    return _is.get();
  }
  
  
  int StandardInputStreamAdapter::peek() {
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