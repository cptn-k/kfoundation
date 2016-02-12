#ifndef PERSON_H
#define PERSON_H

#include <kfoundation/KFObject.h>

using namespace kfoundation;

class Apple;

class Person : public KFObject {

  private: RefConst<UString> _name;
  public: Ref<Apple> _apple;

  public: Person(RefConst<UString> name);
  public: ~Person();

  public: void setApple(Ref<Apple> apple);

};

#endif /* PERSON_H */