//
//  StandardOutputStreamAdapter.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Std
#include <ostream>

// Internal
#include "Ptr.h"
#include "System.h"
#include "InputStream.h"
#include "KFException.h"

// Self
#include "StandardOutputStreamAdapter.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  StandardOutputStreamAdapter::StandardOutputStreamAdapter(ostream& os)
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
  
  
  void StandardOutputStreamAdapter::write(PPtr<InputStream> is) {
    kf_octet_t buffer[4096];
    while (!is->isEof()) {
      kf_int32_t s = is->read(buffer, 4069);
      write(buffer, s);
    }
  }
  
  
  void StandardOutputStreamAdapter::close() {
    throw KFException("close() not supported.");
  }
  
} // namespace kfoundation