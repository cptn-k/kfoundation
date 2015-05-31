//
//  StandardInputStreamAdapter.h
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__StandardInputStreamAdapter__
#define __KFoundation__StandardInputStreamAdapter__

// Internal
#include "definitions.h"

// Super
#include "InputStream.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
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
