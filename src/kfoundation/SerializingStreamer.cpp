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
  
  
  SerializingStreamer::~SerializingStreamer() {
    // Nothing
  }
  
  
  void SerializingStreamer::printToStream(int otype, ostream& os) const
  {
    Ptr<ObjectSerializer> builder(new ObjectSerializer(os, (ObjectSerializer::output_type_t)otype, 4));
    serialize(builder);
  }

  
  /**
   * Implements compatibility with Streamer interface. 
   * Prints the serialization of this object in KFOR format to the given stream.
   * @param os The stream to print to.
   */
  
  void SerializingStreamer::printToStream(ostream& os) const {
    printToStream(ObjectSerializer::DUMP, os);
  }
  
  
  /**
   * Prints the serialization of this object in JSON format to the given stream.
   * @param os The stream to print to.
   */

  void SerializingStreamer::printToJsonStream(ostream& os) const {
    printToStream(ObjectSerializer::JSON, os);
  }
  
  
  /**
   * Prints the serialization of this object in XML format to the given stream.
   * @param os The stream to print to.
   */

  void SerializingStreamer::printToXmlStream(ostream& os) const {
    printToStream(ObjectSerializer::XML, os);
  }
  
  
  string SerializingStreamer::toString(int otype) const
  {
    stringstream stream;
    this->printToStream(otype, stream);
    return stream.str();
  }

  
  /**
   * Returns a string containing the serialization of this object in JSON 
   * format.
   */
  
  string SerializingStreamer::toJsonString() const {
    return toString(ObjectSerializer::JSON);
  }

  
  /**
   * Returns a string containing the serialization of this object in XML format.
   */
  
  string SerializingStreamer::toXmlString() const {
    return toString(ObjectSerializer::XML);
  }
  
  
  /**
   * Returns a string containing the serialization of this object in KFOR
   * format.
   */
  
  string SerializingStreamer::toString() const {
    return toString(ObjectSerializer::DUMP);
  }
  
  
  /**
   * @fn kfoundation::SerializingStreamer::serialize(PPtr<ObjectSerializer>)
   *
   * Implements compatibility with SerializingStreamer interface.
   */

} // namespace kfoundation
