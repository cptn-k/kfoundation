//
//  XmlObjectStreamReader.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_XMLSTREAMREADER
#define KFOUNDATION_XMLSTREAMREADER

#include "KFObject.h"
#include "RefDecl.h"
#include "RefArrayDecl.h"
#include "ObjectStreamReader.h"
#include "ParseException.h"
#include "UChar.h"

namespace kfoundation {

  class XmlStreamItem;
  class XmlAttribute;
  class XmlElement;
  class XmlEndElement;
  class StreamParser;
  class UString;
  class InputStream;

  
  
  /**
   * ObjectStreamReader to deserialize XML streams.
   *
   * @see ObjectStreamReader
   * @see StreamDeserializer
   * @ingroup io
   * @headerfile XmlObjectStreamReader.h <kfoundation/XmlObjectStreamReader.h>
   */
  
  class XmlObjectStreamReader : public ObjectStreamReader {

  // --- NESTED TYPES --- //

    private: enum state {
      INITIAL,
      ELEMENT_HEAD,
      ELEMENT_END,
      TEXT,
      END
    };


  // --- STATIC FIELDS --- //

    public: static const StaticRefConst<UString> COLLECTION_TAG_NAME;
    public: static const StaticRefConst<UString> ID_ATTRIB_NAME;
    public: static const StaticRefConst<UString> HEADER_TAG_BEGIN;
    public: static const StaticRefConst<UString> HEADER_TAG_END;
    public: static const StaticRefConst<UString> END_TAG_BEGIN;
    public: static const StaticRefConst<UString> END_TAG_END;
    public: static const StaticRefConst<UString> QOUT;
    public: static const StaticRefConst<UString> AMP;
    public: static const StaticRefConst<UString> APOS;
    public: static const StaticRefConst<UString> LT;
    public: static const StaticRefConst<UString> GT;
    public: static const StaticRefConst<UString> CDATA_BEGIN;
    public: static const StaticRefConst<UString> CDATA_END;
    public: static const UChar SINGLE_QOUTE;
    public: static const UChar DOUBLE_QOUTE;
    public: static const UChar EQUAL_SIGN;
    public: static const UChar AND_SIGN;
    public: static const UChar NUMBER_SIGN;
    public: static const UChar BEGIN_CHAR;
    public: static const UChar END_CHAR;
    public: static const UChar SEMICOLON;


  // --- FIELDS --- //

    private: state             _state;
    private: Ref<InputStream>  _input;
    private: Ref<StreamParser> _parser;
    private: Ref<XmlAttribute> _nextAttrib;
    private: Ref< RefConstArray<UString> >  _elementStack;


  // --- CONSTRUCTORS --- //

    public: XmlObjectStreamReader(Ref<InputStream> input);


  // --- METHODS --- //

   private: bool parseHeader() throw(ParseException);
   private: Ref<XmlAttribute> readAttribute() throw(ParseException);
   private: Ref<XmlElement> readElement() throw(ParseException);
   private: Ref<Token> readElementOrCollection() throw(ParseException);
   private: Ref<Token> readEndElementOrEndCollection() throw(ParseException);
   private: void readStringUnescaped(Ref<OutputStream>, const UChar endChar);
   private: Ref<XmlEndElement> readCloseTag();
    public: Ref<Token> next() throw(ParseException);

  };
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_XMLSTREAMREADER) */