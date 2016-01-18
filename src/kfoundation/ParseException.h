//
//  ParseException.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KFoundation_XCode_Wrapper__ParseException__
#define __KFoundation_XCode_Wrapper__ParseException__

// Internal
#include "CodeLocation.h"
#include "CodeRange.h"
#include "RefDecl.h"

// Super
#include "KFException.h"

namespace kfoundation {
  
  /**
   * Thrown when a parsing error happens. Stores a string message and a
   * CodeLocation or CodeRange if needed.
   *
   * @ingroup io
   * @ingroup utils
   * @ingroup exceptions
   * @headerfile ParseException.h <kfoundation/ParseException.h>
   */
  
  class ParseException : public KFException {

  // --- FIELDS --- //

    private: CodeLocation _begin;
    private: CodeLocation _end;
    private: bool _hasLocation;
    private: bool _hasRange;


  // --- (DE)CONSTRUCTORS --- //
    
    public: ParseException(RefConst<UString> message);
    public: ParseException(RefConst<UString> message, const CodeLocation& location);
    public: ParseException(RefConst<UString> message, const CodeRange& range);
    public: ~ParseException() throw();


  // --- METHODS --- //

    bool hasLocation() const;
    bool hasRange() const;
    const CodeLocation& getBegin() const;
    const CodeLocation& getEnd() const;

  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation_XCode_Wrapper__ParseException__) */