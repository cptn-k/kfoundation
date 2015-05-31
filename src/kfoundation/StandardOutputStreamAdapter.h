//
//  StandardOutputStreamAdapter.h
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__StandardOutputStreamAdapter__
#define __KFoundation__StandardOutputStreamAdapter__

// Internal
#include "definitions.h"

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  using namespace std;
  
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
