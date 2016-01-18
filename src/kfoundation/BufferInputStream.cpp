/*---[BufferInputStream.cpp]-----------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::BufferInputStream::*
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

// Std
#include <cstring>
#include <algorithm>

// Internal
#include "System.h"

// Self
#include "BufferInputStream.h"

namespace kfoundation {

// --- (DE)CONSTRUCTORS --- //
  
  
  /**
   * Constructor.
   *
   * @param buffer The buffer to read from.
   * @param size Size of the buffer to read.
   * @param takeover If set `true` the buffer will be deleted once this object
   *                 is deconstructed. The default value is `false`.
   */
  
  BufferInputStream::BufferInputStream(const kf_octet_t* const buffer,
      const kf_int32_t size, bool takeover)
  {
    _buffer = buffer;
    _size = size;
    _takeover = takeover;
    _position = 0;
    _mark = 0;
  }
  
  
  /**
   * Deconstructor.
   */
  
  BufferInputStream::~BufferInputStream() {
    if(_takeover) {
      delete[] _buffer;
    }
  }
  
  
// --- METHODS --- //
  
  kf_int32_t BufferInputStream::getSize() const {
    return _size;
  }
  
  
  kf_int32_t BufferInputStream::read(kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    kf_int32_t nReadableBytes = std::min(nBytes, _size - _position);
    memcpy(buffer, _buffer + _position, nReadableBytes);
    _position += nBytes;
    return nReadableBytes;
  }
  
  
  kf_int16_t BufferInputStream::read() {
    if(_position == _size) {
      return -1;
    }
    
    return _buffer[_position++];
  }
  
  
  kf_int16_t BufferInputStream::peek() {
    if(_position == _size) {
      return -1;
    }
    
    return _buffer[_position];
  }
  
  
  kf_int32_t BufferInputStream::skip(kf_int32_t bytes) {
    bytes = std::min(bytes, _size - _position);
    _position += bytes;
    return bytes;
  }
  
  
  bool BufferInputStream::isEof() {
    return _position == _size;
  }
  
  
  bool BufferInputStream::isMarkSupported() {
    return true;
  }
  
  
  void BufferInputStream::mark() {
    _mark = _position;
  }
  
  
  void BufferInputStream::reset() {
    _position = _mark;
  }
  
  
  bool BufferInputStream::isBigEndian() {
    return System::isBigEndian();
  }

} // namespace kfoundation