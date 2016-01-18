//
//  StreamParser.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Std
#include <cmath>
#include <cstring>

// Internal
#include "UChar.h"
#include "UString.h"
#include "BufferOutputStream.h"
#include "StringPrintWriter.h"

// Self
#include "StreamParser.h"


namespace kfoundation {

  const UChar StreamParser::NEW_LINE_CHAR = '\n';
  const UChar StreamParser::RETURN_CHAR   = '\r';
  const UChar StreamParser::PLUS_CHAR     = '+';
  const UChar StreamParser::MINUS_CHAR    = '-';
  const UChar StreamParser::DOT_CHAR      = '.';
  const UChar StreamParser::LOWERCASE_E_CHAR = 'e';


  /**
   * Checks if the given argument is a valid begining of an identifier name.
   * Override to customize the parser based on your particular needs.
   * The default implementation returns `isAplhabet(ch) || ch == '_'`.
   *
   * @param ch The character to be tested.
   */
  
  bool StreamParser::isValidIdentifierBeginChar(const wchar_t& ch) const {
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
  
  bool StreamParser::isValidIdentifierChar(const wchar_t &ch) const {
    return isAlphanumeric(ch) || ch == '_';
  }
  
  
  /**
   * Checks if the given character is a white space.
   * The default implementation returns `ch == ' '`. Override to customize the
   * parser based on your particular needs.
   *
   * @param ch The character to be checked.
   */
  
  bool StreamParser::isSpace(const wchar_t &ch) const {
    return ch == L' ';
  }


  kf_int8_t StreamParser::read(UChar& ch) {
    if(_bufferPos < _bufferSize) {
      unsigned short int n = ch.read(_buffer + _bufferPos);
      
      if(_bufferPos + n > BUFFER_CAPACITY) {
        throw new KFException(K"Buffer overload");
      }
      
      _bufferPos += n;
      
      if(ch.equals(NEW_LINE_CHAR)) {
        _tmpLine++;
        _tmpCol = 1;
      } else if(!ch.equals(RETURN_CHAR)) {
        _tmpCol++;
      }
      _tmpByteIndex += n;
      _tmpCharIndex += 1;
      
      return n;
    }
    
    if(_stream.isEof()) {
      _eof = true;
      return 0;
    }
    
    unsigned short int n = ch.read(_streamRetainer);

    _bufferSize += ch.write(_buffer + _bufferPos);
    _bufferPos = _bufferSize;
    
    if(ch.equals(NEW_LINE_CHAR)) {
      _tmpLine++;
      _tmpCol = 1;
    } else if(ch.equals(RETURN_CHAR)) {
      _tmpCol++;
    }

    _tmpByteIndex += n;
    _tmpCharIndex += 1;
    
    return n;
  }
  
  
  void StreamParser::commit(const kf_int8_t n) {
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
  
  
  void StreamParser::rollback(const kf_int8_t n) {
    _tmpLine = _location.getLine();
    _tmpCol = _location.getCol();
    _tmpByteIndex = _location.getByteIndex();
    _tmpCharIndex = _location.getCharIndex();
    
    _bufferPos -= n;
  }
  
  
  kf_int8_t StreamParser::read(kf_octet_t* buffer) {
    UChar ch;
    return read(ch);
  }
  
  
  UChar StreamParser::readAndRollback() {
    UChar ch;
    rollback(read(ch));
    return ch;
  }
  
  
  /**
   * Constructor, creates a parser that reads symbols from the given stream.
   *
   * @param input The stream to parse.
   */
  
  StreamParser::StreamParser(Ref<InputStream> input)
    : _bufferSize(0),
      _bufferPos(0),
      _eof(input->isEof()),
      _tmpLine(1),
      _tmpCol(1),
      _tmpByteIndex(0),
      _tmpCharIndex(0),
      _stream(*input)
  {
    _streamRetainer = input;
    _location.setLine(1);
  }

  
  /**
   * Tests if the given character is next in the stream.
   *
   * @param ch The character to test.
   */
  
  bool StreamParser::testChar(const UChar ch) {
    return ch.equals(readAndRollback()) && !_eof;
  }
  
  
  /**
   * Thest if any of the given characters is next in the stream.
   *
   * @param chars The list of characters to test against.
   * @param n The number of characters in the given list.
   */
  
//  bool StreamParser::testChar(const wchar_t* chars, const int& n) {
//    wchar_t readChar = readAndRollback();
//    
//    if(_eof) {
//      return false;
//    }
//    
//    for(int i = 0; i < n; i++) {
//      if(readChar == chars[i]) {
//        return true;
//      }
//    }
//    
//    return false;
//  }

  
  /**
   * Thest if the next character is alphabet.
   */
  
  bool StreamParser::testAlphabet() {
    return isAlphabet(readAndRollback().toWChar()) && !_eof;
  }
  
  
  /**
   * Tests if the next character is alphanumeric.
   */
  
  bool StreamParser::testAlphanumeric() {
    return isAlphanumeric(readAndRollback().toWChar()) && !_eof;
  }
  
  
  /**
   * Test if the next character is space.
   * Override isSpace() to customize the behavior of this function.
   */
  
  bool StreamParser::testSpace() {
    return isSpace(readAndRollback().toWChar()) && !_eof;
  }
  
  
  /**
   * Test if the next character is a newline character '`\n`'.
   */
  
  bool StreamParser::testNewLine() {
    return readAndRollback().equals(NEW_LINE_CHAR) && !_eof;
  }
  
  
  /**
   * Checks if the end of stream is reached.
   */
  
  bool StreamParser::testEndOfStream() {
    return _stream.isEof();
  }
  
  
  /**
   * Checks if the given string is next in the stream.
   * @param str The sequence of characters to check against.
   */
  
  bool StreamParser::testSequence(RefConst<UString> str) {
    unsigned short int pos = 0;
    UChar ch;

    for(UCharIterator it = str->getUCharIterator(); it.hasMore(); it.next()) {
      pos += read(ch);
      if(!ch.equals(it) || _eof) {
        rollback(pos);
        return false;
      }
    }

    rollback(pos);
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
  
  kf_int8_t StreamParser::readChar(const UChar ch) {
    UChar readCh;
    unsigned short int s = read(readCh);
    
    if(ch.equals(readCh)) {
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
  
//  unsigned short int StreamParser::readChar(const wchar_t* chars, const int& n, kf_octet_t* octets) {
//    wchar_t ch = 0;
//    unsigned short int s = read(ch, octets);
//    
//    for(int i = 0; i < n; i++) {
//      if(chars[i] == ch) {
//        commit(s);
//        return s;
//      }
//    }
//    
//    rollback(s);
//    return 0;
//  }

  
  /**
   * Checks if the next character in the stream is an alphabet, and reads it
   * if so.
   *
   * @param ch Output, will be assigned with the read character.
   * @param octets If not `NULL`, the read octets are written on the buffer
   *        pointed by this argument. Default value is `NULL`.
   * @return The number of read octets.
   */
  
  kf_int8_t StreamParser::readAlphabet(UChar& ch) {
    kf_int8_t s = read(ch);
    
    if(isAlphabet(ch.toWChar())) {
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
  
  kf_int8_t StreamParser::readNumeric(UChar& ch) {
    kf_int8_t s = read(ch);
    
    if(isNumeric(ch.toWChar())) {
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

  kf_int8_t StreamParser::readNumeric(kf_int8_t& digit) {
    UChar ch;
    kf_int8_t s = read(ch);
    wchar_t wch = ch.toWChar();
    if(!isNumeric(wch)) {
      return 0;
    }
    digit = getNumericalValueOf(wch);
    return s;
  }


  kf_int8_t StreamParser::readHexNumeric(kf_int8_t& n) {
    UChar ch;
    read(ch);

    wchar_t wch = ch.toWChar();
    if(wch >= '0' && wch <= '9') {
      n = wch - '0';
      return 1;
    } else if(wch >= 'a' && wch <= 'f') {
      n = wch - 'a' + 10;
      return 1;
    } else if(wch >= 'A' && wch <= 'F') {
      n = wch - 'A' + 10;
      return 1;
    }
    return 0;
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
  
  kf_int8_t StreamParser::readAlphanumeric(UChar& ch) {
    kf_int8_t s = read(ch);
    
    if(isAlphanumeric(ch.toWChar())) {
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
  
  kf_int8_t StreamParser::readIdentifierBeginChar(UChar& ch)
  {
    kf_int8_t s = read(ch);
    
    if(isValidIdentifierBeginChar(ch.toWChar())) {
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
  
  kf_int8_t StreamParser::readIdentifierChar(UChar& ch) {
    unsigned short int s = read(ch);
    
    if(isValidIdentifierChar(ch.toWChar())) {
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
  
  kf_int8_t StreamParser::readSpace() {
    UChar ch;
    kf_int8_t s = read(ch);

    if(isSpace(ch.toWChar())) {
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
  
  kf_int8_t StreamParser::readNewLine() {
    UChar ch;
    kf_int8_t s = read(ch);
    
    if(ch.equals(NEW_LINE_CHAR)) {
      commit(s);
      
      kf_int8_t s2 = read(ch);
      if(ch.equals(RETURN_CHAR)) {
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
  
  kf_int8_t StreamParser::readAny(UChar& ch) {
    kf_int8_t n = read(ch);
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
  
  kf_int8_t StreamParser::readSequence(RefConst<UString> str) {
    UChar ch;
    kf_int64_t pos = 0;

    for(UCharIterator it = str->getUCharIterator(); it.hasMore(); it.next()) {
      pos += read(ch);
      if(!ch.equals(it)) {
        rollback(pos);
        return false;
      }
    }

    commit(pos);
    return pos;
  }
  
  
  /**
   * Reads all the next characters that are alphabet and appends them to the
   * given argument.
   *
   * @param storage Output, read octets will be appended to it.
   * @return The number of octets read.
   */
  
  kf_int64_t StreamParser::readAllAlphabet(Ref<OutputStream> storage) {
    UChar ch;
    kf_int64_t total = 0;

    while(could(readAlphabet(ch))) {
      total += ch.getOctetCount();
      ch.printToStream(storage);
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
  
  kf_int64_t StreamParser::readAllAlphanumeric(Ref<OutputStream> storage) {
    UChar ch;
    kf_int64_t total = 0;

    while(could(readAlphanumeric(ch))) {
      total += ch.getOctetCount();
      ch.printToStream(storage);
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
  
  kf_int64_t StreamParser::readAllNumeric(Ref<OutputStream> storage) {
    UChar ch;
    kf_int64_t total = 0;

    while(could(readNumeric(ch), total)) {
      ch.printToStream(storage);
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
  
  kf_int64_t StreamParser::readNumber(Ref<OutputStream> storage) {
    UChar ch;
    kf_int64_t total = 0;

    if(could(readChar(PLUS_CHAR), total)) {
      PLUS_CHAR.printToStream(storage);
    } else if(could(readChar(MINUS_CHAR), total)) {
      MINUS_CHAR.printToStream(storage);
    }

    total += readAllNumeric(storage);
    
    if(could(readChar(DOT_CHAR), total)) {
      DOT_CHAR.printToStream(storage);
    }
    
    total += readAllNumeric(storage);

    return total;
  }
  
  
  /**
   * Checks if the next character(s) represents an integer number and if so, 
   * reads and converts it to it equivalant integer value.
   *
   * @param output The integer number read.
   * @return The number of octets read.
   */
  
  kf_int64_t StreamParser::readNumber(kf_int64_t& output) {
    kf_int64_t total = 0;
    kf_int8_t digit = 0;
    
    output = 0;
    long int factor = 1;
    
    if(could(readChar(MINUS_CHAR), total)) {
      factor = -1;
    } else {
      total += readChar(PLUS_CHAR);
    }
    
    while(could(readNumeric(digit), total)) {
      output += digit;
      output *= 10;
    }
    
    output = output * factor / 10;
    
    return total;
  }


  kf_int64_t StreamParser::readNumber(kf_int32_t& output) {
    kf_int64_t n;
    size_t s = readNumber(n);
    output = (kf_int32_t)n;
    return s;
  }


  kf_int64_t StreamParser::readHexNumber(kf_int32_t &output) {
    kf_int64_t n;
    size_t s = readHexNumber(n);
    output = (kf_int32_t)n;
    return s;
  }


  kf_int64_t StreamParser::readHexNumber(kf_int64_t& output) {
    kf_int64_t total = 0;
    kf_int8_t digit = 0;

    output = 0;

    while(could(readHexNumeric(digit), total)) {
      output += digit;
      output <<= 4;
    }

    output >>= 4;

    return total;
  }
  
  
  /**
   * Checks if the next character(s) represents a real number and if so,
   * reads and converts it to it equivalant integer value.
   *
   * @param output The integer number read.
   * @return The number of octets read.
   */
  
  kf_int64_t StreamParser::readNumber(double& output) {
    kf_int64_t total = 0;
    kf_int8_t digit = 0;
    
    output = 0;
    double factor = 1;
    
    if(could(readChar(MINUS_CHAR), total)) {
      factor = -1;
    } else {
      total += readChar(PLUS_CHAR);
    }
    
    while (could(readNumeric(digit), total)) {
      output += digit;
      output *= 10;
    }
    
    output = output * factor / 10;
    
    if(could(readChar(DOT_CHAR), total)) {
      factor = 0.1;
      while(could(readNumeric(digit), total)) {
        output += factor * digit;
        factor *= 0.1;
      }
    }
    
    if(could(readChar(LOWERCASE_E_CHAR), total)) {
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
  
  kf_int64_t StreamParser::readIdentifier(Ref<OutputStream> storage) {
    kf_int64_t total = 0;
    UChar ch;

    if(could(readIdentifierBeginChar(ch), total)) {
      ch.printToStream(storage);
    } else {
      return 0;
    }
    
    while(could(readIdentifierChar(ch), total)) {
      ch.printToStream(storage);
    }

    return total;
  }
  
  
  /**
   * Reads all character before a space (or end of stream) is encountered.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */
  
  kf_int64_t StreamParser::readAllBeforeSpace(Ref<OutputStream> storage) {
    UChar ch;
    kf_int64_t total = 0;

    while(!testSpace() && !_eof) {
      total += readAny(ch);
      ch.printToStream(storage);
    }
    
    return total;
  }
  
  
  /**
   * Reads all character before a new line (or end of stream) is encountered.
   *
   * @param storage Output, the octets read will be appended to it.
   * @return The number of read octets.
   */
  
  kf_int64_t StreamParser::readAllBeforeNewLine(Ref<OutputStream> storage) {
    kf_int64_t total = 0;
    UChar ch;

    while(!testNewLine() && !_eof) {
      total += readAny(ch);
      ch.printToStream(storage);
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

  kf_int64_t StreamParser::readAllBeforeSpaceOrNewLine(Ref<OutputStream> storage) {
    kf_int64_t total = 0;
    UChar ch;

    while(!testNewLine() && !testSpace() && !_eof) {
      total += readAny(ch);
      ch.printToStream(storage);
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
  
  kf_int64_t
  StreamParser::readAllBeforeChar(const UChar ch, Ref<OutputStream> storage) {
    UChar readCh;
    kf_int64_t total = 0;

    while(!testChar(ch) && !_eof) {
      total += readAny(readCh);
      readCh.printToStream(storage);
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
  
  kf_int64_t
  StreamParser::readAllBeforeCharSkipEscaped(const UChar ch,
      const UChar escape, Ref<OutputStream> storage)
  {
    UChar readCh;
    kf_int64_t total = 0;
    bool escapeFlag = false;
    
    while((!testChar(ch) || escapeFlag) && !_eof) {
      total += readAny(readCh);
      
      if(escapeFlag) {
        escapeFlag = false;
      } else {
        escapeFlag = (readCh.equals(escape));
      }

      readCh.printToStream(storage);
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
  
  kf_int64_t StreamParser::readAllBeforeSequence(RefConst<UString> str,
      Ref<OutputStream> storage)
  {
    kf_int64_t total = 0;
    UChar ch;

    while(!testSequence(str) && !_eof) {
      total += readAny(ch);
      ch.printToStream(storage);
    }
    
    return total;
  }
  
  
  /**
   * Consumes all the spaces next in the stream.
   *
   * @return The number of read octets.
   */
  
  kf_int64_t StreamParser::skipSpaces() {
    kf_int64_t total = 0;

    while(could(readSpace(), total)) {
      // Nothing;
    }
    
    return total;
  }
  
  
  /**
   * Consumes all the spaces and new line characters next in the stream.
   *
   * @return The number of read octets.
   */
  
  kf_int64_t StreamParser::skipSpacesAndNewLines() {
    kf_int64_t total = 0;

    while(could(readSpace(), total) || could(readNewLine(), total)) {
      // Nothing;
    }
    
    return total;
  }
  
  
  /**
   * Returns the CodeLocation object corresponding to the current position
   * in the stream.
   */
  
  const CodeLocation& StreamParser::getCodeLocation() const {
    return _location;
  }
  
} // namespace kfoundation