//
//  StringPrintWriter.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/14/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "Ref.h"
#include "OutputStream.h"
#include "UString.h"
#include "BufferOutputStream.h"

// Self
#include "StringPrintWriter.h"

namespace kfoundation {

  StringPrintWriter::StringPrintWriter()
  : PrintWriter(new BufferOutputStream())
  {
    // Nothing;
  }


  StringPrintWriter::StringPrintWriter(const kf_int32_t capacity)
  : PrintWriter(new BufferOutputStream(capacity))
  {
    // Nothing;
  }


  void StringPrintWriter::clear() {
    getStream().AS(BufferOutputStream)->clear();
  }


  void StringPrintWriter::printToStream(Ref<OutputStream> stream) const {
    BufferOutputStream& src = (BufferOutputStream&)*getStream();
    stream->write(src.getData(), src.getSize());
  }


  RefConst<UString> StringPrintWriter::toString() const {
    return getStream().AS(BufferOutputStream)->getString();
  }

} // namespace kfoundation