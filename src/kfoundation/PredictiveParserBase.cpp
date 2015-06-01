//
//  PredictiveParserBase.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cmath>
#include <cstring>

#include "UniChar.h"
#include "PredictiveParserBase.h"

#define KF_DOT_CHAR '.'
#define KF_E_ALLCASES (const wchar_t[2]){L'e', L'E'}
#define KF_PLUS_MINUS (const wchar_t[2]){L'+', L'-'}
#define KF_NEWLINE_CARRIAGERETURN (const wchar_t[2]){L'\n', L'\r'}

namespace kfoundation {

  using namespace std;
  
  bool PredictiveParserBase::isValidIdentifierBeginChar(const wchar_t& ch) const {
    return isAlphabet(ch) || ch == '_';
  }
  
  bool PredictiveParserBase::isValidIdentifierChar(const wchar_t &ch) const {
    return isAlphanumeric(ch) || ch == '_';
  }
  
  bool PredictiveParserBase::isSpace(const wchar_t &ch) const {
    return ch == L' ';
  }

  unsigned short int PredictiveParserBase::read(wchar_t& ch, kf_octet_t* readOctets) {
    if(_bufferPos < _bufferSize) {
      unsigned short int n = UniChar::readUtf8(_buffer + _bufferPos, ch);
      
      if(_bufferPos + n > BUFFER_CAPACITY) {
        throw new KFException("Buffer overload");
      }
      
      if(readOctets != NULL) {
        memcpy(readOctets, _buffer + _bufferPos, n);
      }
      _bufferPos += n;
      
      if(ch == '\n') {
        _tmpLine++;
        _tmpCol = 1;
      } else if(ch != '\r') {
        _tmpCol++;
      }
      _tmpByteIndex += n;
      _tmpCharIndex += 1;
      
      return n;
    }
    
    if(_stream->isEof()) {
      _eof = true;
      return 0;
    }
    
    unsigned short int n = UniChar::readUtf8(_stream, ch, _buffer + _bufferPos);
    if(readOctets != NULL) {
      memcpy(readOctets, _buffer + _bufferPos, n);
    }
    _bufferSize += n;
    _bufferPos = _bufferSize;
    
    if(ch == '\n') {
      _tmpLine++;
      _tmpCol = 1;
    } else if(ch != '\r') {
      _tmpCol++;
    }
    _tmpByteIndex += n;
    _tmpCharIndex += 1;
    
    return n;
  }
  
  void PredictiveParserBase::commit(const unsigned short int& n) {
    _location.set(_tmpLine, _tmpCol, _tmpByteIndex, _tmpCharIndex);
    
    if(n < _bufferSize) {
      memcpy(_buffer, _buffer+n, _bufferSize-n);
      _bufferPos -= n;
      _bufferSize -= n;
    } else {
      _bufferSize = 0;
      _bufferPos = 0;
    }
  }
  
  void PredictiveParserBase::rollback(const unsigned short int& n) {
    _tmpLine = _location.getLine();
    _tmpCol = _location.getCol();
    _tmpByteIndex = _location.getByteIndex();
    _tmpCharIndex = _location.getCharIndex();
    
    _bufferPos -= n;
  }
  
  unsigned short int PredictiveParserBase::read(kf_octet_t* buffer) {
    wchar_t ch = 0;
    return read(ch, buffer);
  }
  
  wchar_t PredictiveParserBase::readAndRollback() {
    wchar_t ch = 0;
    rollback(read(ch, NULL));
    return ch;
  }
  
  PredictiveParserBase::PredictiveParserBase(PPtr<InputStream> input)
    : _bufferSize(0),
      _bufferPos(0),
      _eof(input->isEof()),
      _tmpLine(1),
      _tmpCol(1),
      _tmpByteIndex(0),
      _tmpCharIndex(0)
  {
    _stream = input;
    _location.setLine(1);
  }
  
  PredictiveParserBase::~PredictiveParserBase() {
    // Nothing;
  }
  
