#ifndef ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H
#define ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H

// Super
#include "Streamer.h"

namespace kfoundation {

  template<typename T> class Ref;
  template<typename T> class RefConst;
  class ObjectSerializer;
  class UString;
  
  /**
   * Objects implementing this class can be serialized into any format
   * allowed by ObjectSerializer. The output is used to provide compatibility
   * with Streamer interfaceo.
   *
   * @ingroup serialization
   * @headerfile SerializingStreamer.h <kfoundation/SerializingStreamer.h>
   */

  class SerializingStreamer : public Streamer {
  public:
    virtual ~SerializingStreamer();
    virtual void serialize(Ref<ObjectSerializer> builder) const = 0;
    virtual RefConst<UString> toString() const;
    virtual void printToStream(Ref<OutputStream> os) const;
  };
  
} // namespace kfoundation

#endif