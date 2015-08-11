/*---[StandardInputStreamAdapter.h]----------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::StandardInputStreamAdapter::*
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

#ifndef __KFoundation__StandardInputStreamAdapter__
#define __KFoundation__StandardInputStreamAdapter__

// Internal
#include "definitions.h"

// Super
#include "InputStream.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  /**
   * Wraps around the given `istream` (C++ standard libraries) to be read as a
   * a KFoundation input stream.
   *
   * @ingroup io
   * @headerfile StandardInputStreamAdapter.h <kfoundation/StandardInputStreamAdapter.h>
   */
  
  class StandardInputStreamAdapter : public InputStream {
    
  // --- FIELDS --- //
  
    private: istream& _is;
    private: istream::pos_type _mark;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: StandardInputStreamAdapter(istream& stream);
    
    
  // --- METHODS --- //
  
    // Inherited from InputStream //
    public: kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes);
    public: int read();
    public: int peek();
    public: kf_int32_t skip(kf_int32_t nBytes);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();
    
  };
  
} // namespace kfoundation
#endif /* defined(__KFoundation__StandardInputStreamAdapter__) */
