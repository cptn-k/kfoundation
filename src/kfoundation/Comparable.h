//
//  Comparable.h
//  KFoundation
//
//  Created by Kay Khandan on 11/27/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef Comparable_h
#define Comparable_h

#include "definitions.h"
#include "RefDecl.h"

namespace kfoundation {

  template<typename T>
  class Comparable {
    public: virtual bool equals(RefConst<T> other) const = 0;
  };

} // namespace kfoundation

#endif /* Comparable_h */