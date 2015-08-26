//
//  XmlObjectStreamReader.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_XMLSTREAMREADER
#define KFOUNDATION_XMLSTREAMREADER

#include "ObjectStreamReader.h"
#include "InputStream.h"
#include "PredictiveParserBase.h"
#include "ParseException.h"

namespace kfoundation {

  class XmlStreamItem;
  class XmlAttribute;
  class XmlElement;
  class XmlEndElement;
  
  
  /**
   * ObjectStreamReader to deserialize XML streams.
   *
   * @see ObjectStreamReader
   * @see StreamDeserializer
   * @ingroup io
   * @headerfile XmlObjectStreamReader.h <kfoundation/XmlObjectStreamReader.h>
   */
  
  class XmlObjectStreamReader : /*public ObjectStreamReader*/ public ManagedObject {
  public: static const string ID_ATTRIB_NAME;
    
  private:
    enum state {
      INITIAL,
      ELEMENT_HEAD,
      ELEMENT_END,
      TEXT,
      END
    };
    
    state                     _state;
    vector<string>            _elementStack;
    Ptr<InputStream>          _input;
    Ptr<PredictiveParserBase> _parser;
    Ptr<XmlAttribute>         _nextAttrib;

    bool parseHeader() throw(ParseException);
    Ptr<XmlAttribute> readAttribute() throw(ParseException);
    Ptr<XmlElement> readElement() throw(ParseException);
    Ptr<Token> readElementOrCollection() throw(ParseException);
    Ptr<Token> readEndElementOrEndCollection() throw(ParseException);
    void readStringUnescaped(string& output, const wchar_t& endChar);
    Ptr<XmlEndElement> readCloseTag();
    
  public:
    XmlObjectStreamReader(PPtr<InputStream> input);
    ~XmlObjectStreamReader();
    
    Ptr<Token> next() throw(ParseException);
  };
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_XMLSTREAMREADER) */
