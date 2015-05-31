//
//  UniString.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cstring>
#include <cityhash/city.h>

#include "Ptr.h"
#include "UniChar.h"
#include "LongInt.h"
#include "Array.h"

#include "UniString.h"


namespace kfoundation {

  using namespace std;

  void UniString::from(const char* const& cstr, kf_int32_t size) {
    _nOctets = 1;
    _nCodePoints = size;
    
    for(int i = 0; i < _nCodePoints; i++) {
      _nOctets += UniChar::getNumberOfUtf8Octets(cstr[i]);
    }
    
    _buffer = new kf_octet_t[_nOctets];
    
    kf_octet_t* p = _buffer;
    for(int i = 0; i < _nCodePoints; i++) {
      p += UniChar::writeUtf8(cstr[i], p);
    }
    _buffer[_nOctets - 1] = 0;
    
    recalculateHash();
  }
  
  void UniString::from(const wchar_t * const& cstr, kf_int32_t size) {
    _nOctets = 1;
    _nCodePoints = size;
    
    for(int i = 0; i < _nCodePoints; i++) {
      _nOctets += UniChar::getNumberOfUtf8Octets(cstr[i]);
    }
    
    _buffer = new kf_octet_t[_nOctets];
    
    kf_octet_t* p = _buffer;
    for(int i = 0; i < _nCodePoints; i++) {
      p += UniChar::writeUtf8(cstr[i], p);
    }
    _buffer[_nOctets - 1] = 0;
    
    recalculateHash();
  }
  
  void UniString::recalculateHash() {
    uint32 hash = CityHash32((char*)_buffer, _nOctets);
    _hash = *((kf_int32_t*)((void*)&hash));
  }
  
  UniString::UniString(kf_octet_t* externalBuffer) {
    _buffer = externalBuffer;
    short int n = 0;
    wchar_t ch = 0;
    
    for(_nOctets = 0, _nCodePoints = 0; _buffer[_nOctets] != 0; _nOctets += n)
    {
      n = UniChar::readUtf8(_buffer, ch);
      _nCodePoints++;
    }
    
    _nOctets++;
    
    recalculateHash();
  }
  
  UniString::UniString() {
    _nOctets = 0;
    _nCodePoints = 0;
    _buffer = NULL;
    _hash = 0;
  }
  
  UniString::UniString(const char* const& cstr) {
    from(cstr, (kf_int32_t)strlen(cstr));
  }
  
  UniString::UniString(const char* const& cstr, kf_int32_t size) {
    from(cstr, size);
  }
  
  UniString::UniString(const wchar_t* const& cstr) {
    from(cstr, (kf_int32_t)wcslen(cstr));
  }
  
  UniString::UniString(const wchar_t* const& cstr, kf_int32_t size) {
    from(cstr, (kf_int32_t)size);
  }
  
  UniString::UniString(const string& str) {
    from(str.c_str(),(kf_int32_t)str.length());
  }
  
  UniString::UniString(const wstring& str) {
    from(str.c_str(), (kf_int32_t)str.length());
  }
  
  UniString::UniString(const Ptr<UniString>& str) {
    _nOctets = str->_nOctets;
    _nCodePoints = str->_nCodePoints;
    _buffer = new kf_octet_t[_nOctets];
    memcpy(_buffer, str->_buffer, _nOctets);
  }
  
  UniString::~UniString() {
    if(_buffer != NULL) {
      delete[] _buffer;
    }
  }
  
  const kf_octet_t* UniString::raw() const {
    return _buffer;
  }
  
  kf_int32_t UniString::getLength() const {
    return _nCodePoints;
  }
  
  kf_int32_t UniString::getNumberOfOctets() const {
    return _nOctets;
  }
  
  kf_octet_t UniString::getOctetAt(const kf_int32_t& index) const
  throw(IndexOutOfBoundException)
  {
    if(index >= _nOctets) {
      throw(IndexOutOfBoundException("Attempt to read octet " + LongInt(index)
                                     + " from a string with " + LongInt(_nOctets)
                                     + " octets" ));
    }
    return _buffer[index];
  }
  