  bool PredictiveParserBase::testChar(const wchar_t& ch) {
    return ch == readAndRollback() && !_eof;
  }
  
  bool PredictiveParserBase::testChar(const wchar_t* chars, const int& n) {
    wchar_t readChar = readAndRollback();
    
    if(_eof) {
      return false;
    }
    
    for(int i = 0; i < n; i++) {
      if(readChar == chars[i]) {
        return true;
      }
    }
    
    return false;
  }
  
  bool PredictiveParserBase::testAlphabet() {
    return isAlphabet(readAndRollback()) && !_eof;
  }
  
  bool PredictiveParserBase::testAlphanumeric() {
    return isAlphanumeric(readAndRollback()) && !_eof;
  }
  
  bool PredictiveParserBase::testSpace() {
    return isSpace(readAndRollback()) && !_eof;
  }
  
  bool PredictiveParserBase::testNewLine() {
    return readAndRollback() == '\n' && !_eof;
  }
  
  bool PredictiveParserBase::testEndOfStream() {
    return _stream->isEof();
  }
  
  bool PredictiveParserBase::testSequence(const wstring& str) {
    wchar_t ch = 0;
    size_t l = str.length();
    unsigned short int total = 0;
    
    for(int i = 0; i < l; i++) {
      total += read(ch, NULL);
      if(ch != str[i] || _eof) {
        rollback(total);
        return false;
      }
    }
    
    rollback(total);
    return true;
  }
  
