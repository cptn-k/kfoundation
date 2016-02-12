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

// Std
#include <ostream>
#include <exception>
#include <typeinfo>
#include <vector>
#include <string>
#include <cstdlib>

// Ext
#include <execinfo.h>
#include <cxxabi.h>

// POSIX
#include <unistd.h>

// Internal
#include "Ref.h"
#include "UString.h"
#include "ObjectSerializer.h"
#include "Thread.h"
#include "BufferInputStream.h"
#include "StreamParser.h"
#include "UString.h"
#include "Array.h"
#include "InputStream.h"
#include "StringPrintWriter.h"

// Self
#include "KFException.h"

namespace kfoundation {

//\/ KSystemException::StackTraceItem /\///////////////////////////////////////

  void KFException::StackTraceItem::set(char* str) {
    Ref<BufferInputStream> input = new BufferInputStream((kf_octet_t*)str,
        (kf_int32_t)strlen(str), false);

    StreamParser parser(input.AS(InputStream));

    parser.skipSpaces();
    parser.readNumber(_index);

    parser.skipSpaces();
    StringPrintWriter pw1;
    parser.readAllBeforeSpace(pw1.getStream());
    _exeName = pw1.toString();

    parser.skipSpaces();
    StringPrintWriter pw2;
    parser.readAllAlphanumeric(pw2.getStream());
    _address = pw2.toString();

    parser.skipSpaces();
    StringPrintWriter pw3;
    parser.readAllBeforeSpace(pw3.getStream());
    _symbol = System::demangle(pw3.toString());

    parser.skipSpaces();
    parser.readChar('+');
    parser.skipSpaces();
    parser.readNumber(_offset);
  }
  
  
  void KFException::StackTraceItem::serialize(Ref<ObjectSerializer> os) const
  {
    os->object(K"StackTraceItem")
      ->attribute(K"function", _symbol)
      ->attribute(K"exeName" , _exeName)
      ->attribute(K"address" , _address)
      ->attribute(K"offset"  , _offset)
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
  
  KFException::KFException(RefConst<UString> msg)
  : _name(K"KFException"), _message(msg)
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
    _stackTrace = other._stackTrace;
    /// TODO shouldn't copy _what ?
  }
  
  
  /** Deconstructor */
  
  KFException::~KFException() throw() {
    // Nothing
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
    
    _stackTrace = new Array<StackTraceItem>(_nFrames);

    for(int i = 0; i < _nFrames; i++) {
      _stackTrace->at(i).set(symbolList[i]);
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
  
  void KFException::setName(RefConst<UString> name) {
    _name = name;
    _nHiddenFrames++;
  }
  
  
  void KFException::incrementNumberOfHiddenFrames() {
    _nHiddenFrames++;
  }

  
  /**
   * @return The message assigned to this exception.
   */
  RefConst<UString> KFException::getMessage() const {
    return _message;
  }

  
  // Inherited from SerializingStreamer //
  
  void KFException::serialize(Ref<ObjectSerializer> builder) const {
    builder->object(_name)
      ->attribute(K"message", _message)
      ->attribute(K"thread", Thread::getNameOfCurrentThread())
      ->member(K"stackTrace")->collection();
    
    for(int i = _nHiddenFrames; i < _nFrames; i++) {
      builder->object(_stackTrace->at(i));
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
    if(_what.isNull()) {
      _what = toString();
    }
    return _what->getCString();
  }
  
} // namespace kfoundation