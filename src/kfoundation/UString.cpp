//
//  UString.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// City Hash
#include <cityhash/city.h>

// Std
#include <cstring>

// Internal
#include "Ref.h"
#include "UChar.h"
#include "Array.h"
#include "OutputStream.h"
#include "BufferOutputStream.h"
#include "StringPrintWriter.h"

#include "Bool.h"
#include "Int32.h"
#include "Int64.h"
#include "Double.h"

// Self
#include "UString.h"

namespace kfoundation {
  
//\/ UString /\////////////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const StaticRefConst<UString> UString::EMPTY = new UString();


// --- (DE)CONSTRUCTORS --- //

  /**
   * Default constructor, creates an empty UString.
   */

  UString::UString()
  : _buffer(NULL),
    _hash(0)
  {
    // Nothing;
  }


  UString::UString(const char* cstring)
  : _buffer(makeBuffer((kf_octet_t*)cstring,
            cStringLength((kf_octet_t*)cstring)))
  {
    // Nothing;
  }


  /**
   * Constructs a new UString from an ASCII encoded C-UString.
   */

  UString::UString(const kf_octet_t* raw)
  : _buffer(makeBuffer(raw, cStringLength(raw))),
    _hash(0)
  {
    // Nothing
  }


  UString::UString(const kf_int32_t nOctets)
  : _buffer(makeBuffer(NULL, nOctets))
  {
    // Nothing
  }


  UString::UString(const kf_octet_t* raw, kf_int32_t size)
  : _buffer(makeBuffer(raw, size)),
    _hash(0)
  {
    // Nothing;
  }


  /**
   * Duplicates another UString object.
   */

  UString::UString(RefConst<UString> str)
  : _buffer(NULL),
    _hash(0)
  {
    set(str);
  }


  /**
   * Deconstructor, deletes the internal buffer.
   */

  UString::~UString() {
    if(NOT_NULL(_buffer)) {
      delete[] _buffer;
    }
  }


// --- STATIC METHODS --- //

  kf_int32_t UString::cStringLength(const kf_octet_t *src) {
    if(IS_NULL(src)) {
      return 0;
    }

    register const kf_octet_t* p = src;
    while(NOT_NULL(*p)) {
      p++;
    }
    return (kf_int32_t)(p - src);
  }


  kf_octet_t* UString::makeBuffer(const kf_octet_t* src, kf_int32_t size) {
    kf_octet_t* buffer = new kf_octet_t[size + HEADER_SIZE + 1];

    if(NOT_NULL(src)) {
      memcpy(buffer + HEADER_SIZE, src, size);
    }

    *(kf_int32_t*)buffer = size;

    buffer[HEADER_SIZE + size] = 0;
    return buffer;
  }
  
  
// --- METHODS --- //

  void UString::releaseOwner() const {
    // Nothing;
  }


  const kf_octet_t* UString::getExternalOctets() const {
    return NULL;
  }


  kf_int32_t UString::getExternalOctetCount() const {
    return 0;
  }


  const char* UString::getCString() const {
    if(IS_NULL(_buffer)) {
      _buffer = makeBuffer(getOctets(), getOctetCount());
      _hash = 0;
      releaseOwner();
    }

    return (const char*)(_buffer + HEADER_SIZE);
  }


  UChar UString::getUCharAtOffset(const kf_int32_t offset) const {
    return UChar(getOctets() + offset);
  }


  kf_int32_t UString::getHashCode() const {
    if(_hash == 0) {
      if(!isEmpty()) {
        _hash = (kf_int32_t)CityHash32((const char*)getOctets(), getOctetCount());
      }
    }
    return _hash;
  }


  /**
   * Returns the number of UTF-8 octets in this KFKFString.
   */
  
  kf_int32_t UString::getCharCount() const {
    register const kf_octet_t* p = getOctets();
    const kf_octet_t* end = p + getOctetCount();

    kf_int32_t n = 0;
    while(p < end) {
      p += UChar::getNumberOfUtf8Octets(*p);
      n++;
    }

    return n;
  }


