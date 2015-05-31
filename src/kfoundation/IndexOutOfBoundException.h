#ifndef ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H
#define ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H

#include "MemoryException.h"

namespace kfoundation {

  class IndexOutOfBoundException : public MemoryException {
  public:
    IndexOutOfBoundException(string message);
  };

} // namespace kfoundation

#endif
