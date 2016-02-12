#ifndef APPLE_H
#define APPLE_H

#include <kfoundation/KFObject.h>
#include <kfoundation/SerializingStreamer.h>
#include <kfoundation/StreamDeserializer.h>

using namespace kfoundation;

class Apple : public KFObject, public SerializingStreamer {

  private: RefConst<UString> _color;

  public: Apple();
  public: Apple(RefConst<UString> color);
  
  public: RefConst<UString> getColor() const;

  // Inherited from SerializingStreamer
  public: void serialize(Ref<ObjectSerializer> os) const;
  
  // Inherited from StreamDeserializer
  public: void deserialize(Ref<ObjectToken> headToken);

};

#endif // APPLE_H