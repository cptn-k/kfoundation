//
//  IOException.h
//  KFoundation
//
//  Created by Kay Khandan on 10/27/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__IOException__
#define __KFoundation__IOException__

#include "KFException.h"

namespace kfoundation {
  
  class IOException : public KFException {
    public: IOException(const string& message);
  };
  
}

#endif /* defined(__KFoundation__IOException__) */
