/*---[OutputStream.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: OutputStream
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

#ifndef KFoundation_OutputStream_h
#define KFoundation_OutputStream_h

#include "definitions.h"
#include "ManagedObject.h"
#include "PtrDecl.h"

using namespace std;

namespace kfoundation {
  
  class InputStream;
  
  class OutputStream : public ManagedObject {
  public:
    virtual bool isBigEndian() const = 0;
    virtual void write(const kf_octet_t* buffer, const kf_int32_t nBytes) = 0;
    virtual void write(kf_octet_t byte) = 0;
    virtual void write(PPtr<InputStream> os) = 0;
    virtual void close() = 0;
  };
  
} // namespace kfoundation


#endif
