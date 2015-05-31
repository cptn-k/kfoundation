//
//  NumericArray.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/27/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_NUMERICVECTOR
#define KFOUNDATION_NUMERICVECTOR

#include "KFException.h"
#include "NumericVectorDecl.h"
#include "StringInputStream.h"
#include "PredictiveParserBase.h"

namespace kfoundation {

  template<typename T>
  NumericVector<T>::NumericVector()
    : Array<T>()
  {
    // Nothing;
  }

  
  template<typename T>
  NumericVector<T>::NumericVector(T* values, kf_int32_t size)
  : Array<T>(values, size)
  {
    // Nothing;
  }

  
  template<typename T>
  Ptr< NumericVector<T> > NumericVector<T>::negate() const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(- Array<T>::get(i));
    }
    return result.retain();
  }

  
  template<typename T>
  Ptr< NumericVector<T> >
  NumericVector<T>::add(const Ptr< NumericVector<T> >& other) const {
    Ptr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) + other->get(i));
    }
    return result.retain();
  }
  

  template<typename T>
  Ptr< NumericVector<T> >
  NumericVector<T>::sub(const Ptr< NumericVector<T> >& other) const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) - other->get(i));
    }
    return result.retain();
  }

  
  template<typename T>
  Ptr< NumericVector<T> > NumericVector<T>::mul(const T& coef) const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) * coef);
    }
    return result.retain();
  }
  
  
  template<typename T>
  Ptr< NumericVector<T> > NumericVector<T>::parseInt(const string& str) {
    Ptr< NumericVector<T> > v(new NumericVector<T>());
    
    AutoPtr<StringInputStream> input(new StringInputStream(str));
    AutoPtr<PredictiveParserBase> parser(new PredictiveParserBase(input.AS(InputStream)));
    parser->skipSpacesAndNewLines();
    parser->readChar('{');
    while(true) {
      long int n = 0;
      parser->readNumber(n);
      v->push(n);
      parser->skipSpacesAndNewLines();
      parser->readChar(',');
      parser->skipSpacesAndNewLines();
      if(parser->readChar('}') || parser->testEndOfStream()) {
        return v;
      }
    }
    
    return v;
  }
  
  
  template<typename T>
  void NumericVector<T>::printToStream(ostream& os) const {
    kf_int32_t s = Array<T>::getSize();
    
    os << "{";
    for(int i = 0; i < s; i++) {
      if(i > 0) {
        os << ", ";
      }
      
      os << Array<T>::get(i);
    }
    os << "}";
  }
  
} // namespace kfoundation

#endif /* KFOUNDATION_NUMERICVECTOR */