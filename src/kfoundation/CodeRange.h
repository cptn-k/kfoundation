//
//  CodeRange.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 4/7/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__CodeRange__
#define __KFoundation__CodeRange__

#include "CodeLocation.h"

namespace kfoundation {
  
  class CodeRange : public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: CodeLocation _begin;
    private: CodeLocation _end;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: CodeRange(const CodeLocation& begin, const CodeLocation& end);
    public: const CodeLocation& getBegin() const;
    public: const CodeLocation& getEnd() const;
    public: void serialize(PPtr<ObjectSerializer> serializer) const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__CodeRange__) */
