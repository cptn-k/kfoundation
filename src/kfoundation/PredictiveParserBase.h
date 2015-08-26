//
//  PredictiveParserBase.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_PREDICTIVEPARSERBASE
#define KFOUNDATION_PREDICTIVEPARSERBASE

#include "ManagedObject.h"
#include "InputStream.h"
#include "CodeLocation.h"
#include "Ptr.h"

namespace kfoundation {
  
  
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
   * @headerfile PredictiveParserBase.h <kfoundation/PredictiveParserBase.h>
   */
  
  class PredictiveParserBase : public ManagedObject {
  private:
    static const int BUFFER_CAPACITY = 16;
    
    Ptr<InputStream> _stream;
    CodeLocation _location;
    bool _eof;
    kf_octet_t _buffer[BUFFER_CAPACITY];
    unsigned short int _bufferSize;
    unsigned short int _bufferPos;
    
    long int _tmpLine;
    long int _tmpCol;
    long int _tmpByteIndex;
    long int _tmpCharIndex;
    
    inline static unsigned short int getNumericalValueOf(const wchar_t& ch);
    unsigned short int read(wchar_t& ch, kf_octet_t* readOctets);
    unsigned short int read(kf_octet_t* readOctets);
    void commit(const unsigned short int& n);
    void rollback(const unsigned short int& n);
    wchar_t readAndRollback();
    
  protected:
    inline static bool isAlphabet(const wchar_t& ch);
    inline static bool isAlphanumeric(const wchar_t& ch);
    inline static bool isNumeric(const wchar_t& ch);
    virtual bool isValidIdentifierBeginChar(const wchar_t& ch) const;
    virtual bool isValidIdentifierChar(const wchar_t& ch) const;
    virtual bool isSpace(const wchar_t& ch) const;
    
  public:
    PredictiveParserBase(PPtr<InputStream> input);
    ~PredictiveParserBase();
    
    bool testChar(const wchar_t& t);
    bool testChar(const wchar_t* chars, const int& n);
    bool testAlphabet();
    bool testAlphanumeric();
    bool testSpace();
    bool testNewLine();
    bool testEndOfStream();
    bool testSequence(const wstring& str);
    
    unsigned short int readChar(const wchar_t& t, kf_octet_t* octets = NULL);
    unsigned short int readChar(const wchar_t* chars, const int& n, kf_octet_t* octets = NULL);
    unsigned short int readAlphabet(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readNumeric(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readNumeric(unsigned short int& digit);
    unsigned short int readAlphanumeric(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readIdentifierBeginChar(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readIdentifierChar(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readSpace();
    unsigned short int readNewLine();
    unsigned short int readAny(wchar_t& ch, kf_octet_t* octets = NULL);
    unsigned short int readSequence(const wstring& str);
    
    size_t readAllAlphabet(string& storage);
    size_t readAllAlphanumeric(string& storage);
    size_t readAllNumeric(string& storage);
    size_t readNumber(string& storage);
    size_t readNumber(long int& output);
    size_t readNumber(double& output);
    size_t readIdentifier(string& storage);
    size_t readAllBeforeSpace(string& storage);
    size_t readAllBeforeNewLine(string& storage);
    size_t readAllBeforeSpaceOrNewLine(string& storage);
    size_t readAllBeforeChar(const wchar_t ch, string& storage);
    size_t readAllBeforeCharSkipEscaped(const wchar_t ch, const wchar_t escape, string& storage);
    size_t readAllBeforeSequence(const wstring& str, string& storage);
    size_t skipSpaces();
    size_t skipSpacesAndNewLines();
    
    const CodeLocation& getCodeLocation() const;
  };
  
  inline bool PredictiveParserBase::isAlphabet(const wchar_t& ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
  }
  
  inline bool PredictiveParserBase::isNumeric(const wchar_t& ch) {
    return '0' <= ch && ch <= '9';
  }
  
  inline bool PredictiveParserBase::isAlphanumeric(const wchar_t& ch) {
    return isAlphabet(ch) || isNumeric(ch);
  }
  
  inline unsigned short int PredictiveParserBase::getNumericalValueOf(const wchar_t& ch) {
    return ch - '0';
  }
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_PREDICTIVEPARSERBASE) */