  bool UString::isEmpty() const {
    return getOctetCount() == 0;
  }


  void UString::internalSet(kf_octet_t* buffer) {
    if(NOT_NULL(_buffer)) {
      delete[] _buffer;
    } else {
      releaseOwner();
    }

    _buffer = buffer;
    _hash = 0;
  }


  void UString::set(const kf_octet_t* raw) {
    internalSet(makeBuffer(raw, cStringLength(raw)));
  }


  void UString::set(const kf_octet_t* raw, kf_int32_t size) {
    internalSet(makeBuffer(raw, size));
  }


  void UString::set(RefConst<UString> other) {
    const UString& o = *other;
    internalSet(makeBuffer(o.getOctets(), o.getOctetCount()));
  }


  bool UString::equalsIgnoreCase(RefConst<UString> other) const {
    const register kf_octet_t* p1 = getOctets();
    const register kf_octet_t* p2 = other->getOctets();
    const kf_octet_t* e1 = p1 + getOctetCount();
    const kf_octet_t* e2 = p2 + other->getOctetCount();
    wchar_t ch1 = 0;
    wchar_t ch2 = 0;

    while(p1 < e1 && p2 < e2) {
      p1 += UChar::readUtf8(p1, ch1);
      p2 += UChar::readUtf8(p2, ch2);
      if(UChar::toLowercase(ch1) != UChar::toLowercase(ch2)) {
        return false;
      }
    }

    return true;
  }


  kf_int32_t UString::find(const kf_octet_t octet, const kf_int32_t offset)
  const
  {
    const kf_octet_t* const begin = getOctets();
    const kf_octet_t* const end = begin + getOctetCount();
    for(register const kf_octet_t* p = begin + offset; p < end; p++) {
      if(*p == octet) {
        return (kf_int32_t)(p - begin);
      }
    }
    return KF_NOT_FOUND;
  }


  kf_int32_t UString::find(UChar ch, const kf_int32_t offset) const {
    const kf_octet_t* begin = getOctets();

    const kf_octet_t* pos = OctetIterator::find(begin,
        begin + getOctetCount() + offset,
        ch.get(), ch.get() + ch.getOctetCount());

    if(IS_NULL(pos)) {
      return KF_NOT_FOUND;
    }

    return (kf_int32_t)(pos - begin);
  }


  kf_int32_t UString::find(RefConst<UString> str, kf_int32_t offset) const {
    const kf_octet_t* begin = getOctets();

    const kf_octet_t* pos = OctetIterator::find(begin,
        begin + getOctetCount() + offset, str->getOctets(),
        str->getOctets() + str->getOctetCount());

    if(IS_NULL(pos)) {
      return KF_NOT_FOUND;
    }

    return (kf_int32_t)(pos - begin);
  }


  Ref<UString> UString::toLowercase() const {
    BufferOutputStream stream(getOctetCount() + 1);

    UCharIterator it = getUCharIterator();
    for(it.first(); it.hasMore(); it.next()) {
      UChar lower = it.toLowercase();
      stream.write(lower.get(), lower.getOctetCount());
    }

    stream.write(0);

    return new UString(stream.getData(), stream.getSize());
  }


  Ref<UString> UString::toUppercase() const {
    BufferOutputStream stream(getOctetCount() + 1);

    UCharIterator it = getUCharIterator();
    for(it.first(); it.hasMore(); it.next()) {
      UChar upper = it.toUppercase();
      stream.write(upper.get(), upper.getOctetCount());
    }

    stream.write(0);

    return new UString(stream.getData(), stream.getSize());
  }

  
  Ref<UString>
  UString::subString(const kf_int32_t begin, const kf_int32_t end) const {
    return new UStringWindow(this, begin, end);
  }


