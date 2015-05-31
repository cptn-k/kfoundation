//
//  ParseException.cp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "Ptr.h"

#include "ParseException.h"

#define KF_PARSEEXCEPTION_NAME "ParseException"

namespace kfoundation {
  
  ParseException::ParseException(const string& message)
    : KFException(message)
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _hasLocation = false;
    _hasRange = false;
  }
  
  ParseException::ParseException(const string& message, const CodeLocation& location)
    : KFException(message + " (Line: " + LongInt::toString(location.getLine())
                  + ", Col: " + LongInt::toString(location.getCol()) + ")")
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _begin = location;
    _hasLocation = true;
  }
  
  ParseException::ParseException(const string& message, const CodeRange& range)
    : KFException(message + " (Line: " + LongInt::toString(range.getBegin().getLine())
                + ", Col: " + LongInt::toString(range.getBegin().getCol()) + ")")
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _begin = range.getBegin();
    _end = range.getEnd();
    _hasLocation = true;
    _hasRange = true;
  }
  
  ParseException::~ParseException() throw() {
    // Nothing;
  }
  
  bool ParseException::hasLocation() const {
    return _hasLocation;
  }
  
  bool ParseException::hasRange() const {
    return _hasRange;
  }
  
  const CodeLocation& ParseException::getBegin() const {
    return _begin;
  }
  
  const CodeLocation& ParseException::getEnd() const {
    return _end;
  }
  
} // namespace kfoundation