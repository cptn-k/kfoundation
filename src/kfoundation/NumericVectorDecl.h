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

// Super
#include "definitions.h"
#include "RefDecl.h"
#include "Array.h"
#include "Streamer.h"

namespace kfoundation {

  class UString;
  class OutputStream;
  
  /**
   * A subclass of Array, adds numeric operations. Can also convert the contents
   * to and from string.
   *
   * @ingroup containers
   * @headerfile NumericVector.h <kfounadtion/NumericVector.h>
   */
  
  template<typename T>
  class NumericVector : public Array<T>, public Streamer {

  // --- CONSTRUCTORS --- //

    public: NumericVector();
    public: NumericVector(T* values, kf_int32_t size);


  // --- STATIC METHODS --- //

    public: static Ref< NumericVector<kf_int32_t> > parseInt(RefConst<UString> str);

  // --- METHODS --- //
    
    public: Ref< NumericVector<T> > negate() const;
    public: Ref< NumericVector<T> > add(RefConst< NumericVector<T> >& other) const;
    public: Ref< NumericVector<T> > sub(RefConst< NumericVector<T> >& other) const;
    public: Ref< NumericVector<T> > mul(const T coef) const;

    // From Streamer
    void printToStream(Ref<OutputStream> stream) const;
  };
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_NUMERICVECTOR_DECL) */
