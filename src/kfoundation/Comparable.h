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

  /**
   * Interface indicating the implementing class can be compared to the given
   * type of values.
   */

  template<typename T>
  class Comparable {

    /**
     * Returns true of the given reference equals to the given object.
     */

    public: virtual bool equals(RefConst<T> other) const = 0;
    
  };

} // namespace kfoundation

#endif /* Comparable_h */