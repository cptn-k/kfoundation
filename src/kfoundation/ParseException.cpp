//
//  ParseException.cp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "Ref.h"
#include "Int64.h"
#include "UString.h"

// Self
#include "ParseException.h"

#define KF_PARSEEXCEPTION_NAME K"ParseException"

namespace kfoundation {
  
  
  /**
   * Constructor, creates a new instance with the given message but no code
   * location.
   *
   * @param message A message describing the error.
   */
  
  ParseException::ParseException(RefConst<UString> message)
    : KFException(message)
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _hasLocation = false;
    _hasRange = false;
  }
  
  
  /**
   * Constructor, stores the given string message and CodeLocation to report
   * when thrown.
   *
   * @param message A message describing the error.
   * @param The location of the parsed stream in which the error is observed.
   */
  
  ParseException::ParseException(RefConst<UString> message,
      const CodeLocation& location)
  : KFException(message + " (Line: " + location.getLine()
        + ", Col: " + location.getCol() + ")")
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _begin = location;
    _hasLocation = true;
  }
  
  
  /**
   * Constructor, stores the given string message and CoreRange to report
   * when thrown.
   *
   * @param message A message describing the error.
   * @param range The code range in which the error is observed.
   */
  
  ParseException::ParseException(RefConst<UString> message, const CodeRange& range)
  : KFException(message + " (Line: " + range.getBegin().getLine()
      + ", Col: " + range.getBegin().getCol() + ")")
  {
    setName(KF_PARSEEXCEPTION_NAME);
    _begin = range.getBegin();
    _end = range.getEnd();
    _hasLocation = true;
    _hasRange = true;
  }
  
  
  /**
   * Deconstrcutor.
   */
  
  ParseException::~ParseException() throw() {
    // Nothing;
  }
  
  
  /**
   * Checks if a CodeLocation is stored in this object.
   */
  
  bool ParseException::hasLocation() const {
    return _hasLocation;
  }
  
  
  /**
   * Checks if a CodeRange is stored in this object.
   */
  
  bool ParseException::hasRange() const {
    return _hasRange;
  }
  
  
  /**
   * Returns the begining of the range that the error is observed.
   * Can be used when either hasLocation() or hadRange() is `true`.
   */
  
  const CodeLocation& ParseException::getBegin() const {
    return _begin;
  }
  
  
  /**
   * Returns the end of the range that the error is observed.
   * Can be used only if hasRange() is `true`.
   */
  
  const CodeLocation& ParseException::getEnd() const {
    return _end;
  }
  
} // namespace kfoundation