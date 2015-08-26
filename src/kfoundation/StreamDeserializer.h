//
//  StreamDeserializer.h
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/31/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__StreamDeserializer__
#define __KFoundation__StreamDeserializer__

#include "PtrDecl.h"

namespace kfoundation {
  
  class ObjectToken;
  class InputStream;
  
  
  /**
   * Interface to be implemented by any class that can be deserialized from
   * stream.
   *
   * @ingroup io
   * @headefile StreamDeserializer.h <kfoundation/StreamDeserializer.h>
   */
  
  class StreamDeserializer {
    public: virtual void deserialize(PPtr<ObjectToken> headToken) = 0;
    public: void readFromXmlStream(PPtr<InputStream> stream);
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__StreamDeserializer__) */