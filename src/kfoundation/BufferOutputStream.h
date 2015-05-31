//
//  BufferOutputStream.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/28/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__BufferOutputStream__
#define __KFoundation__BufferOutputStream__

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  class InputStream;
  
  class BufferOutputStream : public OutputStream {
  
  // --- FIELDS --- //
    
    private: kf_octet_t* _data;
    private: kf_int32_t _size;
    private: kf_int32_t _capacity;
    private: bool _debug;
    
    
  // --- STATIC METHODS --- //
    
    public: static string toBinaryString(const kf_octet_t* data, int size);
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: BufferOutputStream(const kf_int32_t capacity);
    public: ~BufferOutputStream();
    
    
  // --- METHODS --- //
    
    private: void grow();
    public: kf_octet_t* getData() const;
    public: kf_int32_t getSize() const;
    public: void print() const;
    public: void debug(bool on);
    
    // Inherited from OutputStream
    public: bool isBigEndian() const;
    public: void write(const kf_octet_t* buffer, const kf_int32_t nBytes);
    public: void write(kf_octet_t byte);
    public: void write(PPtr<InputStream> is);
    public: void close();
    
  };
}

#endif /* defined(__KFoundation__BufferOutputStream__) */
