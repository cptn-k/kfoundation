//
//  StreamParser.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_PREDICTIVEPARSERBASE
#define KFOUNDATION_PREDICTIVEPARSERBASE

#include "InputStream.h"
#include "CodeLocation.h"
#include "Ref.h"
#include "UChar.h"

// Super
#include "KFObject.h"


namespace kfoundation {

  class UString;
  
  /**
   * Packs ample of basic functionalities to implement any predictive parser.
   * It can be directly instantiated, or be extended to implement a parser for
   * the desired grammar. Protected methods 
   * `isValidIdentifierBeginChar()`,
   * `isValidIdentifierChar()`, and
   * `isSpace()` can be overriden if needed.
   *
   * There are two set of methods for reading the input. The readXXX methods
   * consume the stream characters if successful, and return the number read
   * character. If faild the stream will remain at its current position and
   * return value will be zero. The testXXX methods will test if the given
   * argument is there next in the stream without consuming anything.
   *
   * @ingroup io
   * @ingroup utils
   * @headerfile StreamParser.h <kfoundation/StreamParser.h>
   */
  
  class StreamParser : public KFObject {

  // --- STATIC FIELDS --- //

    private: static const int BUFFER_CAPACITY = 16;
    private: static const UChar NEW_LINE_CHAR;
    private: static const UChar RETURN_CHAR;
    private: static const UChar PLUS_CHAR;
    private: static const UChar MINUS_CHAR;
    private: static const UChar DOT_CHAR;
    private: static const UChar LOWERCASE_E_CHAR;


  // --- FIELDS --- //
    
    private: bool       _eof;
    private: kf_int8_t  _bufferSize;
    private: kf_int8_t  _bufferPos;
    private: kf_int64_t _tmpLine;
    private: kf_int64_t _tmpCol;
    private: kf_int64_t _tmpByteIndex;
    private: kf_int64_t _tmpCharIndex;
    private: kf_octet_t _buffer[BUFFER_CAPACITY];
    private: InputStream& _stream;
    private: CodeLocation _location;
    private: Ref<InputStream> _streamRetainer;


  // --- CONSTRUCTORS --- //
    
    public: StreamParser(Ref<InputStream> input);


  // --- METHODS --- //

    private: inline static kf_int8_t getNumericalValueOf(const wchar_t& ch);
    private: kf_int8_t read(UChar& ch);
    private: kf_int8_t read(kf_octet_t* readOctets);
    private: void commit(const kf_int8_t n);
    private: void rollback(const kf_int8_t n);
    private: UChar readAndRollback();

    protected: inline static bool isAlphabet(const wchar_t& ch);
    protected: inline static bool isAlphanumeric(const wchar_t& ch);
    protected: inline static bool isNumeric(const wchar_t& ch);
    protected: virtual bool isValidIdentifierBeginChar(const wchar_t& ch) const;
    protected: virtual bool isValidIdentifierChar(const wchar_t& ch) const;
    protected: virtual bool isSpace(const wchar_t& ch) const;
    protected: inline static bool could(kf_int8_t n);
    protected: inline static bool could(kf_int8_t n, kf_int64_t& total);
    protected: inline static bool could(kf_int64_t n);
    protected: inline static bool couldNot(kf_int8_t n);
    protected: inline static bool couldNot(kf_int64_t n);
    
    public: bool testChar(const UChar ch);
    //public: bool testChar(const wchar_t* chars, const int n);
    public: bool testAlphabet();
    public: bool testAlphanumeric();
    public: bool testSpace();
    public: bool testNewLine();
    public: bool testEndOfStream();
    public: bool testSequence(RefConst<UString> str);
    
    public: kf_int8_t readChar(UChar ch);
    //public: kf_int8_t readChar(const wchar_t* chars, const int& n, kf_octet_t* octets = NULL);
    public: kf_int8_t readAlphabet(UChar& ch);
    public: kf_int8_t readNumeric(UChar& ch);
    public: kf_int8_t readNumeric(kf_int8_t& digit);
    public: kf_int8_t readHexNumeric(kf_int8_t& n);
    public: kf_int8_t readAlphanumeric(UChar& ch);
    public: kf_int8_t readIdentifierBeginChar(UChar& ch);
    public: kf_int8_t readIdentifierChar(UChar& ch);
    public: kf_int8_t readSpace();
    public: kf_int8_t readNewLine();
    public: kf_int8_t readAny(UChar& ch);
    public: kf_int8_t readSequence(RefConst<UString> str);
    
    public: kf_int64_t readAllAlphabet(Ref<OutputStream> storage);
    public: kf_int64_t readAllAlphanumeric(Ref<OutputStream> storage);
    public: kf_int64_t readAllNumeric(Ref<OutputStream> storage);
    public: kf_int64_t readNumber(Ref<OutputStream> storage);
    public: kf_int64_t readNumber(kf_int64_t& output);
    public: kf_int64_t readNumber(kf_int32_t& output);
    public: kf_int64_t readHexNumber(kf_int32_t& output);
    public: kf_int64_t readHexNumber(kf_int64_t& output);
    public: kf_int64_t readNumber(double& output);
    public: kf_int64_t readIdentifier(Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeSpace(Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeNewLine(Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeSpaceOrNewLine(Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeChar(const UChar ch, Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeCharSkipEscaped(const UChar ch, const UChar escape, Ref<OutputStream> storage);
    public: kf_int64_t readAllBeforeSequence(RefConst<UString> sequence, Ref<OutputStream> storage);
    public: kf_int64_t skipSpaces();
    public: kf_int64_t skipSpacesAndNewLines();

    public: const CodeLocation& getCodeLocation() const;
  };
  
  inline bool StreamParser::isAlphabet(const wchar_t& ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
  }
  
  inline bool StreamParser::isNumeric(const wchar_t& ch) {
    return '0' <= ch && ch <= '9';
  }
  
  inline bool StreamParser::isAlphanumeric(const wchar_t& ch) {
    return isAlphabet(ch) || isNumeric(ch);
  }
  
  inline kf_int8_t StreamParser::getNumericalValueOf(const wchar_t& ch) {
    return ch - '0';
  }

  inline bool StreamParser::could(kf_int8_t n) {
    return n > 0;
  }

  inline bool StreamParser::could(kf_int8_t n, kf_int64_t& total) {
    if(n > 0) {
      total += n;
      return true;
    }
    return false;
  }

  inline bool StreamParser::could(kf_int64_t n) {
    return n > 0;
  }

  inline bool StreamParser::couldNot(kf_int8_t n) {
    return n <= 0;
  }

  inline bool StreamParser::couldNot(kf_int64_t n) {
    return n <= 0;
  }
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_PREDICTIVEPARSERBASE) */