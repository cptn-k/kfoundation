//
//  StreamingSerializer.cpp
//  KnoRBA-Common
//
//  Created by Hamed KHANDAN on 9/6/13.
//
//

#include <sstream>

#include "SerializingStreamer.h"
#include "Ptr.h"
#include "ObjectSerializer.h"

namespace kfoundation {

  void SerializingStreamer::printToStream(int otype, ostream& os) const
  {
    Ptr<ObjectSerializer> builder(new ObjectSerializer(os, (ObjectSerializer::output_type_t)otype, 4));
    serialize(builder);
  }

  void SerializingStreamer::printToStream(ostream& os) const {
    printToStream(ObjectSerializer::DUMP, os);
  }

  void SerializingStreamer::printToJsonStream(ostream& os) const {
    printToStream(ObjectSerializer::JSON, os);
  }

  void SerializingStreamer::printToXmlStream(ostream& os) const {
    printToStream(ObjectSerializer::XML, os);
  }

  string SerializingStreamer::toString(int otype)
      const
  {
    stringstream stream;
    this->printToStream(otype, stream);
    return stream.str();
  }

  string SerializingStreamer::toJsonString() const {
    return toString(ObjectSerializer::JSON);
  }

  string SerializingStreamer::toXmlString() const {
    return toString(ObjectSerializer::XML);
  }
  
  string SerializingStreamer::toString() const {
    return toString(ObjectSerializer::DUMP);
  }

} // namespace kfoundation
