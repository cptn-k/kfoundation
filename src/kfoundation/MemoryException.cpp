//
//  MemoryException.cpp
//  KnoRBA-Common
//
//  Created by Hamed KHANDAN on 9/13/13.
//
//

#include "MemoryException.h"

namespace kfoundation {

  MemoryException::MemoryException(string message)
    : KFException(message)
  {
    setName("MemoryException");
  }

} // namespace kfoundation