  wchar_t UniString::getCodePointAt(const kf_int32_t& index) const
  throw(IndexOutOfBoundException)
  {
    if(index >= _nCodePoints) {
      throw(IndexOutOfBoundException("Attempt to read codepoint " + LongInt(index)
                                     + " from a string with " + LongInt(_nCodePoints)
                                     + " codepoints"));
    }
    
    kf_octet_t* p = _buffer;
    wchar_t ch = 0;
    
    for(int i = 0; i < index; i++) {
      p += UniChar::readUtf8(p, ch);
    }
    
    return ch;
  }
  
  Ptr<UniChar> UniString::getCodePointAsUniChar(const kf_int32_t& index) const
  throw(IndexOutOfBoundException)
  {
    Ptr<UniChar> ch(new UniChar(getCodePointAt(index)));
    return ch.retain();
  }
  
  kf_int32_t UniString::getIndexOf(const wchar_t& ch) const {
    return getIndexOf(ch, 0);
  }
  
  kf_int32_t UniString::getIndexOf(const wchar_t& ch, const kf_int32_t& offset) const {
    kf_octet_t* p = _buffer;
    wchar_t tmp = 0;
    
    for(kf_int32_t i = offset; i < _nCodePoints; i++) {
      p += UniChar::readUtf8(p, tmp);
      if(tmp == ch) {
        return i;
      }
    }
    
    return NOT_FOUND;
  }
  
  bool UniString::isEmpty() const {
    return _nCodePoints == 0;
  }
  
  kf_int32_t UniString::getHash() const {
    return _hash;
  }
  
  bool UniString::equals(const Ptr<UniString>& str) const {
    return _hash == str->_hash;
  }
  
  bool UniString::equals(const string& str) const {
    Ptr<UniString> uniStr(new UniString(str));
    return this->equals(uniStr);
  }
  
  bool UniString::equals(const wstring& str) const {
    Ptr<UniString> uniStr(new UniString(str));
    return this->equals(uniStr);
  }
  
  bool UniString::equalsIgnoreCases(const UniString& str) const {
    if(str._nCodePoints != _nCodePoints) {
      return false;
    }
    
    kf_octet_t* p1 = _buffer;
    kf_octet_t* p2 = str._buffer;
    wchar_t ch1 = 0;
    wchar_t ch2 = 0;
    
    for(kf_int32_t i = 0; i < _nCodePoints; i++) {
      p1 += UniChar::readUtf8(p1, ch1);
      p2 += UniChar::readUtf8(p2, ch2);
      if(UniChar::toLowerCase(ch1) != UniChar::toLowerCase(ch2)) {
        return false;
      }
    }
    
    return true;
  } //very long password
  
  Ptr<UniString> UniString::duplicateUniString() const {
    Ptr<UniString> newstr(new UniString(*this));
    return newstr;
  }
  
  Ptr<UniString> UniString::duplicateToLowerCase() const {
    Ptr<UniString> newstr(new UniString());
    newstr->_nCodePoints = _nCodePoints;
    newstr->_nOctets = _nOctets;
    newstr->_buffer = new kf_octet_t[_nOctets];
    
    kf_octet_t* p1 = _buffer;
    kf_octet_t* p2 = newstr->_buffer;
    wchar_t ch = 0;
    
    for(kf_int32_t i = 0; i < _nCodePoints; i++) {
      p1 += UniChar::readUtf8(p1, ch);
      p2 += UniChar::writeUtf8(UniChar::toLowerCase(ch), p2);
    }
    
    newstr->recalculateHash();
    
    return newstr;
  }
  
  Ptr<UniString> UniString::duplicateToUpperCase() const {
    Ptr<UniString> newstr(new UniString());
    newstr->_nCodePoints = _nCodePoints;
    newstr->_nOctets = _nOctets;
    newstr->_buffer = new kf_octet_t[_nOctets];
    
    kf_octet_t* p1 = _buffer;
    kf_octet_t* p2 = newstr->_buffer;
    wchar_t ch = 0;
    
    for(kf_int32_t i = 0; i < _nCodePoints; i++) {
      p1 += UniChar::readUtf8(p1, ch);
      p2 += UniChar::writeUtf8(UniChar::toUpperCase(ch), p2);
    }
    
    newstr->recalculateHash();
    
    return newstr;
  }
  
