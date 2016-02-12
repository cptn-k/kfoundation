#include "Person.h"
#include "Apple.h"

#include <kfoundation/Ref.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>

Person::Person(RefConst<UString> name)
: _name(name)
{
  // Nothing;
}

Person::~Person() {
  LOG << "Person " << _name << " deleted." << OVER;
}

void Person::setApple(Ref<Apple> apple) {
  _apple = apple;
}

void assignApple(Ref<Apple> apple) {
  LOG << apple.toString() << OVER;
  Ref<Person> hanna = new Person(K"Hanna");
  Ref<Person> billy = new Person(K"Billy");
  hanna->setApple(apple);
  LOG << apple.toString() << OVER;
  billy->setApple(apple);
  LOG << apple.toString() << OVER;
}

int main(int argc, char** argv) {
  Ref<Apple> anApple = new Apple();
  LOG << anApple.toString() << OVER;
  assignApple(anApple);
  LOG << anApple.toString() << OVER;
}