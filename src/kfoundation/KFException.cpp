/*---[KFException.cpp]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::KFException::*
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

#include "KFException.h"

#include <ostream>
#include <exception>
#include <typeinfo>
#include <vector>
#include <string>

#include <execinfo.h>
#include <unistd.h>
#include <cxxabi.h>
#include <cstdlib>
#include <iostream>

#include "Ptr.h"
#include "Int.h"
#include "LongInt.h"
#include "ObjectSerializer.h"
#include "Thread.h"

namespace kfoundation {

using namespace std;

//\/ KSystemException::StackTraceItem /\///////////////////////////////////////

  KFException::StackTraceItem
  KFException::StackTraceItem::fromStackTraceString(char* str)
  {
    char* begin;
    char* end;
    
    begin = readWhiteSpace(str);
    end = readText(begin);
    int index = Int::parse(string(begin, end));
    
    begin = readWhiteSpace(end);
    end = readText(begin);
    string exeName(begin, end);
    
    begin = readWhiteSpace(end);
    end = readText(begin);
    long int address = LongInt::parse(string(begin, end), LongInt::HEXADECIMAL);
    
    begin = readWhiteSpace(end);
    end = readText(begin);
    string symbol = demangle(string(begin, end));
    
    begin = readWhiteSpace(end);
    begin = readChar(begin, '+');
    begin = readWhiteSpace(begin);
    end = readText(begin);
    int offset = Int::parse(string(begin, end));
    
    StackTraceItem item;

    item._index = index;
    item._exeName = exeName;
    item._address = address;
    item._symbol = symbol;
    item._offset = offset;
    
    return item;
  }
  
  
  char* KFException::StackTraceItem::readWhiteSpace(char* str) {
    while(*str == ' ')
      str++;
    return str;
  }
  
  
  char* KFException::StackTraceItem::readText(char* str) {
    while(*str != ' ' && *str != 0)
      str++;
    return str;
  }
  
  
  char* KFException::StackTraceItem::readChar(char* str, char ch) {
    if(*str == ch)
      str++;
    return str;
  }
  
  
  string KFException::StackTraceItem::demangle(string str) {
    size_t len = 200;
    char* output = (char*)malloc(len);
    int status;
    output = abi::__cxa_demangle(str.c_str(), output, &len, &status);
    
    if(output != NULL) {
      string outstr(output);
      free(output);
      return outstr;
    }
    
    return str;
  }
  
  
  void KFException::StackTraceItem::serialize(PPtr<ObjectSerializer> os) const
  {
    os->object("StackTraceItem")
      ->attribute("function", _symbol)
      ->attribute("exeName", _exeName)
      ->attribute("address", LongInt::toHexString(_address))
      ->attribute("offset", _offset)
      ->endObject();
  }


//\/ KFException /\////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Primary constructor. Once invoked, creates and stores the current stack 
   * trace.
   *
   * @param message The error message describing the exception.
   */
  
  KFException::KFException(string message)
  : _name("KFException"), _message(message)
  {
    _stackTrace = NULL;
    createStackTrace();
    _nHiddenFrames = 3;
  }
  
  
  /**
   * Copy constructor.
   * @param other The object to be copied.
   */
  
  KFException::KFException(const KFException& other) {
    _name = other._name;
    _message = other._message;
    _nHiddenFrames = other._nHiddenFrames;
    _nFrames = other._nFrames;
    _stackTrace = new StackTraceItem[_nFrames];
    for(int i = 0; i < _nFrames; i++) {
      _stackTrace[i] = other._stackTrace[i];
    }
  }
  
  
  /** Deconstructor */
  
  KFException::~KFException() throw() {
    if(NOT_NULL(_stackTrace)) {
      delete[] _stackTrace;
      _stackTrace = NULL;
      _nFrames = 0;
    }
  }
  
  
// --- METHODS --- //

#ifdef KF_UNIX
  void KFException::createStackTrace() {
    void* addrList[MAX_FRAMES + 1];
    _nFrames = backtrace(addrList, sizeof(addrList) / sizeof(void*));
    if(_nFrames == 0) {
      return;
    }
    char** symbolList = backtrace_symbols(addrList, _nFrames);
    
    _stackTrace = new StackTraceItem[_nFrames];
    
    for(int i = 0; i < _nFrames; i++) {
      _stackTrace[i] = StackTraceItem::fromStackTraceString(symbolList[i]);
    }
    
    free(symbolList);
  }
#else
#error "This platform is not supported."
#endif

  /**
   * This method should be called in the constructor of every subclass.
   * It sets the name of the exception which would normally be the same as the
   * name of the class. At the same time, it keeps track of the number of
   * exception class constructors of superclasses called within each other, in
   * order to remove them from the printed stack trace.
   *
   * @param name The name of the exception class.
   */
  
  void KFException::setName(string name) {
    _name = name;
    _nHiddenFrames++;
  }
  
  
  void KFException::incrementNumberOfHiddenFrames() {
    _nHiddenFrames++;
  }

  
  /**
   * @return The message assigned to this exception.
   */
  const string& KFException::getMessage() const {
    return _message;
  }

  
  // Inherited from SerializingStreamer //
  
  /**
   * Serializing method.
   *
   * @param builder The ObjectSerializer used to build the output.
   */
  
  void KFException::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object(_name)
      ->attribute("message", getMessage())
      ->attribute("thread", Thread::getNameOfCurrentThread())
      ->member("stackTrace")->collection();
    
    for(int i = _nHiddenFrames; i < _nFrames; i++) {
      _stackTrace[i].serialize(builder);
    }
    
    builder->endCollection();
    builder->endObject();
  }
  
  
  // Inherited from exception //

  /**
   * Overriding the standard C++ behaviour, returns the message and stack trace
   * in C-string format.
   */
  
  const char* KFException::what() const throw() {
    const char* str = toString().c_str();
    return str;
  }
  
} // namespace kfoundation
