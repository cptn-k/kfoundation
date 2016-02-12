//
//  XmlObjectStreamReader.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "RefArray.h"
#include "UString.h"
#include "UChar.h"
#include "Int64.h"
#include "Ref.h"
#include "Bool.h"
#include "ObjectSerializer.h"
#include "StreamParser.h"
#include "PrintWriter.h"
#include "BufferOutputStream.h"
#include "StringPrintWriter.h"

// Self
#include "XmlObjectStreamReader.h"

namespace kfoundation {
  
//\/ XmlCustomBasicParser /\///////////////////////////////////////////////////
  
  class XmlCustomBasicParser : public StreamParser {
    protected: bool isValidIdentifierChar(const wchar_t& ch) const;
    public: XmlCustomBasicParser(Ref<InputStream> input);
  };
  
  bool XmlCustomBasicParser::isValidIdentifierChar(const wchar_t &ch) const {
    return isAlphanumeric(ch) || ch == '_' || ch == '-';
  }
  
  XmlCustomBasicParser::XmlCustomBasicParser(Ref<InputStream> input)
    : StreamParser(input)
  {
    // Nothing;
  }
  
  
//\/ XmlElement /\/////////////////////////////////////////////////////////////
  
  class XmlElement : public ObjectToken, public Streamer {

  // --- FIELDS --- //

    private: Ref<XmlObjectStreamReader> _owner;
    private: RefConst<UString> _className;
    private: RefConst<UString> _id;
    private: bool         _hasId;


  // --- CONSTRUCTORS --- //
    
    public: XmlElement(Ref<XmlObjectStreamReader> owner, const CodeRange& range,
        RefConst<UString> tagName, RefConst<UString> id);
    
    public: XmlElement(Ref<XmlObjectStreamReader> owner, const CodeRange& range,
        RefConst<UString> tagName);
    

  // --- METHODS --- //

    // From ObjectToken
    public: RefConst<UString> getClassName() const;
    public: RefConst<UString> getIdentifier() const;
    public: bool hasIdentifier() const;
    
    // From Object::ObjectStreamReader
    Ref<Token> next() throw(ParseException);
    
    // From Streamer
    public: void printToStream(Ref<OutputStream> os) const;

  };


  XmlElement::XmlElement(Ref<XmlObjectStreamReader> owner,
     const CodeRange& range, RefConst<UString> tagName, RefConst<UString> id)
  : ObjectToken(range),
    _className(tagName),
    _id(id),
    _hasId(true)
  {
    _owner = owner;
  }

  
  XmlElement::XmlElement(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range, RefConst<UString> tagName)
  : ObjectToken(range),
    _className(tagName),
    _hasId(false)
  {
    _owner = owner;
  }

  
  RefConst<UString> XmlElement::getClassName() const {
    return _className;
  }


  RefConst<UString> XmlElement::getIdentifier() const {
    return _id;
  }

  
  bool XmlElement::hasIdentifier() const {
    return _hasId;
  }

  
  Ref<Token> XmlElement::next() throw(ParseException) {
    return _owner->next();
  }

  
  void XmlElement::printToStream(Ref<OutputStream> os) const {
    PrintWriter(os) << '<' << *_className << "/>";
  }
  
  
//\/ XmlEndElement /\//////////////////////////////////////////////////////////
  
  class XmlEndElement : public EndObjectToken, public Streamer {

  // --- FIELDS --- //

    private: CodeLocation _begin;
    private: CodeLocation _end;
    private: RefConst<UString> _className;
    private: Ref<XmlObjectStreamReader> _owner;


  // --- CONSTRUCTORS --- //
    
    public: XmlEndElement(Ref<XmlObjectStreamReader> owner,
        const CodeRange& range, RefConst<UString> className);


  // --- METHODS --- //

    // From EndObject
    public: RefConst<UString> getClassName() const;
    
    // From EndObject::ObejctStreamItem
    public: Ref<Token> next() throw(ParseException);
    
    // From Streamer
    public: void printToStream(Ref<OutputStream> os) const;

  };


