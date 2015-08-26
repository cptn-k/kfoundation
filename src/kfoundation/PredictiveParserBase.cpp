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
  
  
  /**
   * Checks if the given argument is a valid begining of an identifier name.
   * Override to customize the parser based on your particular needs.
   * The default implementation returns `isAplhabet(ch) || ch == '_'`.
   *
   * @param ch The character to be tested.
   */
  
  bool PredictiveParserBase::isValidIdentifierBeginChar(const wchar_t& ch) const {
    return isAlphabet(ch) || ch == '_';
  }
  
  
  /**
   * Checks if given argument is can be used after the first character of an
   * identifier name.
   * Override to customize the parser based on your particular needs.
   * The default implementation returns `isAlphanumeric(ch) || ch == '_'`.
   *
   * @param ch The character to be checked.
   */
  
  bool PredictiveParserBase::isValidIdentifierChar(const wchar_t &ch) const {
    return isAlphanumeric(ch) || ch == '_';
  }
  
  
  /**
   * Checks if the given character is a white space.
   * The default implementation returns `ch == ' '`. Override to customize the
   * parser based on your particular needs.
   *
   * @param ch The character to be checked.
   */
  
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
  
  
  /**
   * Constructor, creates a parser that reads symbols from the given stream.
   *
   * @param input The stream to parse.
   */
  
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
  
  
  /**
   * Deconstructor.
   */
  
  PredictiveParserBase::~PredictiveParserBase() {
    // Nothing;
  }
  
  
  /**
   * Tests if the given character is next in the stream.
   *
   * @param ch The character to test.
   */
  
  bool PredictiveParserBase::testChar(const wchar_t& ch) {
    return ch == readAndRollback() && !_eof;
  }
  
  
  /**
   * Thest if any of the given characters is next in the stream.
   *
   * @param chars The list of characters to test against.
   * @param n The number of characters in the given list.
   */
  
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
  
  
  /**
   * Thest if the next character is alphabet.
   */
  
  bool PredictiveParserBase::testAlphabet() {
    return isAlphabet(readAndRollback()) && !_eof;
  }
  
  
  /**
   * Tests if the next character is alphanumeric.
   */
  
  bool PredictiveParserBase::testAlphanumeric() {
    return isAlphanumeric(readAndRollback()) && !_eof;
  }
  
  
  /**
   * Test if the next character is space.
   * Override isSpace() to customize the behavior of this function.
   */
  
  bool PredictiveParserBase::testSpace() {
    return isSpace(readAndRollback()) && !_eof;
  }
  
  
  /**
   * Test if the next character is a newline character '`\n`'.
   */
  
  bool PredictiveParserBase::testNewLine() {
    return readAndRollback() == '\n' && !_eof;
  }
  
  
  /**
   * Checks if the end of stream is reached.
   */
  
  bool PredictiveParserBase::testEndOfStream() {
    return _stream->isEof();
  }
  
  
  /**
   * Checks if the given string is next in the stream.
   * @param str The sequence of characters to check against.
   */
  
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
  
  
  /**
   * Checks if the given character is next in the stream, and reads it if so.
   * 
   * @param t The character to be expected next in the stream.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
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
  
  
  /**
   * Checks if any of the given characters is next in the stream, and reads it 
   * if so.
   *
   * @param chars The list of character to check against.
   * @param n The number of characters in the given list.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
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
  
  
  /**
   * Checks if the next character in the stream is an alphabet, and reads it
   * if so.
   *
   * @param ch Output, will be assigned with the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  unsigned short int PredictiveParserBase::readAlphabet(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isAlphabet(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }

  
  /**
   * Checks if the next character in the stream is a digit, and reads it if so.
   *
   * @param ch Output, will be assigned with the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  unsigned short int PredictiveParserBase::readNumeric(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isNumeric(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }

  
  /**
   * Checks if the next character in the stream is a digit, and reads it and 
   * converts it to its equivalant numeric value.
   *
   * @param digit Output, will be assigned to the numeric equivalant of the
   *              read character.
   * @return The number of read octets.
   */

  unsigned short int
  PredictiveParserBase::readNumeric(unsigned short int& digit) {
    wchar_t ch = 0;
    unsigned short int n = readNumeric(ch, NULL);
    if(n == 0) {
      return 0;
    }
    
    digit = getNumericalValueOf(ch);
    return n;
  }
  
  
  /**
   * Checks if the next character in the stream is a letter or a digit, and 
   * reads it and converts it to its equivalant numeric value.
   *
   * @param ch Output, will be assigned to the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  unsigned short int
  PredictiveParserBase::readAlphanumeric(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isAlphanumeric(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  
  /**
   * Checks if the next character in the stream is a valid identifier begining
   * character, and if so, reads it.
   * Override isValidIdentifierBeginChar() to customize the behavior of this 
   * method.
   *
   * @param ch Output, will be assigned to the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  unsigned short int 
  PredictiveParserBase::readIdentifierBeginChar(wchar_t& ch, kf_octet_t* octets)
  {
    unsigned short int s = read(ch, octets);
    
    if(isValidIdentifierBeginChar(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  
  /**
   * Checks if the next character in the stream is a valid identifier
   * character, and if so, reads it.
   * Override isValidIdentifierBeginChar() to customize the behavior of this
   * method.
   *
   * @param ch Output, will be assigned to the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  unsigned short int
  PredictiveParserBase::readIdentifierChar(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int s = read(ch, octets);
    
    if(isValidIdentifierChar(ch)) {
      commit(s);
      return s;
    }
    
    rollback(s);
    return 0;
  }
  
  
  /**
   * Checks if the next character in the stream is a space, and reads it if so.
   * Override isSpace() to customzie the behavior of this function.
   *
   * @return The number of read octers.
   */
  
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
  
  
  /**
   * Checks if the next character in the stream is newline '\n' and reads it
   * if so.
   *
   * @return The number of read character.
   */
  
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
  
  
  /**
   * Reads any character next in the stream unless the end of stream is reached.
   *
   * @param ch Output, will be assigned to the character read.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of octets read.
   */
  
  unsigned short int PredictiveParserBase::readAny(wchar_t& ch, kf_octet_t* octets) {
    unsigned short int n = read(ch, octets);
    commit(n);
    return n;
  }
  
  
  /**
   * Checks if the next sequence of characters match the given string, and
   * reads them if so.
   *
   * @param str The sequence of characters to check agains.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Reads all the next characters that are alphabet and appends them to the
   * given argument.
   *
   * @param storage Output, read octets will be appended to it.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Reads all the next characters that are alphanumeric and appends them
   * to the given argument.
   *
   * @param storage Output, read octets will be appended to it.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Reads all the next numeric characters and appends them to the given 
   * parameter.
   *
   * @param storage Output, read octets will be appended to it.
   * @return The number of read octets.
   */
  
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
  
  
  
  /**
   * Checks if the next character(s) represent a number (+|-)?[0..9]+(.[0..9]*)?,
   * and reads them if so. Read characters are appended to the given parameter.
   *
   * @param storage Output, the read octets will be appended to it.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Checks if the next character(s) represents an integer number and if so, 
   * reads and converts it to it equivalant integer value.
   *
   * @param output The integer number read.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Checks if the next character(s) represents a real number and if so,
   * reads and converts it to it equivalant integer value.
   *
   * @param output The integer number read.
   * @return The number of octets read.
   */
  
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
  
  
  /**
   * Reads all the next characters in the stream that form an identifier, and
   * appends them to the given argument. 
   * Override isValidIdentifierBeginChar() and isValidIdentifierChar() to 
   * customize the behavior of this method.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */
  
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
  
  
  /**
   * Reads all character before a space (or end of stream) is encountered.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */
  
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
  
  
  /**
   * Reads all character before a new line (or end of stream) is encountered.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */
  
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
  

  /**
   * Reads all character before space or new line (or end of stream) is 
   * encountered.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */

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
  
  
  /**
   * Read all characters before the given character (or end of stream) is 
   * encountered.
   *
   * @param t The character to read until.
   * @param storage Output, the read characters will be appended to it.
   * @return The number of read octets.
   */
  
  size_t
  PredictiveParserBase::readAllBeforeChar(const wchar_t t, string& storage) {
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
  
  
  /**
   * Reads all characters before the given character, but skips all the matching
   * ones immidiately after the given excape character.
   *
   * @param t The character to read until.
   * @param escape The escape character.
   * @param storage Output, the read characters will be appended to it.
   * @return The number of read octets.
   */
  
  size_t
  PredictiveParserBase::readAllBeforeCharSkipEscaped(const wchar_t t,
      const wchar_t escape, string &storage)
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
  
  
  /**
   * Reads all character before the given sequence.
   *
   * @param str The sequence of characters to read until.
   * @param storage Output, the read characters will be appended to it.
   * @return The number of read octets.
   */
  
  size_t
  PredictiveParserBase::readAllBeforeSequence(const wstring &str,
      string &storage)
  {
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
  
  
  /**
   * Consumes all the spaces next in the stream.
   *
   * @return The number of read octets.
   */
  
  size_t PredictiveParserBase::skipSpaces() {
    size_t total = 0;
    unsigned short int n = 0;
    
    while((n = readSpace()) != 0) {
      total += n;
    }
    
    return total;
  }
  
  
  /**
   * Consumes all the spaces and new line characters next in the stream.
   *
   * @return The number of read octets.
   */
  
  size_t PredictiveParserBase::skipSpacesAndNewLines() {
    size_t total = 0;
    size_t s = 0;
    
    while((s = skipSpaces() + readNewLine()) != 0) {
      total += s;
    }
    
    return total;
  }
  
  
  /**
   * Returns the CodeLocation object corresponding to the current position
   * in the stream.
   */
  
  const CodeLocation& PredictiveParserBase::getCodeLocation() const {
    return _location;
  }
  
} // namespace kfoundation
