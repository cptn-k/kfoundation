//
//  NumericArray.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/27/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_NUMERICVECTOR_DECL
#define KFOUNDATION_NUMERICVECTOR_DECL

#include "Array.h"
#include "Streamer.h"

namespace kfoundation {
  
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
