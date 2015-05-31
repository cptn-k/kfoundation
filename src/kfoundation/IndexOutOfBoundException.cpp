//
//  IndexOutOfBoundException.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "IndexOutOfBoundException.h"

namespace kfoundation {
  
  IndexOutOfBoundException::IndexOutOfBoundException(string message)
    : MemoryException(message)
  {
    setName("IndexOutOfBoundException");
  }
  
} // namespace kfoundation