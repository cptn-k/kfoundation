//
//  ManagedWrapper.h
//  KFoundation
//
//  Created by Kay Khandan on 11/27/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef ManagedWrapper_h
#define ManagedWrapper_h

#include "Comparable.h"

namespace kfoundation {

  template<typename T>
  class KFWrapper
  : public virtual KFObject, public Comparable< KFWrapper<T> >
  {
    private: T _value;
    public : KFWrapper(const T& value);
    public : T& get();

    // Inherited from Comparable<T>
    public: bool equals(Ref< KFWrapper<T> > other) const;
  };


  template<typename T>
  KFWrapper<T>::KFWrapper(const T& value) {
    _value = value;
  }


  template<typename T>
  T& KFWrapper<T>::get() {
    return _value;
  }


  template<typename T>
  bool KFWrapper<T>::equals(Ref< KFWrapper<T> > other) const {
    return _value == other->get();
  }

} // namespace kfoundation

#endif /* ManagedWrapper_h */