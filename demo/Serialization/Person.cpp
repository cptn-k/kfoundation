#include "Person.h"
#include "Apple.h"

#include <kfoundation/Ref.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>
#include <kfoundation/ObjectSerializer.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/StringInputStream.h>

Person::Person() {
  _apple = new Apple();
}

Person::Person(RefConst<UString> name)
: _name(name)
{
  // Nothing;
}

RefConst<UString> Person::getName() const {
  return _name;
}

void Person::setApple(Ref<Apple> apple) {
  _apple = apple;
}

RefConst<Apple> Person::getApple() const {
  return _apple;
}

void Person::serialize(Ref<ObjectSerializer> serializer) const {
  serializer->object(K"Person")
    ->attribute(K"name", _name)
    ->member(K"apple")->object<Apple>(_apple)
    ->endObject();
}

void Person::deserialize(Ref<ObjectToken> headToken) {
  headToken->validateClass(K"Person");
  _name = headToken->next()->asAttribute()->validateName(K"name")->getValue();
  Ref<Token> token = headToken->next();
  if(token->is(Token::OBJECT)) {
    _apple->deserialize(token.AS(ObjectToken));
    token = token->next();
  }
  token->validateType(Token::END_OBJECT);
}