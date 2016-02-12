#include "Apple.h"

#include <kfoundation/Ref.h>
#include <kfoundation/UString.h>
#include <kfoundation/Logger.h>


Apple::Apple() {
  // Nothing;
}


Apple::~Apple() {
  LOG << *this << " deleted" << OVER;
}


void Apple::printToStream(Ref<OutputStream> stream) const {
  (K"An Apple")->printToStream(stream);
}