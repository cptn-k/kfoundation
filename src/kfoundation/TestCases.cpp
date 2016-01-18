//
//  TestCases.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/19/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#include "Ref.h"
#include "System.h"
#include "PrintWriter.h"
#include "UString.h"
#include "Bool.h"
#include "Logger.h"
#include "Path.h"
#include "KforObjectSerializer.h"

using namespace kfoundation;

void testSystemOut() {
  System::OUT->print("Hello World!")->over();
  System::OUT << K"Hello World Again! (" << 123 << ")" << OVER;
}


void testLogger() {
  LOG << "Hello Logger!" << OVER;
}


void testPath() {
  Ref<Path> exePath = System::getExePath();
  Ref<KforObjectSerializer> serializer = new KforObjectSerializer(System::OUT->getStream());
  exePath->serialize(serializer.AS(ObjectSerializer));
  System::OUT << OVER;
}


int main(int argc, char** argv) {
  System::getLogger()->setPrintShortTime(OFF);
  testLogger();
}