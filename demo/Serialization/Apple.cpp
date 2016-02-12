#include "Apple.h"

#include <kfoundation/Ref.h>
#include <kfoundation/UString.h>
#include <kfoundation/Logger.h>
#include <kfoundation/ObjectSerializer.h>
#include <kfoundation/ObjectStreamReader.h>

Apple::Apple() {
  _color = K"red";
}

Apple::Apple(RefConst<UString> color)
: _color(color)
{
  // Nothing;
}

RefConst<UString> Apple::getColor() const {
  return _color;
}

void Apple::serialize(Ref<ObjectSerializer> os) const {
  os->object(K"Apple")->attribute(K"color", _color)->endObject();
}

void Apple::deserialize(Ref<ObjectToken> headToken) {
  headToken->validateClass(K"Apple");
  _color = headToken->next()->asAttribute()->validateName(K"color")->getValue();
  headToken->next()->validateType(Token::END_OBJECT);
}