/*---[BufferOutputStream.h]------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::BufferOutputStream::*
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

#ifndef __KFoundation__BufferOutputStream__
#define __KFoundation__BufferOutputStream__

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  class InputStream;
  
  
  /**
   * Output stream used to write to a buffer in memory.
   *
   * @ingroup io
   * @headerfile BufferOutputStream.h <kfoundation/BufferOutputStream.h>
   */
  
  class BufferOutputStream : public OutputStream {
  
  // --- FIELDS --- //
    
    private: kf_octet_t* _data;
    private: kf_int32_t _size;
    private: kf_int32_t _capacity;
    
    
  // --- STATIC METHODS --- //
    
    public: static string toBinaryString(const kf_octet_t* data, int size);
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: BufferOutputStream(const kf_int32_t capacity);
    public: ~BufferOutputStream();
    
    
  // --- METHODS --- //
    
    private: void grow();
    public: kf_octet_t* getData() const;
    public: kf_int32_t getSize() const;
    
    // Inherited from OutputStream
    public: bool isBigEndian() const;
    public: void write(const kf_octet_t* buffer, const kf_int32_t nBytes);
    public: void write(kf_octet_t byte);
    public: void write(PPtr<InputStream> is);
    public: void close();
    
  };
  
} // kfoundation

#endif /* defined(__KFoundation__BufferOutputStream__) */
