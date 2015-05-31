//
//  BufferInputStream.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/28/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__BufferInputStream__
#define __KFoundation__BufferInputStream__

#include "InputStream.h"

namespace kfoundation {
  
  class BufferInputStream : public InputStream {
  
  // --- FIELDS --- //
    
    private: const kf_octet_t* _buffer;
    private: kf_int32_t _size;
    private: kf_int32_t _position;
    private: kf_int32_t _mark;
    private: bool _takeover;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: BufferInputStream(const kf_octet_t* const buffer,
        const kf_int32_t size, bool takover);
    
    public: ~BufferInputStream();
    
    
  // --- METHODS --- //
    
    public: kf_int32_t getSize() const;
    
    // From InputStream
    kf_int32_t read(kf_octet_t* buffer, kf_int32_t nBytes);
    int read();
    int peek();
    kf_int32_t skip(kf_int32_t bytes);
    bool isEof();
    bool isMarkSupported();
    void mark();
    void reset();
    bool isBigEndian();
    
  };
  
}


#endif /* defined(__KFoundation__BufferInputStream__) */
