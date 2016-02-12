#ifndef PERSON_H
#define PERSON_H

#include <kfoundation/KFObject.h>
#include <kfoundation/SerializingStreamer.h>
#include <kfoundation/StreamDeserializer.h>

using namespace kfoundation;

class Apple;

class Person : public KFObject, public SerializingStreamer,
    public StreamDeserializer
{

  private: RefConst<UString> _name;
  public: Ref<Apple> _apple;

  public: Person();
  public: Person(RefConst<UString> name);

  public: RefConst<UString> getName() const;
  public: void setApple(Ref<Apple> apple);
  public: RefConst<Apple> getApple() const;

  // From SerializingStreamer
  public: void serialize(Ref<ObjectSerializer> serializer) const;

  // From StreamDeserializer
  public: void deserialize(Ref<ObjectToken> headToken);

};

#endif /* PERSON_H */