#include "Person.h"

#include <kfoundation/Ref.h>
#include <kfoundation/Logger.h>
#include <kfoundation/UString.h>
#include <kfoundation/RefArray.h>
#include <kfoundation/RefDictionary.h>

Person::Person(RefConst<UString> name)
: _name(name)
{
  // Nothing;
}

Person::~Person() {
  LOG << "Person " << _name << " deleted." << OVER;
}

void Person::printToStream(Ref<OutputStream> stream) const {
  _name->printToStream(stream);
}

void arrayDemo() {
  Ref< RefArray<Person> > people = new RefArray<Person>();
  people->push(new Person(K"Hanna"));
  people->push(new Person(K"Billy"));
  people->push(new Person(K"太郎"));

  LOG << "People: " << *people << OVER;

  for(RefArray<Person>::Iterator i = people->getIterator(); i.hasMore(); i.next()) {
    LOG << "Person at index " << i.getIndex() << " is " << *i << OVER;
  }
}


void dictionaryDemo() {
  Ref< RefDictionary<UString, Int32> > grades = new RefDictionary<UString, Int32>();
  grades->set(K"Hanna", new Int32(90));
  grades->set(K"Billy", new Int32(85));
  grades->set(K"太郎"  , new Int32(87));

  LOG << K"Grades: " << *grades << OVER;

  LOG << K"Billy's grade: " << *grades->get(K"Billy") << OVER;
}


void stringDemo() {
  RefConst<UString> phrase = K"太郎 = Tarō";

  kf_int64_t n = phrase->getCharCount();
  kf_int64_t ws = n * sizeof(wchar_t);

  LOG << "Phrase: " << phrase << OVER;
  LOG << "Number of characters: " << n << OVER;
  LOG << "Size in memory: " << phrase->getOctetCount() << " bytes" << OVER;
  LOG << "Size as wstring: " << ws << " bytes" << OVER;
  for(UCharIterator i = phrase->getUCharIterator(); i.hasMore(); i.next()) {
    LOG << "Char '" << i << "' consumes " << (int)i.getOctetCount() << " bytes"
        << OVER;
  }
}


int main(int argc, char** argv) {
  LOG << "Array Demo" << OVER;
  arrayDemo();

  LOG << "Dictionary Demo" << OVER;
  dictionaryDemo();

  LOG << "String Demo" << OVER;
  stringDemo();
}