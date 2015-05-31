#ifndef ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H
#define ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H

#include <ostream>

#include "Streamer.h"

using namespace std;

namespace kfoundation {

  template<typename T>
  class PPtr;

  class ObjectSerializer;

  class SerializingStreamer : public Streamer {
  private:
    void printToStream(int otype, ostream& os) const;
    string toString(int otype) const;
  public:
    virtual ~SerializingStreamer() {
      // Nothing
    }
    virtual string toString() const;
    virtual void serialize(PPtr<ObjectSerializer> builder) const = 0;
    virtual void printToStream(ostream& os) const;
    void printToJsonStream(ostream& os) const;
    void printToXmlStream(ostream& os) const;
    string toJsonString() const;
    string toXmlString() const;
  };
  
} // namespace kfoundation

#endif
