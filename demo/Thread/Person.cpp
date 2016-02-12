#include "Person.h"

#include <kfoundation/Ref.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>

Person::Person(RefConst<UString> name)
: _name(name)
{
  // Nothing;
}

void Person::countTo(kf_int16_t n) {
  Ref<UString> threadName = Thread::getNameOfCurrentThread();

  for(int i = 0; i < n; i++) {
    KF_SYNCHRONIZED(_mutex, {
      System::OUT << threadName << ": \"" << i << "\"" << OVER;
    })
  }
}

MyThread::MyThread(Ref<Person> p)
: _person(p)
{
  // Empty;
}

void MyThread::run() {
  _person->countTo(10);
}

int main(int argc, char** argv) {
  Ref<Person> hanna = new Person(K"Hanna");
  Ref<MyThread> thread1 = new MyThread(hanna);
  Ref<MyThread> thread2 = new MyThread(hanna);
  thread1->start();
  thread2->start();
  System::exitAfterAllThreads();
}