  XmlEndElement::XmlEndElement(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range, RefConst<UString> name)
  : EndObjectToken(range),
    _className(name)
  {
    _owner = owner;
  }


  RefConst<UString> XmlEndElement::getClassName() const {
    return _className;
  }

  
  Ref<Token> XmlEndElement::next() throw(ParseException) {
    return _owner->next();
  }
  
  void XmlEndElement::printToStream(Ref<OutputStream> os) const {
    PrintWriter(os) << "</" << *_className << '>';
  }
  
  
//\/ XmlAttribute /\///////////////////////////////////////////////////////////
  
  class XmlAttribute : public AttributeToken, public Streamer {

  // --- FIELDS -- //

    private: RefConst<UString> _name;
    private: RefConst<UString> _value;
    private: bool _isEmpty;
    private: Ref<XmlObjectStreamReader> _owner;


  // --- CONSTRUCTORS --- //
    
    public: XmlAttribute(Ref<XmlObjectStreamReader> owner,
        const CodeRange& range, RefConst<UString> name, RefConst<UString> value);
    
    public: XmlAttribute(Ref<XmlObjectStreamReader> owner,
        const CodeRange& range, RefConst<UString> name);
    

  // --- METHODS --- //

    // From Attribute
    public: RefConst<UString> getName() const;
    public: RefConst<UString> getValue() const;
    public: bool isEmpty() const;
    
    // From Attribute::ObjectStreamReader
    public: Ref<Token> next() throw(ParseException);
    
    // From Streamer
    public: void printToStream(Ref<OutputStream> stream) const;

  };

  
  XmlAttribute::XmlAttribute(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range, RefConst<UString> name, RefConst<UString> value)
  : AttributeToken(range),
    _name(name),
    _value(value),
    _isEmpty(false),
    _owner(owner)
  {
    // Nothing;
  }


  XmlAttribute::XmlAttribute(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range, RefConst<UString> name)
  : AttributeToken(range),
    _name(name),
    _isEmpty(true)
  {
    _owner = owner;
  }


  RefConst<UString> XmlAttribute::getName() const {
    return _name;
  }
  
  RefConst<UString> XmlAttribute::getValue() const {
    return _value;
  }

  
  bool XmlAttribute::isEmpty() const {
    return _isEmpty;
  }


  Ref<Token> XmlAttribute::next() throw(ParseException) {
    return _owner->next();
  }


  void XmlAttribute::printToStream(Ref<OutputStream> os) const {
    PrintWriter pw(os);
    pw << *_name;
    if(!_isEmpty) {
      pw << "=\"" << *_value << "\"";
    }
  }
  
  
//\/ XmlText /\////////////////////////////////////////////////////////////////
  
  class XmlText : public TextToken {

  // --- FIELDS --- //

    private: CodeLocation _begin;
    private: CodeLocation _end;
    private: Ref<XmlObjectStreamReader> _owner;
    private: RefConst<UString> _value;


  // --- CONSTRUCTORS --- //

    public: XmlText(Ref<XmlObjectStreamReader> owner, const CodeRange& range,
        RefConst<UString> value);


  // --- METHODS --- //

    // From Text
    RefConst<UString> get() const;
    
    // From Text::ObjectStreamReader
    Ref<Token> next() throw(ParseException);

  };


  XmlText::XmlText(Ref<XmlObjectStreamReader> owner, const CodeRange& range,
      RefConst<UString> value)
  : TextToken(range),
    _value(value)
  {
    _owner = owner;
  }


  RefConst<UString> XmlText::get() const {
    return _value;
  }


  Ref<Token> XmlText::next() throw(ParseException) {
    return _owner->next();
  }
  
  
//\/ XmlCollection /\//////////////////////////////////////////////////////////
  
  class XmlCollection : public CollectionToken {

  // --- FIELDS --- //

    private: Ref<XmlObjectStreamReader> _owner;
    private: RefConst<UString> _id;
    private: bool _hasId;


  // --- CONSTRUCTORS --- //
    
