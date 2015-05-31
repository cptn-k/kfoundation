//
//  IOException.cpp
//  KFoundation
//
//  Created by Kay Khandan on 10/27/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#include "IOException.h"

namespace kfoundation {
  
  IOException::IOException(const string& message)
  : KFException(message)
  {
    setName("IOException");
  }
  
}
