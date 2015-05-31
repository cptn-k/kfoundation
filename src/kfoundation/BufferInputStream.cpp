//
//  BufferInputStream.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/28/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

// Std
#include <cstring>

// Internal
#include "System.h"

// Self
#include "BufferInputStream.h"

namespace kfoundation {

// --- (DE)CONSTRUCTORS --- //
  
  BufferInputStream::BufferInputStream(const kf_octet_t* const buffer,
      const kf_int32_t size, bool takeover)
  {
    _buffer = buffer;
    _size = size;
    _takeover = takeover;
    _position = 0;
    _mark = 0;
  }
  
  
  BufferInputStream::~BufferInputStream() {
    if(_takeover) {
      delete[] _buffer;
    }
  }
  
  
// --- METHODS --- //
  
  kf_int32_t BufferInputStream::getSize() const {
    return _size;
  }
  
  kf_int32_t BufferInputStream::read(kf_octet_t* buffer, const kf_int32_t nBytes)
  {
    kf_int32_t nReadableBytes = min(nBytes, _size - _position);
    memcpy(buffer, _buffer + _position, nReadableBytes);
    _position += nBytes;
    return nReadableBytes;
  }
  
  
  int BufferInputStream::read() {
    if(_position == _size) {
      return -1;
    }
    
    return _buffer[_position++];
  }
  
  
  int BufferInputStream::peek() {
    if(_position == _size) {
      return -1;
    }
    
    return _buffer[_position];
  }
  
  
  kf_int32_t BufferInputStream::skip(kf_int32_t bytes) {
    bytes = min(bytes, _size - _position);
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
