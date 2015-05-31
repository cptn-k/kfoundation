//
//  CodeLocation.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_CODEPOINT
#define KFOUNDATION_CODEPOINT

#include "ManagedObject.h"
#include "SerializingStreamer.h"
#include "PtrDecl.h"

namespace kfoundation {

  class CodeLocation : public SerializingStreamer {
  private:
    long int _line;
    long int _col;
    long int _charIndex;
    long int _byteIndex;
    
  public:
    CodeLocation();
    void set(const CodeLocation& other);
    void set(long int line, long int col, long int byteIndex);
    void set(long int line, long int col, long int charIndex, long int byteIndex);
    
    CodeLocation& setLine(long int line);
    CodeLocation& setCol(long int col);
    CodeLocation& setByteIndex(long int byteIndex);
    CodeLocation& setCharIndex(long int charIndex);
    
    inline long int getLine() const;
    inline long int getCol() const;
    inline long int getByteIndex() const;
    inline long int getCharIndex() const;
    
    // From SrializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
  };
  
  inline long int CodeLocation::getLine() const {
    return _line;
  }
  
  inline long int CodeLocation::getCol() const {
    return _col;
  }
  
  inline long int CodeLocation::getByteIndex() const {
    return _byteIndex;
  }
  
  inline long int CodeLocation::getCharIndex() const {
    return _charIndex;
  }
  
} // namespace kfoundation

#endif /* defined(__KFoundation_XCode_Wrapper__CodePoint__) */
