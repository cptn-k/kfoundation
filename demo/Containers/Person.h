#ifndef PERSON_H
#define PERSON_H

#include <kfoundation/KFObject.h>
#include <kfoundation/Streamer.h>

using namespace kfoundation;

class Person : public KFObject, public Streamer {

  private: RefConst<UString> _name;

  public: Person(RefConst<UString> name);
  public: ~Person();

  // Inherited from Streamer
  public: void printToStream(Ref<OutputStream> stream) const;

};

#endif /* PERSON_H */