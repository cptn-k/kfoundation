/*---[InternetAddress.cpp]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::InputStream::*
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


#ifndef KFOUNDATION_INPUTSTREAM
#define KFOUNDATION_INPUTSTREAM

#include "definitions.h"
#include "ManagedObject.h"
#include "PtrDecl.h"

using namespace std;

namespace kfoundation {
  
  /**
   * Abstract interface for all input streams.
   * @headerfile InputStream.h <kfoundation/InputStream.h>
   */
  
  class InputStream : public ManagedObject {
    public: virtual kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes) = 0;
    public: virtual int read() = 0;
    public: virtual int peek() = 0;
    public: virtual kf_int32_t skip(kf_int32_t bytes) = 0;
    public: virtual bool isEof() = 0;
    public: virtual bool isMarkSupported() = 0;
    public: virtual void mark() = 0;
    public: virtual void reset() = 0;
    public: virtual bool isBigEndian() = 0;
  };
  
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_INPUTSTREAM) */
