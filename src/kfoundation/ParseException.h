//
//  ParseException.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KFoundation_XCode_Wrapper__ParseException__
#define __KFoundation_XCode_Wrapper__ParseException__

#include "KFException.h"
#include "CodeLocation.h"
#include "CodeRange.h"
#include "PtrDecl.h"

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
  private:
    CodeLocation _begin;
    CodeLocation _end;
    bool _hasLocation;
    bool _hasRange;
    
  public:
    ParseException(const string& message);
    ParseException(const string& message, const CodeLocation& location);
    ParseException(const string& message, const CodeRange& range);
    ~ParseException() throw();
    
    bool hasLocation() const;
    bool hasRange() const;
    
    const CodeLocation& getBegin() const;
    const CodeLocation& getEnd() const;
  };
  
}

#endif /* defined(__KFoundation_XCode_Wrapper__ParseException__) */
