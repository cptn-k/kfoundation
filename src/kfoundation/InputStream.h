/*---[InputStream.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: InputStream
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

#ifndef KFOUNDATION_INPUTSTREAM
#define KFOUNDATION_INPUTSTREAM

#include "definitions.h"
#include "ManagedObject.h"
#include "PtrDecl.h"

using namespace std;

namespace kfoundation {
  
  class InputStream : public ManagedObject {
  public:
    virtual kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes) = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual kf_int32_t skip(kf_int32_t bytes) = 0;
    virtual bool isEof() = 0;
    virtual bool isMarkSupported() = 0;
    virtual void mark() = 0;
    virtual void reset() = 0;
    virtual bool isBigEndian() = 0;
  };
  
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_INPUTSTREAM) */
