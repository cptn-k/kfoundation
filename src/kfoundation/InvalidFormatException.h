//
//  InvalidFormatException.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_INVALIDFORMATEXCEPTION
#define KFOUNDATION_INVALIDFORMATEXCEPTION

#include "KFException.h"

namespace kfoundation {
  
  class InvalidFormatException : public KFException {
  public:
    InvalidFormatException(string message);
  };
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_INVALIDFORMATEXCEPTION) */
