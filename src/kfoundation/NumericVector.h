/*---[NumericVector.h]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::NumericVectorDecl::*
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef KFOUNDATION_NUMERICVECTOR
#define KFOUNDATION_NUMERICVECTOR

#include "KFException.h"
#include "NumericVectorDecl.h"
#include "StringInputStream.h"
#include "PredictiveParserBase.h"

namespace kfoundation {

  
  /**
   * Default constructor, creates an empty array.
   */
  
  template<typename T>
  NumericVector<T>::NumericVector()
    : Array<T>()
  {
    // Nothing;
  }

  
  /**
   * Constructor, creats a new NumericVector copying the specified elements
   * from a C-style array.
   *
   * @param values The begining of the C-style array containing values to be
   *               copied.
   * @param size The number of elements to be copied.
   */
  
  template<typename T>
  NumericVector<T>::NumericVector(T* values, kf_int32_t size)
  : Array<T>(values, size)
  {
    // Nothing;
  }

  
  /**
   * Returns the pointer to a new NumericVector whos elements are the negative
   * of their corresponding elements in this object.
   */
  
  template<typename T>
  Ptr< NumericVector<T> > NumericVector<T>::negate() const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(- Array<T>::get(i));
    }
    return result.retain();
  }

  
  /**
   * Returns the pointer to a new NumericVector representing the summation
   * of this vector with the given one.
   *
   * @param other The vector to add this one to.
   */
  
  template<typename T>
  Ptr< NumericVector<T> >
  NumericVector<T>::add(const Ptr< NumericVector<T> >& other) const {
    Ptr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) + other->get(i));
    }
    return result.retain();
  }
  

  /**
   * Returns the pointer to a new NumericVector representing the substaction of
   * the given vector from this one.
   *
   * @param other The vector to substract from this one.
   */
  
  template<typename T>
  Ptr< NumericVector<T> >
  NumericVector<T>::sub(const Ptr< NumericVector<T> >& other) const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) - other->get(i));
    }
    return result.retain();
  }

  
  /**
   * Resturns the pointer to a new NumericVector resulted from multiplying this
   * vector to the given scalar value.
   *
   * @param coef The scalar value to multiply this vector to.
   */
  
  template<typename T>
  Ptr< NumericVector<T> > NumericVector<T>::mul(const T& coef) const {
    AutoPtr< NumericVector<T> > result(new NumericVector<T>());
    for(kf_int32_t i = Array<T>::getSize() - 1; i >= 0; i--) {
      result->set(Array<T>::get(i) * coef);
    }
    return result.retain();
  }
  
  
  /**
   * Returns the pointer to a new NumericVector parsing the given string.
   * A valid input string should have a format like "{1.1, 2.3, 4.2, 3}".
   *
   * @param str The string to parse.
   */
  
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