  Ptr<UniString>
  UniString::substring(const kf_int32_t& offset, const kf_int32_t& count) const
  {
    kf_int32_t last = min(offset + count, _nCodePoints);
    
    kf_octet_t* begin = _buffer;
    wchar_t ch = 0;
    
    for(int i = 0; i < offset && i < last; i++) {
      begin += UniChar::readUtf8(begin, ch);
    }
    
    kf_octet_t* end = begin;
    
    for(int i = offset; i < last; i++) {
      end += UniChar::readUtf8(end, ch);
    }
    
    Ptr<UniString> newstr(new UniString());
    newstr->_nOctets = (kf_int32_t)(begin - end) + 1;
    newstr->_nCodePoints = last - offset;
    newstr->_buffer = new kf_octet_t[newstr->_nOctets];
    memcpy(newstr->_buffer, begin, newstr->_nOctets - 1);
    newstr->_buffer[newstr->_nOctets - 1] = 0;
    
    return newstr;
  }
  
  Ptr<UniString> UniString::append(const Ptr<UniString>& str) const {
    Ptr<UniString> newstr(new UniString());
    
    newstr->_nOctets = _nOctets + str->_nOctets - 1;
    newstr->_nCodePoints = _nCodePoints + str->_nCodePoints;
    newstr->_buffer = new kf_octet_t[newstr->_nOctets];
    memcpy(newstr->_buffer, _buffer, _nOctets - 1);
    memcpy(newstr->_buffer + _nOctets - 1, str->_buffer, str->_nOctets);
    
    return newstr;
  }
  
  Ptr<UniString>
  UniString::replace(const wchar_t &a, const wchar_t &b) const {
    Ptr< Array<kf_int32_t> > locations(new Array<kf_int32_t>());
    
    short int aSize = UniChar::getNumberOfUtf8Octets(a);
    
    kf_octet_t bInUtf8[6];
    short int bSize = UniChar::writeUtf8(b, bInUtf8);
    
    wchar_t ch = 0;
    short int chSize = 0;
    
    for(kf_int32_t i = 0; i < _nOctets; i += chSize) {
      chSize = UniChar::readUtf8(_buffer + i, ch);
      if(ch == a) {
        locations->push(i);
      }
    }
    
    int change = (bSize - aSize) * locations->getSize();
    
    kf_octet_t *newBuffer = new kf_octet_t[_nOctets + change];
    
    kf_int32_t n = locations->getSize();
    kf_int32_t offset1 = 0;
    kf_int32_t offset2 = 0;
    
    for(kf_int32_t i = 0; i < n; i++) {
      kf_int32_t loc = locations->at(i);
      kf_int32_t count = loc - offset1;
      memcpy(newBuffer + offset2, _buffer + offset1, count);
      
      offset1 += count;
      offset2 += count;
      
      memcpy(newBuffer + offset2, bInUtf8, bSize);
      
      offset1 += aSize;
      offset2 += bSize;
    }
    
    memcpy(newBuffer + offset2, _buffer + offset1, _nOctets - offset1);
    
    Ptr<UniString> str(new UniString(newBuffer));
    return str;
  }
  
  wstring UniString::toWString() const {
    wchar_t* chars = new wchar_t[_nCodePoints];
    kf_octet_t* p = _buffer;
    for(int i = 0; i < _nCodePoints; i++) {
      p += UniChar::readUtf8(p, chars[i]);
    }
    return wstring(chars, _nCodePoints);
  }
  
  string UniString::toUtf8String() const {
    if(_nOctets <= 1) {
      return "";
    }
    return string((char*)_buffer, _nOctets - 1);
  }
  
  bool UniString::areEqualIgnoreCases(const string& first, const string& second) {
    if(first.length() != second.length()) {
      return false;
    }
    
    for(long int i = first.length() - 1; i >= 0; i--) {
      if(tolower(first[i]) != tolower(second[i])) {
        return false;
      }
    }
    
    return true;
  }
  
  void UniString::printToStream(ostream& os) const {
    if(_nOctets <= 1) {
      return;
    }
    
    os.write((char*)_buffer, _nOctets - 1);
  }
  
  bool UniString::equals(const ManagedObject& obj) const {
    const UniString* ptr = dynamic_cast<const UniString*>(&obj);
    if(ptr == 0) {
      return false;
    }
    return _hash == ptr->_hash;
  }
  
} // kfoundation