    public: XmlCollection(Ref<XmlObjectStreamReader> owner,
        const CodeRange& range, RefConst<UString> id);
    
    XmlCollection(Ref<XmlObjectStreamReader> owner, const CodeRange& range);
    

  // --- METHODS --- //

    // From CollectionToken
    public: RefConst<UString> getIdentifier() const;
    public: bool hasIdentifier() const;

    // From CollectionToken::ObjectStreamReader
    Ref<Token> next() throw(ParseException);

  };


  XmlCollection::XmlCollection(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range, RefConst<UString> id)
  : CollectionToken(range),
    _id(id)
  {
    _owner = owner;
    _hasId = true;
  }

  
  XmlCollection::XmlCollection(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range)
  : CollectionToken(range)
  {
    _hasId = false;
  }
  

  RefConst<UString> XmlCollection::getIdentifier() const {
    return _id;
  }

  
  bool XmlCollection::hasIdentifier() const {
    return !_hasId;
  }

  
  Ref<Token> XmlCollection::next() throw(ParseException) {
    return _owner->next();
  }
  
  
//\/ XmlEndCollection /\///////////////////////////////////////////////////////
  
  class XmlEndCollection : public EndCollectionToken {

  // --- FIELDS --- //

    private: Ref<XmlObjectStreamReader> _owner;


  // --- CONSTRUCTORS --- //
    
    public: XmlEndCollection(Ref<XmlObjectStreamReader> owner,
        const CodeRange& range);

    // From EndCollectionToken::ObjectStreamReader
    public: Ref<Token> next() throw(ParseException);

  };


  XmlEndCollection::XmlEndCollection(Ref<XmlObjectStreamReader> owner,
      const CodeRange& range)
  : EndCollectionToken(range)
  {
    _owner = owner;
  }
  

  Ref<Token> XmlEndCollection::next() throw(ParseException) {
    return _owner->next();
  }
  
  
//\/ XmlObjectStreamReader /\//////////////////////////////////////////////////

  const StaticRefConst<UString> XmlObjectStreamReader::COLLECTION_TAG_NAME = new UString("_collection_");
  const StaticRefConst<UString> XmlObjectStreamReader::ID_ATTRIB_NAME = new UString("_id");
  const StaticRefConst<UString> XmlObjectStreamReader::HEADER_TAG_BEGIN = new UString("<?xml");
  const StaticRefConst<UString> XmlObjectStreamReader::HEADER_TAG_END = new UString("?>");
  const StaticRefConst<UString> XmlObjectStreamReader::END_TAG_BEGIN = new UString("</");
  const StaticRefConst<UString> XmlObjectStreamReader::END_TAG_END = new UString("/>");
  const StaticRefConst<UString> XmlObjectStreamReader::QOUT = new UString("qout");
  const StaticRefConst<UString> XmlObjectStreamReader::AMP = new UString("amp");
  const StaticRefConst<UString> XmlObjectStreamReader::APOS = new UString("apos");
  const StaticRefConst<UString> XmlObjectStreamReader::LT = new UString("lt");
  const StaticRefConst<UString> XmlObjectStreamReader::GT = new UString("gt");
  const StaticRefConst<UString> XmlObjectStreamReader::CDATA_BEGIN = new UString("<!CDATA[[");
  const StaticRefConst<UString> XmlObjectStreamReader::CDATA_END = new UString("]]>");
  const UChar XmlObjectStreamReader::SINGLE_QOUTE = '\'';
  const UChar XmlObjectStreamReader::DOUBLE_QOUTE = '"';
  const UChar XmlObjectStreamReader::EQUAL_SIGN = '=';
  const UChar XmlObjectStreamReader::AND_SIGN = '&';
  const UChar XmlObjectStreamReader::NUMBER_SIGN = '#';
  const UChar XmlObjectStreamReader::BEGIN_CHAR = '<';
  const UChar XmlObjectStreamReader::END_CHAR = '>';
  const UChar XmlObjectStreamReader::SEMICOLON = ';';

  
  /**
   * Constructor.
   * 
   * @param input A stream containing XML representation of an object.
   */
  
