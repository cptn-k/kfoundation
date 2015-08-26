/*---[NumericVectorDecl.h]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::NumericVectorDecl::*
 |  Implements: -
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

#ifndef KFOUNDATION_NUMERICVECTOR_DECL
#define KFOUNDATION_NUMERICVECTOR_DECL

#include "Array.h"
#include "Streamer.h"

namespace kfoundation {
  
  /**
   * A subclass of Array, adds numeric operations. Can also convert the contents
   * to and from string.
   *
   * @ingroup containers
   * @headerfile NumericVector.h <kfounadtion/NumericVector.h>
   */
  
  template<typename T>
  class NumericVector : public Array<T>, public Streamer {
  public:
    NumericVector();
    NumericVector(T* values, kf_int32_t size);
    
    Ptr< NumericVector<T> > negate() const;
    Ptr< NumericVector<T> > add(const Ptr< NumericVector<T> >& other) const;
    Ptr< NumericVector<T> > sub(const Ptr< NumericVector<T> >& other) const;
    Ptr< NumericVector<T> > mul(const T& coef) const;

    static Ptr< NumericVector<T> > parseInt(const string& str);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_NUMERICVECTOR_DECL) */
