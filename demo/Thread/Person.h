#ifndef PERSON_H
#define PERSON_H

#include <kfoundation/KFObject.h>
#include <kfoundation/Streamer.h>
#include <kfoundation/Thread.h>
#include <kfoundation/Mutex.h>

using namespace kfoundation;

class Person : public KFObject {
  private: RefConst<UString> _name;
  private: Mutex _mutex;

  public: Person(RefConst<UString> name);
  
  public: void countTo(kf_int16_t n);
};


class MyThread : public Thread {
  public: Ref<Person> _person;
  public: MyThread(Ref<Person> p);
  public: void run();
};

#endif /* PERSON_H */