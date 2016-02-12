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
#include "RefCountMemoryManager.h"
#include "RefArray.h"
#include "RefDictionary.h"
#include "StreamParser.h"
#include "BufferOutputStream.h"
#include "XmlObjectSerializer.h"
#include "JsonObjectSerializer.h"
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include "Tuple.h"
#include "Range.h"
#include "RangeIterator.h"
#include "ProximityIterator.h"
#include "Timer.h"

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


void testException() {
  try {
    Ref<UString> myStr;
    myStr->getOctetCount();
  } catch(KFException e) {
    XmlObjectSerializer::print(LOG_ERR, e) << OVER;
    LOG_ERR << e << OVER;
  }
  LOG << "This program won’t crash!" << OVER;
}


void testSystem() {
  System::OUT->print(K"Current Time: ")
    ->printTime(K"%Y:%m:%d %H:%M:%S", System::getCurrentTimeInMiliseconds())
    ->over();
  
  System::OUT << K"Exe Path: " << *System::getExePath() << NEW_LINE
      << K"Exe name: " << System::getExePath()->getFileNameWithExtension() << NEW_LINE
      << K"Working Dir: " << *System::getCurrentWorkingDirectory() << NEW_LINE
      << K"Process ID: " << System::getPid() << NEW_LINE
      << K"Number of Cores: " << System::getNumberOfCpuCores() << OVER;
}


void testRef() {
  Ref<UString> str = new UString();
  LOG << (RefCountMemoryManager&)System::getMasterMemoryManager().getDefaultManager() << OVER;
}


void testUString() {
  RefConst<UString> myString = K"My 日本語";

  for(UCharIterator i = myString->getUCharIterator(); i.hasMore(); i.next()) {
    LOG << i << " consumes " << (int)i.getOctetCount() << " octet(s)" << OVER;
  }
}


void testParser() {
  Ref<StreamParser> parser = new StreamParser(System::IN);

  kf_int64_t number = parser->readInteger();
  LOG << number << OVER;

  Ref<UString> str = parser->readAllBeforeNewLine();
  LOG << str << OVER;
}


void testRange() {
  Range r(Tuple2D(4, 4));
  
  Range innerR = r.shrink(1);
  System::OUT << "innerR: " << innerR << OVER;
  System::OUT << "number of cells in range: "
      << innerR.getVolume() << OVER;
  
  ProximityIterator j(1);
  for(RangeIterator i(innerR); i.hasMore(); i.next()) {
    System::OUT << "Cells around " << i << ": ";
    for(j.centerAt(i); j.hasMore(); j.next()) {
      System::OUT << j << ", ";
    }
    System::OUT << OVER;
  }
  
}


int main(int argc, char** argv) {
  Timer t;
  
  t.start();
  testRange();
  double time = t.get();
  
  LOG << "Elapsed Time: " << time << 's' << OVER;
}