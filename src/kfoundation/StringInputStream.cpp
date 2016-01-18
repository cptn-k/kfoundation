/*---[StringInputStream.cpp]-----------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::StringInputStream::*
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

// Internal
#include "System.h"
#include "Ref.h"
#include "UString.h"

// Self
#include "StringInputStream.h"

namespace kfoundation {
  
// --- CONSTRUCTORS --- //
  
  /**
   * Constructor, uses the given string as input.
   *
   * @param str The string to read.
   */
  
  StringInputStream::StringInputStream(RefConst<UString> str)
  : _str(str),
    _pos(str->getOctets()),
    _end(str->getOctets() + str->getOctetCount()),
    _mark(_pos)
  {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  kf_int32_t StringInputStream::read(kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    kf_int32_t remainder = (kf_int32_t)(_end - _pos);
    kf_int32_t nReadableBytes = nBytes;
    if(remainder < nBytes) {
      nReadableBytes = remainder;
    }
    
    memcpy(buffer, _pos, nReadableBytes);

    _pos += nReadableBytes;
    return nReadableBytes;
  }
  
  
  kf_int16_t StringInputStream::read() {
    if(_pos < _end) {
      kf_int16_t value = *_pos;
      _pos++;
      return value;
    }
    return -1;
  }
  
  
  kf_int16_t StringInputStream::peek() {
    if(_pos < _end) {
      return *_pos;
    }
    return -1;
  }
  
  
  kf_int32_t StringInputStream::skip(kf_int32_t nBytes) {
    kf_int32_t remainder = (kf_int32_t)(_end - _pos);
    kf_int32_t nReadableBytes = nBytes;
    if(remainder < nBytes) {
      nReadableBytes = remainder;
    }

    _pos += nReadableBytes;
    
    return nReadableBytes;
  }
  
  
  bool StringInputStream::isEof() {
    return _pos >= _end;
  }
  
  
  bool StringInputStream::isMarkSupported() {
    return true;
  }
  
  
  void StringInputStream::mark() {
    _mark = _pos;
  }
  
  
  void StringInputStream::reset() {
    _pos = _mark;
  }
  
  
  bool StringInputStream::isBigEndian() {
    return System::isBigEndian();
  }
  
} // namespace kfoundation