  Ref<UString> UString::append(RefConst<UString> str) const {
    return append(str->getOctets(), str->getOctetCount());
  }
  
  
  Ref<UString> UString::append(const kf_octet_t* raw, const kf_int32_t size)
  const
  {
    kf_int32_t s1 = getOctetCount();
    kf_int32_t s2 = size;

    UString* newStr = new UString(s1 + s2);
    
    if(s1 > 0) {
      memcpy(newStr->_buffer + HEADER_SIZE, getOctets(), s1);
    }
    
    if(s2 > 0) {
      memcpy(newStr->_buffer + HEADER_SIZE + s1, raw, s2);
    }
    
    return newStr;
  }


//  std::wstring UString::toCppString() const {
//    wchar_t* buffer = new wchar_t[getOctetCount() * 3];
//    register wchar_t* p = buffer;
//
//
//    for(UCharIterator ch = getUCharIterator(); ch.hasMore(); ch.next(), p++) {
//      *p = ch.toWChar();
//    }
//
//    return std::wstring(buffer, p - buffer);
//  }

  
//  std::string UString::toCppUtf8String() const {
//    if(IS_NULL(_buffer)) {
//      return std::string();
//    }
//    return std::string((const char*)_buffer, _size);
//  }

  
  OctetIterator UString::getOctetIterator() const {
    return OctetIterator(this);
  }
  
  
  UCharIterator UString::getUCharIterator() const {
    return UCharIterator(this);
  }


  bool UString::equals(RefConst<UString> other) const {
    if(other.isNull()) {
      return false;
    }
    return getHashCode() == other->getHashCode();
  }

  
  void UString::printToStream(Ref<OutputStream> stream) const {
    stream->write(getOctets(), getOctetCount());
  }


  RefConst<UString> UString::toString() const {
    return this;
  }


  Ref<UString> operator+(RefConst<UString> str, const bool v) {
    return str->append(Bool(v).toString());
  }


  Ref<UString> operator+(RefConst<UString> str, const kf_int32_t v) {
    return str->append(Int32(v).toString());
  }


  Ref<UString> operator+(RefConst<UString> str, const kf_int64_t v) {
    return str->append(Int64(v).toString());
  }


  Ref<UString> operator+(RefConst<UString> str, const float v) {
    return str->append(Double(v).toString());
  }


  Ref<UString> operator+(RefConst<UString> str, const double v) {
    return str->append(Double(v).toString());
  }


  Ref<UString> operator+(RefConst<UString> str, const wchar_t v) {
    UChar ch(v);
    return str->append(ch.get(), ch.getOctetCount());
  }

  
  Ref<UString> operator+(RefConst<UString> str, const char* ch) {
    return str->append((const kf_octet_t*)ch, (kf_int32_t)strlen(ch));
  }
  

//  Ref<UString> operator+(RefConst<UString> str, const std::string& v) {
//    return str->append((const kf_octet_t*)v.c_str(), (kf_int32_t)v.length());
//  }


//  Ref<UString> operator+(RefConst<UString> str, const std::wstring& v) {
//    return str->append(new UString(v));
//  }


  Ref<UString> operator+(RefConst<UString> str, const Streamer& v) {
    return str->append(v.toString());
  }


  Ref<UString> operator+(RefConst<UString> str, RefConst<UString> v) {
    return str->append(v);
  }


//\/ UStringWindow /\//////////////////////////////////////////////////////////

// --- CONSTRUCTOR --- //

  UStringWindow::UStringWindow(RefConst<UString> str, kf_int32_t begin,
      kf_int32_t end)
  {
    _bufferOwner = str;
    _offset = begin;
    _size = end - begin;
  }


// --- METHODS --- //

  void UStringWindow::releaseOwner() const {
    _bufferOwner = NULL;
  }


//\/ UStringLiteral /\/////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const UStringLiteral UStringLiteral::EMPTY;


// --- CONSTRUCTORS --- //

  UStringLiteral::UStringLiteral() {
    _memory = NULL;
    _size = 0;
  }


  UStringLiteral::UStringLiteral(const kf_octet_t* memory, const kf_int32_t size)
  {
    _memory = memory;
    _size = size;
  }


// --- OPERATORS --- //