  XmlObjectStreamReader::XmlObjectStreamReader(Ref<InputStream> input) {
    _state = INITIAL;
    _input = input;
    _parser = new StreamParser(input);
    _elementStack = new RefConstArray<UString>();
  }

  
  bool XmlObjectStreamReader::parseHeader() throw(ParseException) {
    if(!_parser->readSequence(HEADER_TAG_BEGIN)) {
      return false;
    }
    
    _parser->skipSpacesAndNewLines();
    
    while(!_parser->readSequence(HEADER_TAG_END)) {
      Ref<XmlAttribute> attr = readAttribute();
      if(attr.isNull()) {
        throw ParseException(K"Expected XML attribute",
            _parser->getCodeLocation());
      }
      
      if(attr->getName()->equals(K"encoding")) {
        if(!attr->getValue()->equals(K"utf-8")) {
          throw ParseException(K"XML encoding is not utf-8",
              attr->codeRange.getBegin());
        }
      }
      
      _parser->skipSpacesAndNewLines();
    }
    
    _state = INITIAL;
    
    return true;
  }
  
  
  Ref<XmlAttribute> XmlObjectStreamReader::readAttribute()
  throw(ParseException)
  {
    _parser->skipSpacesAndNewLines();
    CodeLocation begin = _parser->getCodeLocation();

    Ref<BufferOutputStream> storage = new BufferOutputStream();

    UChar strBeginChar;
    bool hasValue = false;
    
    if(_parser->readIdentifier(storage.AS(OutputStream)) == 0) {
      return NULL;
    }

    RefConst<UString> name = new UString(storage->getData(), storage->getSize());
    storage->clear();

    _parser->skipSpaces();
    
    if(_parser->readChar(EQUAL_SIGN)) {
      hasValue = true;
      
      _parser->skipSpaces();
      
      if(_parser->readChar(DOUBLE_QOUTE)) {
        strBeginChar = DOUBLE_QOUTE;
      } else if(_parser->readChar(SINGLE_QOUTE)) {
        strBeginChar = SINGLE_QOUTE;
      } else {
        throw ParseException(K"Double quot expected", begin);
      }
      
      readStringUnescaped(storage.AS(OutputStream), strBeginChar);
      if(!_parser->readChar(strBeginChar)) {
        throw ParseException(K"Closing '" + UChar(strBeginChar)
            + "' could not be found", _parser->getCodeLocation());
      }
    }
    
    CodeLocation end = _parser->getCodeLocation();
    
    if(!hasValue) {
      return new XmlAttribute(this, CodeRange(begin, end), name);
    }
    
    return new XmlAttribute(this, CodeRange(begin, end), name,
        storage->getString());
  }
  
  
  Ref<XmlElement> XmlObjectStreamReader::readElement() throw(ParseException) {
    Ref<BufferOutputStream> storage = new BufferOutputStream();
    RefConst<UString> name;
    RefConst<UString> id;
    bool hasId = false;
    
    _parser->skipSpacesAndNewLines();
    
    CodeLocation begin = _parser->getCodeLocation();
    
    if(!_parser->readChar(BEGIN_CHAR)) {
      return NULL;
    }
    
    if(!_parser->readIdentifier(storage.AS(OutputStream))) {
      throw ParseException(K"Tag name expected", _parser->getCodeLocation());
    }
    
    CodeLocation end = _parser->getCodeLocation();

    name = new UString(storage->getData(), storage->getSize());
    storage->clear();

    _elementStack->push(name);

    _nextAttrib = readAttribute();
    if(!_nextAttrib.isNull()) {
      if(_nextAttrib->checkName(ID_ATTRIB_NAME)) {
        id = _nextAttrib->getValue();
        _nextAttrib = NULL;
        hasId = true;
      }
    }
    
    _state = ELEMENT_HEAD;
    
    if(hasId) {
      return new XmlElement(this, CodeRange(begin, end), name, id);
    }
 
    return new XmlElement(this, CodeRange(begin, end), name);
  }
  
  
  Ref<Token> XmlObjectStreamReader::readElementOrCollection()
  throw(ParseException)
  {
    Ref<XmlElement> element = readElement();
    
    if(element.isNull()) {
      return NULL;
    }
    
    if(element->getClassName()->equals(COLLECTION_TAG_NAME)) {
      Ref<XmlCollection> collection;
      if(element->hasIdentifier()) {
        collection = new XmlCollection(this, element->codeRange,
            element->getIdentifier());
      } else {
        collection = new XmlCollection(this, element->codeRange);
      }
      return collection.AS(Token);
    }
    
    return element.AS(Token);
  }