  unsigned short int PredictiveParserBase::readChar(const wchar_t& t, kf_octet_t* octets) {
    wchar_t ch = 0;
    unsigned short int s = read(ch, octets);
    
    if(ch == t) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readChar(const wchar_t* chars, const int& n, kf_octet_t* octets) {
    wchar_t ch = 0;
    unsigned short int s = read(ch, octets);
    
    for(int i = 0; i < n; i++) {
      if(chars[i] == ch) {
        commit(s);
        return s;
      }
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readAlphabet(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isAlphabet(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readNumeric(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isNumeric(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readNumeric(unsigned short int& digit) {
    wchar_t ch = 0;
    unsigned short int n = readNumeric(ch, NULL);
    if(n == 0) {
      return 0;
    }
    
    digit = getNumericalValueOf(ch);
    return n;
  }
  
  unsigned short int PredictiveParserBase::readAlphanumeric(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isAlphanumeric(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readIdentifierBeginChar(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isValidIdentifierBeginChar(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readIdentifierChar(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isValidIdentifierChar(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readSpace() {
    wchar_t ch = 0;
    unsigned short int s = read(ch, NULL);
    if(isSpace(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readNewLine() {
    wchar_t ch = 0;
    unsigned short int s = read(ch, NULL);
    
    if(ch == '\n') {
      commit(s);
      
      unsigned short int s2 = read(ch, NULL);
      if(ch == '\r') {
        commit(s2);
        return s + s2;
      }
      rollback(s2);
      
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  unsigned short int PredictiveParserBase::readAny(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int n = read(ch, octets);
    commit(n);
    return n;
  }
  
  unsigned short int PredictiveParserBase::readSequence(const wstring& str) {
    wchar_t ch = 0;
    unsigned short int s = 0;
    
    size_t l = str.length();
    for(int i = 0; i < l; i++) {
      s += read(ch, NULL);
      if(ch != str[i]) {
        rollback(s);
        return false;
      }
    }
    
    commit(s);
    return s;
  }
  
  size_t PredictiveParserBase::readAllAlphabet(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch = 0;
    
    while((n = readAlphabet(ch, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllAlphanumeric(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch = 0;

    while((n = readAlphanumeric(ch, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllNumeric(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch = 0;
    
    while((n = readNumeric(ch, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readNumber(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    
    if((n = readChar(KF_PLUS_MINUS, 2, buffer)) != 0) {
      storage.append((char*)buffer, n);
    }
    
    total += readAllNumeric(storage);
    
    if((n =readChar('.', buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += 1;
    }
    
    total += readAllNumeric(storage);
    
    if((n = readChar(KF_E_ALLCASES, 2, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += readAllNumeric(storage);
    }

    return total;
  }
  
  size_t PredictiveParserBase::readNumber(long int& output) {
    size_t total = 0;
    unsigned short int n = 0;
    unsigned short int digit = 0;
    
    output = 0;
    long int factor = 1;
    
    if((n = readChar('-', NULL)) != 0) {
      factor = -1;
      total += n;
    } else {
      total += readChar('+', NULL);
    }
    
    while((n = readNumeric(digit)) != 0) {
      output += digit;
      output *= 10;
      total += n;
    }
    
    output = output * factor / 10;
    
    return total;
  }
  
  size_t PredictiveParserBase::readNumber(double& output) {
    size_t total = 0;
    unsigned short int n = 0;
    unsigned short int digit = 0;
    
    output = 0;
    double factor = 1;
    
    if((n = readChar('-', NULL)) != 0) {
      factor = -1;
      total += n;
    } else {
      total += readChar('+', NULL);
    }
    
    while ((n = readNumeric(digit)) != 0) {
      total += n;
      output += digit;
      output *= 10;
    }
    
    output = output * factor / 10;
    
    if((n = readChar('.', NULL)) != 0) {
      total += n;
      factor = 0.1;
      while((n = readNumeric(digit)) != 0) {
        total += n;
        output += factor * digit;
        factor *= 0.1;
      }
    }
    
    if((n = readChar(KF_E_ALLCASES, 2, NULL)) != 0) {
      total += n;
      
      long int power = 0;
      total += readNumber(power);
      
      if(power != 0) {
        output *= pow(10.0, (int)power);
      }
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readIdentifier(string& storage) {
    size_t total = 0;
    unsigned short int n = 0;
    kf_octet_t buffer[6];
    wchar_t ch = 0;
    
    if((n = readIdentifierBeginChar(ch, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += n;
    } else {
      return 0;
    }
    
    while((n = readIdentifierChar(ch, buffer)) != 0) {
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeSpace(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    
    while(!testSpace() && !_eof) {
      n = readAny(ch, buffer);
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeNewLine(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    
    while(!testNewLine() && !_eof) {
      n = readAny(ch, buffer);
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeSpaceOrNewLine(string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    
    while(!testNewLine() && !testSpace() && !_eof) {
      n = readAny(ch, buffer);
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeChar(const wchar_t t, string& storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    
    while(!testChar(t) && !_eof) {
      n = readAny(ch, buffer);
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeCharSkipEscaped(const wchar_t t, const wchar_t escape, string &storage)
  {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    bool escapeFlag = false;
    
    while((!testChar(t) || escapeFlag) && !_eof) {
      n = readAny(ch, buffer);
      
      if(escapeFlag) {
        escapeFlag = false;
      } else {
        escapeFlag = (ch == escape);
      }
      
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::readAllBeforeSequence(const wstring &str, string &storage) {
    size_t total = 0;
    kf_octet_t buffer[6];
    unsigned short int n = 0;
    wchar_t ch;
    
    while(!testSequence(str) && !_eof) {
      n = readAny(ch, buffer);
      storage.append((char*)buffer, n);
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::skipSpaces() {
    size_t total = 0;
    unsigned short int n = 0;
    
    while((n = readSpace()) != 0) {
      total += n;
    }
    
    return total;
  }
  
  size_t PredictiveParserBase::skipSpacesAndNewLines() {
    size_t total = 0;
    size_t s = 0;
    
    while((s = skipSpaces() + readNewLine()) != 0) {
      total += s;
    }
    
    return total;
  }
    
  const CodeLocation& PredictiveParserBase::getCodeLocation() const {
    return _location;
  }
  
} // namespace kfoundation
