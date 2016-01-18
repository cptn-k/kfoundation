//
//  StringPrintWriter.hpp
//  KFoundation
//
//  Created by Kay Khandan on 12/14/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef StringPrintWriter_hpp
#define StringPrintWriter_hpp

// Super
#include "PrintWriter.h"
#include "Streamer.h"

namespace kfoundation {

  class BufferOutputStream;
  class UString;

  class StringPrintWriter : public PrintWriter, public Streamer {

  // --- CONSTRUCTOR --- //

    public: StringPrintWriter();
    public: StringPrintWriter(const kf_int32_t capacity);


  // --- METHODS --- //

    public: void clear();

    // From Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };

} // namespace kfoundation

#endif /* StringPrintWriter_hpp */