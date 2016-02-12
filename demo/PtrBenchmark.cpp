//
//  PtrBenchmark.cpp
//  KFoundation
//
//  Created by Kay Khandan on 10/1/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include <memory>

#include <kfoundation/Ref.h>
#include <kfoundation/Timer.h>
#include <kfoundation/Logger.h>
#include <kfoundation/System.h>
#include <kfoundation/RefCountMemoryManager.h>

using namespace std;
using namespace kfoundation;

class TestClass1 {
  private: double _variable;
  public: void set(const double value);
  public: double get() const;
};

void TestClass1::set(const double value) {
  _variable = value;
}

double TestClass1::get() const {
  return _variable;
}


class TestClass2 : public KFObject {
  private: double _variable;
  public: void set(const double value);
  public: double get() const;
};

void TestClass2::set(const double value) {
  _variable = value;
}

double TestClass2::get() const {
  return _variable;
}


class SharedPtrContainer {
  public: shared_ptr<TestClass1> _value;
  public: SharedPtrContainer() : _value(new TestClass1()) {
    // Nothing
  }
};

void testKFPtr(int n) {
  Ref<TestClass2>* array = new Ref<TestClass2>[n];
  for(int i = 0; i < n; i++) {
    array[i] = new TestClass2();
  }
  delete[] array;
}

void testSharedPtr(int n) {
  shared_ptr<TestClass2>* array = new shared_ptr<TestClass2>[n];
  for(int i = 0; i < n; i++) {
    array[i] = shared_ptr<TestClass2>(new TestClass2());
  }
  delete[] array;
}

double testKFPtrLookup(int n) {
  Ref<TestClass2>* array = new Ref<TestClass2>[n];
  for(int i = n - 1; i >= 0; i--) {
    array[i] = new TestClass2();
  }

  double time = 0;
  Timer timer;

//  for(int i = 999; i >= 0; i--) {
    timer.start();
    Ref<TestClass2> first = array[0];
    Ref<TestClass2> middle = array[n/2];
    Ref<TestClass2> last = array[n - 1];
    time += timer.get();
//  }

  delete[] array;
  return time;
}


double testSharedPtrLookup(int n) {
  shared_ptr<TestClass2>* array = new shared_ptr<TestClass2>[n];
  for(int i = n - 1; i >= 0; i--) {
    array[i] = shared_ptr<TestClass2>(new TestClass2());
  }

  double time = 0;
  Timer timer;

//  for(int i = 999; i >= 0; i--) {
    timer.start();
    shared_ptr<TestClass2> first = array[0];
    shared_ptr<TestClass2> middle = array[n/2];
    shared_ptr<TestClass2> last = array[n - 1];
    time += timer.get();
//  }

  delete[] array;
  return time;
}


int main(int argc, char** argv) {
  int n = 1000000;

  System::OUT << "Testing ..." << OVER;

  Timer t;
  
  double creationTime = 0;
  double deletionTime = 0;
  double referencingTime = 0;
  
  for(int i = 0; i < n; i++) {
    {
      t.start();
      Ref<TestClass2> ptr(new TestClass2());
      creationTime += t.get();
      Ref<TestClass2> ptrRef = ptr;
      t.start();
      ptr->set(12.345);
      ptr->get();
      referencingTime += t.get();
      t.start();
    }
    deletionTime += t.get();
  }

  LOG << "Ref: Creation: " << creationTime << ", deletion: " << deletionTime
      << ", referecing: " << referencingTime << OVER;


  creationTime = 0;
  deletionTime = 0;
  referencingTime = 0;

  for(int i = 0; i < n; i++) {
    {
      t.start();
      shared_ptr<TestClass2> ptr(new TestClass2());
      creationTime += t.get();
      shared_ptr<TestClass2> ptrRef = ptr;
      t.start();
      ptr->set(12.345);
      ptr->get();
      referencingTime += t.get();
      t.start();
    }
    deletionTime += t.get();
  }

  LOG << "shared_ptr: Creation: " << creationTime << ", deletion: "
      << deletionTime << ", referecing: " << referencingTime << OVER;
  

  TestClass2* ptr3 = new TestClass2();
  t.start();
  for(int i = 0; i < n; i++) {
    ptr3->set(12.345);
    ptr3->get();
  }

  LOG << "Pure Ref: " << t.get() << OVER;

  delete ptr3;


  for(int i = 1; i <  50; i++) {
    printf("%.10f\n", testKFPtrLookup(i*100000));
  }

  printf("\n");

  for(int i = 1; i <  50; i++) {
    printf("%.10f\n", testSharedPtrLookup(i*100000));
  }

  return 0;
}