  void XmlObjectStreamReader::readStringUnescaped(Ref<OutputStream> storage,
     const UChar endChar)
  {
    UChar ch;

    while(!_parser->testChar(endChar)) {
      if(_parser->testEndOfStream()) {
        throw ParseException(K"Stream eneded while reading text section");
      }
      
      if(_parser->readChar(AND_SIGN)) {
        if(_parser->readChar(NUMBER_SIGN)) {
          if(_parser->readChar('x')) {
            kf_int32_t code;
            if(_parser->readHexNumber(code)) {
              UChar((wchar_t)code).printToStream(storage);
            } else {
              throw ParseException(K"Could not parse hex character code",
                  _parser->getCodeLocation());
            }
          } else {
            kf_int32_t code = 0;
            if(_parser->readNumber(code)) {
              UChar((wchar_t)code).printToStream(storage);
            } else {
              throw ParseException(K"Could not parse character code",
                  _parser->getCodeLocation());
            }
          }
        } else if(_parser->readSequence(QOUT)) {
          storage->write('"');
        } else if(_parser->readSequence(AMP)) {
          storage->write('&');
        } else if(_parser->readSequence(APOS)) {
          storage->write('\'');
        } else if(_parser->readSequence(LT)) {
          storage->write('>');
        } else if(_parser->readSequence(GT)) {
          storage->write('<');
        } else {
          throw ParseException(K"Unknown escape sequence",
              _parser->getCodeLocation());
        }
        
        if(!_parser->readChar(SEMICOLON)) {
          throw ParseException(K"Semicolon expected",
              _parser->getCodeLocation());
        }
      } else {
        _parser->readAny(ch);
        ch.printToStream(storage);
      }
    } // white()
  }
  
  
  Ref<XmlEndElement> XmlObjectStreamReader::readCloseTag() {
    CodeLocation begin = _parser->getCodeLocation();
    if(!_parser->readSequence(END_TAG_BEGIN)) {
      return Ref<XmlEndElement>();
    }
    
    _parser->skipSpaces();

    Ref<BufferOutputStream> storage = new BufferOutputStream();
    _parser->readIdentifier(storage.AS(OutputStream));
    RefConst<UString> name = new UString(storage->getData(), storage->getSize());
    storage->clear();

    _parser->skipSpaces();
    
    if(!_parser->readChar(END_CHAR)) {
      throw ParseException(K"'>' expected", _parser->getCodeLocation());
    }
    
    CodeLocation end = _parser->getCodeLocation();
    
    if(!name->equals(_elementStack->last())) {
      throw ParseException(K"Closing tag \"" + name + "\" does not match its"
          " corresponding opening tag " + *_elementStack->last(),
          CodeRange(begin, end));
    }
    
    _elementStack->pop();
    
    _state = ELEMENT_END;
    
    return new XmlEndElement(this, CodeRange(begin, end), name);
  }
  
  
  Ref<Token> XmlObjectStreamReader::readEndElementOrEndCollection()
  throw(ParseException)
  {
    Ref<XmlEndElement> endElement = readCloseTag();
    
    if(endElement.isNull()) {
      return Ref<Token>();
    }
    
    if(endElement->getClassName()->equals(COLLECTION_TAG_NAME)) {
      Ref<XmlEndCollection> endCollection(
          new XmlEndCollection(this, endElement->codeRange));
      return endCollection.AS(Token);
    }
    
    return endElement.AS(Token);
  }
  
  
  /**
   * Returns the next token in the stream.
   */
  
