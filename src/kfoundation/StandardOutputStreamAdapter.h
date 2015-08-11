/*---[StandardOutputStreamAdapter.h]---------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::StandardOutputStreamAdapter::*
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

#ifndef __KFoundation__StandardOutputStreamAdapter__
#define __KFoundation__StandardOutputStreamAdapter__

// Internal
#include "definitions.h"

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  using namespace std;
  
  /**
   * KFoundation wrapper for C++ `ostream`.
   *
   * @ingroup io
   * @headerfile StandardOutputStreamAdapter.h <kfoundation/StandardOutputStreamAdapter.h>
   */
  
  class StandardOutputStreamAdapter : public OutputStream {
    
  // --- FIELDS --- //
    
      private: ostream& _os;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: StandardOutputStreamAdapter(ostream& os);
    
    
  // --- METHODS --- //
    
    // Inherited from OutputStream //
    public: bool isBigEndian() const;
    public: void write(const kf_octet_t* buffer, const kf_int32_t nBytes);
    public: void write(kf_octet_t byte);
    public: void write(PPtr<InputStream> os);
    public: void close();
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__StandardOutputStreamAdapter__) */
