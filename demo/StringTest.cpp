//
//  StringTest.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/2/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#include <kfoundation/Ref.h>
#include <kfoundation/UString.h>
#include <kfoundation/Int.h>
#include <kfoundation/RefCountMemoryManager.h>
#include <kfoundation/System.h>
#include <kfoundation/PrintWriter.h>

using namespace kfoundation;

void testChar() {
  UChar c(L'日');
  kf_int8_t nOctets = c.getOctetCount();
  System::OUT << "N Octets: " << nOctets << OVER;
  for(int i = 0; i < nOctets; i++) {
    System::OUT->printHex(c.get()[i])->print(' ');
  }
  System::OUT->over();
}


void testString() {
  System::OUT << (K"日本語")->getOctetCount() << OVER;

  Ref<UString> str = new UString(K"日本語 English فارسی");

  System::OUT << *str << OVER;
  for(OctetIterator i = str->getOctetIterator(); i.hasMore(); i.next()) {
    System::OUT->printHex(i.get())->print(' ');
  }
  System::OUT->over();

  for(UCharIterator i = str->getUCharIterator(); i.hasMore(); i.next()) {
    System::OUT << i.getCharIndex() << ". " << i.getOctetCount() << ": " << i << OVER;
  }
}


int main(int argc, char** argv) {
  testString();
}