  RefConst<UString> UStringLiteral::operator+(const char *cstr) const {
    return new UStringLiteral((const kf_octet_t*)cstr, (kf_int32_t)strlen(cstr));
  }


//\/ OctetIterator /\//////////////////////////////////////////////////////////

// --- CONSTRUCTORS --- //

  OctetIterator::OctetIterator(RefConst<UString> str)
  : _bufferOwner(str),
    _begin(str->getOctets()),
    _end(_begin + str->getOctetCount()),
    _pos(_begin)
  {
    // Nothing
  }


  OctetIterator::OctetIterator(const kf_octet_t* octets, kf_int32_t size)
  : _bufferOwner(),
    _begin(octets),
    _end(octets + size),
    _pos(octets)
  {
    // Nothing;
  }


// --- STATIC METHODS --- //

  const kf_octet_t* OctetIterator::find(const kf_octet_t *seq1, const kf_octet_t* end1,
      const kf_octet_t *seq2, const kf_octet_t* end2)
  {
    const kf_octet_t first = *seq2;
    for(register const kf_octet_t* p1 = seq1; p1 < end1; p1++) {
      if(*p1 == first) {
        const kf_octet_t* p = p1;
        for(register const kf_octet_t* p2 = seq2; p2 < end2; p2++, p++) {
          if(*p != *p2) {
            break;
          }
          if(p >= end1) {
            return NULL;
          }
        }
        return p1;
      }
    }
    return NULL;
  }


// --- METHODS --- //

  kf_int32_t OctetIterator::seek(const kf_octet_t octet) {
    register const kf_octet_t* p = _pos;
    while(p < _end) {
      if(*p == octet) {
        _pos = p;
        return (kf_int32_t)(p - _begin);
      }
    }
    return KF_NOT_FOUND;
  }


  kf_int32_t OctetIterator::seek(UChar ch) {
    const kf_octet_t* p = find(_pos, _end, ch.get(), ch.get() + ch.getOctetCount());
    if(IS_NULL(p)) {
      return KF_NOT_FOUND;
    }
    _pos = p;
    return (kf_int32_t)(p - _begin);
  }


  kf_int32_t OctetIterator::seek(RefConst<UString> str) {
    const kf_octet_t* p = find(_pos, _end, str->getOctets(),
        str->getOctets() + str->getOctetCount());

    if(IS_NULL(p)) {
      return KF_NOT_FOUND;
    }

    _pos = p;
    return (kf_int32_t)(p - _begin);
  }


  kf_int32_t OctetIterator::getIndex() const {
    return (kf_int32_t)(_pos - _begin);
  }


  kf_octet_t OctetIterator::get() const {
    return *_pos;
  }


//\/ UCharIterator /\//////////////////////////////////////////////////////////

// --- CONSTRUCTOR --- //

  UCharIterator::UCharIterator(RefConst<UString> str)
  : UChar(str->getOctets()),
    _bufferOwner(str),
    _begin(str->getOctets()),
    _end(_begin + str->getOctetCount()),
    _pos(0),
    _charIndex(0)
  {
    first();
  }


// --- METHODS --- //

//  kf_int32_t UCharIterator::seek(UChar ch) {
//    const kf_octet_t* p = OctetIterator::find(_pos, _end, ch.get(),
//        ch.get() + ch.getOctetCount());
//
//    if(IS_NULL(p)) {
//      return KF_NOT_FOUND;
//    }
//
//    _pos = p;
//    return (kf_int32_t)(p - _begin);
//  }


//  kf_int32_t UCharIterator::seek(RefConst<kfoundation::UString> str) {
//    const kf_octet_t* p = OctetIterator::find(_pos, _end, str->getOctets(),
//        str->getOctets() + str->getOctetCount());
//
//    if(IS_NULL(p)) {
//      return KF_NOT_FOUND;
//    }
//
//    _pos = p;
//    return (kf_int32_t)(p - _begin);
//  }


  kf_int32_t UCharIterator::getOctetIndex() const {
    return (kf_int32_t)(_pos - _begin) - getOctetCount();
  }


  kf_int32_t UCharIterator::getCharIndex() const {
    return _charIndex;
  }

} // kfoundation