//
//  InvalidFormatException.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Internal
#include "Ref.h"
#include "UString.h"

// Self
#include "InvalidFormatException.h"

namespace kfoundation {
  
  /**
   * Constrcutor.
   *
   * @param message A message describing the error.
   */
  
  InvalidFormatException::InvalidFormatException(RefConst<UString> message)
    : KFException(message)
  {
    setName(K"InvalidFormatException");
  }
  
} // namespace kfoundation