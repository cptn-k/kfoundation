//
//  InvalidFormatException.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "InvalidFormatException.h"

namespace kfoundation {
  
  /**
   * Constrcutor.
   *
   * @param message A message describing the error.
   */
  
  InvalidFormatException::InvalidFormatException(string message)
    : KFException(message)
  {
    setName("InvalidFormatException");
  }
  
} // namespace kfoundation