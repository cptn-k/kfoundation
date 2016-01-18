/*---[BufferInputStream.h]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::BufferInputStream::*
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

#ifndef __KFoundation__BufferInputStream__
#define __KFoundation__BufferInputStream__

// Internal
#include "definitions.h"

// Super
#include "InputStream.h"

namespace kfoundation {
  
  /**
   * Input stream to read from a buffer in memory.
   *
   * @ingroup io
   * @headerfile BufferInputStream.h <kfoundation/BufferInputStream.h>
   */
  
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
    public: kf_int32_t read(kf_octet_t* buffer, kf_int32_t nBytes);
    public: kf_int16_t read();
    public: kf_int16_t peek();
    public: kf_int32_t skip(kf_int32_t bytes);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__BufferInputStream__) */