  Ref<Token> XmlObjectStreamReader::next() throw(ParseException) {
    _parser->skipSpacesAndNewLines();
    
    if(_parser->testEndOfStream()) {
      return new EndStreamToken();
    }
    
    switch (_state) {
      case INITIAL:
        parseHeader();
        return readElement().AS(Token);
      
      case ELEMENT_HEAD: {
        if(!_nextAttrib.isNull()) {
          Ref<Token> t = _nextAttrib.AS(Token);
          _nextAttrib = NULL;
          return t;
        }
        
        _parser->skipSpacesAndNewLines();
        
        Ref<XmlAttribute> attr = readAttribute();
        if(!attr.isNull()) {
          return attr.AS(Token);
        }
        
        if(_parser->testSequence(END_TAG_END)) {
          CodeLocation begin = _parser->getCodeLocation();
          _parser->readSequence(END_TAG_END);
          CodeLocation end = _parser->getCodeLocation();
          RefConst<UString> name = _elementStack->pop();
          _state = ELEMENT_END;
          
          if(name->equals(COLLECTION_TAG_NAME)) {
            return new XmlEndCollection(this, CodeRange(begin, end));
          }
          
          return new XmlEndElement(this, CodeRange(begin, end), name);
        }
        
        if(!_parser->readChar(END_CHAR)) {
          throw ParseException(K"'>' expected", _parser->getCodeLocation());
        }
        
        _parser->skipSpacesAndNewLines();
        
        if(_parser->readSequence(CDATA_BEGIN)) {
          CodeLocation begin = _parser->getCodeLocation();
          Ref<BufferOutputStream> storage = new BufferOutputStream();
          _parser->readAllBeforeSequence(CDATA_END, storage.AS(OutputStream));
          CodeLocation end = _parser->getCodeLocation();
          if(!_parser->readSequence(CDATA_END)) {
            throw ParseException(K"CDATA section is not properly closed "
                "with \"]]>\"", begin);
          }
          _state = TEXT;
          return new XmlText(this, CodeRange(begin, end), storage->getString());
        }
        
        Ref<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag;
        }
        
        Ref<Token> child = readElementOrCollection();
        if(!child.isNull()) {
          return child;
        }
        
        CodeLocation begin = _parser->getCodeLocation();
        Ref<BufferOutputStream> storage = new BufferOutputStream();
        kf_int64_t n = _parser->readAllBeforeChar(BEGIN_CHAR, storage.AS(OutputStream));
        if(n == 0) {
          throw ParseException(K"Error reading element body", begin);
        }
        _state = TEXT;
        CodeLocation end = _parser->getCodeLocation();
        return new XmlText(this, CodeRange(begin, end), storage->getString());
      }
        
      case ELEMENT_END: {
        _parser->skipSpacesAndNewLines();
        if(_parser->testEndOfStream()) {
          if(_elementStack->getSize() > 0) {
            StringPrintWriter pw;
            pw << "The following elements are not closed: ";
            for(int i = 0; i < _elementStack->getSize(); i++) {
              if(i > 0) {
                pw << ", ";
              }
              pw << *_elementStack->at(i);
            }
            throw ParseException(pw.toString());
          }
          _state = END;
          return new EndStreamToken();
        }
        
        Ref<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag;
        }
        
        Ref<Token> nextElement = readElementOrCollection();
        if(!nextElement.isNull()) {
          return nextElement;
        }
        
        throw ParseException(K"Open or close tag expected",
            _parser->getCodeLocation());
      }
        
      case TEXT: {
        Ref<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag;
        }
        
        Ref<Token> nextElement = readElementOrCollection();
        if(!nextElement.isNull()) {
          return nextElement;
        }
        
        throw ParseException(K"Open or close tag expected",
            _parser->getCodeLocation());
      }
        
      case END:
        throw ParseException(K"Parse is already finished");
        
    } // case(_state)
    
    throw ParseException(K"Invalid XML reader state");
    
  } // next()
  
} // namespace kfoundation