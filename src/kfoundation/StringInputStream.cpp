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
#include <string>
#include <cstring>

// Internal
#include "System.h"

// Self
#include "StringInputStream.h"

namespace kfoundation {
  
  using namespace std;
  
  
  /**
   * Constructor, uses the given string as input.
   *
   * @param str The string to read.
   */
  
  StringInputStream::StringInputStream(const string& str)
  : _mark(0),
    _pos(0),
    _str(str),
    _eof(str.size() == 0)
  {
    // Nothing;
  }
  
  
  /**
   * Deconstructor.
   */
  
  StringInputStream::~StringInputStream() {
    // Nothing;
  }
  
  
  kf_int32_t StringInputStream::read(kf_octet_t* buffer, const kf_int32_t nBytes)
  {
    kf_int32_t remainder = (kf_int32_t)_str.size() - _pos;
    kf_int32_t nReadableBytes = nBytes;
    if(remainder < nBytes) {
      nReadableBytes = remainder;
      _eof = true;
    }
    
    _pos += nBytes;
    
    memcpy(buffer, _str.c_str(), nReadableBytes);
    return nBytes;
  }
  
  
  int StringInputStream::read() {
    if(_eof) {
      return -1;
    }
    
    if(_pos >= _str.length() - 1) {
      _eof = true;
    }
    
    return (int)_str[_pos++];
  }
  
  
  int StringInputStream::peek() {
    if(_pos >= _str.length()) {
      _eof = true;
      return -1;
    }
    
    return _str[_pos];
  }
  
  
  kf_int32_t StringInputStream::skip(kf_int32_t nBytes) {
    kf_int32_t remainder = (kf_int32_t)_str.size() - _pos;
    if(remainder < nBytes) {
      nBytes = remainder;
      _eof = true;
    }
    
    _pos += nBytes;
    
    return nBytes;
  }
  
  
  bool StringInputStream::isEof() {
    return _eof;
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