//
//  StreamDeserializer.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 3/31/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "InputStream.h"
#include "XmlObjectStreamReader.h"

// Self
#include "StreamDeserializer.h"

namespace kfoundation {
  
  void StreamDeserializer::readFromXmlStream(PPtr<InputStream> stream) {
    Ptr<XmlObjectStreamReader> reader = new XmlObjectStreamReader(stream);
    Ptr<Token> headToken = reader->next();
    headToken->validateType(Token::OBJECT);
    deserialize(headToken.AS(ObjectToken));
  }
  
} // KFoundation