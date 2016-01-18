//
//  StreamingSerializer.cpp
//  KnoRBA-Common
//
//  Created by Hamed KHANDAN on 9/6/13.
//
//

// Internal
#include "Ref.h"
#include "ObjectSerializer.h"
#include "OutputStream.h"
#include "UString.h"
#include "BufferOutputStream.h"

// Self
#include "SerializingStreamer.h"

namespace kfoundation {

  SerializingStreamer::~SerializingStreamer() {
    // Nothing
  }


  void SerializingStreamer::printToStream(Ref<OutputStream> stream) const {

  }
  
  
  RefConst<UString> SerializingStreamer::toString() const {
    Ref<BufferOutputStream> stream = new BufferOutputStream();
    printToStream(stream.AS(OutputStream));
    return new UString(stream->getData(), stream->getSize());
  }


  /**
   * @fn kfoundation::SerializingStreamer::serialize(Ref<ObjectSerializer>)
   *
   * Implements compatibility with SerializingStreamer interface.
   */

} // namespace kfoundation