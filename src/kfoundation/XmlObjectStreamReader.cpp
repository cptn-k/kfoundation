//
//  XmlObjectStreamReader.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "XmlObjectStreamReader.h"
#include "UniString.h"
#include "UniChar.h"
#include "LongInt.h"
#include "Ptr.h"
#include "Bool.h"
#include "ObjectSerializer.h"
#include "Logger.h"

namespace kfoundation {
  
  // --- XmlCustomBasicParser -------------------------------------------------
  
  class XmlCustomBasicParser : public PredictiveParserBase {
  protected:
    bool isValidIdentifierChar(const wchar_t& ch) const;
    
  public:
    XmlCustomBasicParser(PPtr<InputStream> input);
  };
  
  bool XmlCustomBasicParser::isValidIdentifierChar(const wchar_t &ch) const {
    return isAlphanumeric(ch) || ch == '_' || ch == '-';
  }
  
  XmlCustomBasicParser::XmlCustomBasicParser(PPtr<InputStream> input)
    : PredictiveParserBase(input.retain())
  {
    // Nothing;
  }
  
  
//\/ XmlElement /\/////////////////////////////////////////////////////////////
  
  class XmlElement : public ObjectToken, public Streamer {
  private:
    PPtr<XmlObjectStreamReader> _owner;
    string       _className;
    string       _id;
    bool         _hasId;
    
  public:
    XmlElement(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& tagName, const string& id);
    
    XmlElement(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& tagName);
    
    ~XmlElement();
    
    // From Object
    const string& getClassName() const;
    const string& getIdentifier() const;
    bool hasIdentifier() const;
    
    // From Object::ObjectStreamReader
    Ptr<Token> next() throw(ParseException);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };
  
  XmlElement::XmlElement(PPtr<XmlObjectStreamReader> owner,
     const CodeRange& range, const string& tagName, const string& id)
  : ObjectToken(range),
    _className(tagName),
    _id(id),
    _hasId(true)
  {
    _owner = owner;
  }
  
  XmlElement::XmlElement(PPtr<XmlObjectStreamReader> owner,
      const CodeRange& range, const string& tagName)
  : ObjectToken(range),
    _className(tagName),
    _hasId(false)
  {
    _owner = owner;
  }
  
  XmlElement::~XmlElement() {
    // Nothing;
  }
  
  const string& XmlElement::getClassName() const {
    return _className;
  }
  
  const string& XmlElement::getIdentifier() const {
    return _id;
  }
  
  bool XmlElement::hasIdentifier() const {
    return _hasId;
  }
  
  Ptr<Token> XmlElement::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  void XmlElement::printToStream(ostream &os) const {
    os << '<' << _className << "/>";
  }
  
  
// --- XmlEndElement --------------------------------------------------------
  
  class XmlEndElement : public EndObjectToken, public Streamer {
  private:
    CodeLocation _begin;
    CodeLocation _end;
    string _className;
    PPtr<XmlObjectStreamReader> _owner;
    
  public:
    XmlEndElement(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& name);
    
    ~XmlEndElement();
    
    // From EndObject
    const string& getClassName() const;
    
    // From EndObject::ObejctStreamItem
    Ptr<Token> next() throw(ParseException);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };
  
  XmlEndElement::XmlEndElement(PPtr<XmlObjectStreamReader> owner,
      const CodeRange& range, const string& name)
  : EndObjectToken(range),
    _className(name)
  {
    _owner = owner;
  }
  
  XmlEndElement::~XmlEndElement() {
    // Nothing;
  }
  
  const string& XmlEndElement::getClassName() const {
    return _className;
  }
  
  Ptr<Token> XmlEndElement::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  void XmlEndElement::printToStream(ostream &os) const {
    os << "</" << _className << '>';
  }
  
  
//\/ XmlAttribute /\///////////////////////////////////////////////////////////
  
  class XmlAttribute : public AttributeToken, public Streamer {
  private:
    string _name;
    string _value;
    bool _isEmpty;
    PPtr<XmlObjectStreamReader> _owner;
    
  public:
    XmlAttribute(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& name, const string& value);
    
    XmlAttribute(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& name);
    
    ~XmlAttribute();
    
    // From Attribute
    const string& getName() const;
    const string& getValue() const;
    bool isEmpty() const;
    
    // From Attribute::ObjectStreamReader
    Ptr<Token> next() throw(ParseException);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };
  
  XmlAttribute::XmlAttribute(PPtr<XmlObjectStreamReader> owner,
      const CodeRange& range, const string& name, const string& value)
  : AttributeToken(range),
    _name(name),
    _value(value),
    _isEmpty(false)
  {
    _owner = owner;
  }
  
  XmlAttribute::XmlAttribute(PPtr<XmlObjectStreamReader> owner,
      const CodeRange& range, const string& name)
  : AttributeToken(range),
    _name(name),
    _isEmpty(true)
  {
    _owner = owner;
  }
  
  XmlAttribute::~XmlAttribute() {
    // Nothing
  }
  
  const string& XmlAttribute::getName() const {
    return _name;
  }
  
  const string& XmlAttribute::getValue() const {
    return _value;
  }
  
  bool XmlAttribute::isEmpty() const {
    return _isEmpty;
  }
  
  Ptr<Token> XmlAttribute::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  void XmlAttribute::printToStream(ostream &os) const {
    os << _name;
    if(!_isEmpty) {
      os << "=\"" << _value << "\"";
    }
  }
  
  
//\/ XmlText /\////////////////////////////////////////////////////////////////
  
  class XmlText : public TextToken {
  private:
    CodeLocation _begin;
    CodeLocation _end;
    PPtr<XmlObjectStreamReader> _owner;
    string _value;
    
  public:
    XmlText(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
        string value);
    
    ~XmlText();
    
    // From Text
    const string& get() const;
    
    // From Text::ObjectStreamReader
    Ptr<Token> next() throw(ParseException);
  };
  
  XmlText::XmlText(PPtr<XmlObjectStreamReader> owner, const CodeRange& range,
      string value)
  : TextToken(range),
    _value(value)
  {
    _owner = owner;
  }
  
  XmlText::~XmlText() {
    // Nothing;
  }
  
  const string& XmlText::get() const {
    return _value;
  }
  
  Ptr<Token> XmlText::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  
// --- XmlCollection --------------------------------------------------------
  
  class XmlCollection : public CollectionToken {
  private:
    PPtr<XmlObjectStreamReader> _owner;
    string _id;
    bool _hasId;
    
  public:
    XmlCollection(Ptr<XmlObjectStreamReader> owner, const CodeRange& range,
        const string& id);
    
    XmlCollection(Ptr<XmlObjectStreamReader> owner, const CodeRange& range);
    
    ~XmlCollection();
    
    // From CollectionToken
    const string& getIdentifier() const;
    bool hasIdentifier() const;
    
    
    // From CollectionToken::ObjectStreamReader
    Ptr<Token> next() throw(ParseException);
  };
  
  XmlCollection::XmlCollection(Ptr<XmlObjectStreamReader> owner,
      const CodeRange& range, const string& id)
  : CollectionToken(range),
    _id(id)
  {
    _owner = owner;
    _hasId = true;
  }
  
  XmlCollection::XmlCollection(Ptr<XmlObjectStreamReader> owner,
      const CodeRange& range)
  : CollectionToken(range)
  {
    _hasId = false;
  }
  
  XmlCollection::~XmlCollection() {
    // Nothing;
  }
  
  const string& XmlCollection::getIdentifier() const {
    return _id;
  }
  
  bool XmlCollection::hasIdentifier() const {
    return !_hasId;
  }
  
  Ptr<Token> XmlCollection::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  
// --- XmlEndCollection -----------------------------------------------------
  
  class XmlEndCollection : public EndCollectionToken {
  private:
    PPtr<XmlObjectStreamReader> _owner;
    
  public:
    XmlEndCollection(PPtr<XmlObjectStreamReader> owner, const CodeRange& range);
    ~XmlEndCollection();
    
    // From EndCollectionToken::ObjectStreamReader
    Ptr<Token> next() throw(ParseException);
  };
  
  XmlEndCollection::XmlEndCollection(PPtr<XmlObjectStreamReader> owner,
      const CodeRange& range)
  : EndCollectionToken(range)
  {
    _owner = owner;
  }
  
  XmlEndCollection::~XmlEndCollection() {
    // Nothing
  }
  
  Ptr<Token> XmlEndCollection::next() throw(ParseException) {
    return _owner->next().retain();
  }
  
  
// --- XmlObjectStreamReader ------------------------------------------------------
  
  const string XmlObjectStreamReader::ID_ATTRIB_NAME = "_id";
  
  
  /**
   * Constructor.
   * 
   * @param input A stream containing XML representation of an object.
   */
  
  XmlObjectStreamReader::XmlObjectStreamReader(PPtr<InputStream> input) {
    _state = INITIAL;
    _input = input;
    _parser = new PredictiveParserBase(input);
    
    parseHeader();
  }
  
  
  /**
   * Deconstructor.
   */
  
  XmlObjectStreamReader::~XmlObjectStreamReader() {
    // Nothing;
  }
  
  
  bool XmlObjectStreamReader::parseHeader() throw(ParseException) {
    if(!_parser->readSequence(L"<?xml")) {
      return false;
    }
    
    _parser->skipSpacesAndNewLines();
    
    while(!_parser->readSequence(L"?>")) {
      Ptr<XmlAttribute> attr = readAttribute();
      if(attr.isNull()) {
        throw ParseException("Expected XML attribute", _parser->getCodeLocation());
      }
      
      if(UniString::areEqualIgnoreCases(attr->getName(), "encoding")) {
        if(!UniString::areEqualIgnoreCases(attr->getValue(), "utf-8")) {
          throw ParseException("XML encoding is not utf-8", attr->codeRange.getBegin());
        }
      }
      
      _parser->skipSpacesAndNewLines();
    }
    
    _state = INITIAL;
    
    return true;
  }
  
  
  Ptr<XmlAttribute> XmlObjectStreamReader::readAttribute() throw(ParseException) {
    _parser->skipSpacesAndNewLines();
    CodeLocation begin = _parser->getCodeLocation();
    string name;
    string value;
    char strBeginChar = 0;
    bool hasValue = false;
    
    if(_parser->readIdentifier(name) == 0) {
      return Ptr<XmlAttribute>();
    }
    
    _parser->skipSpaces();
    
    if(_parser->readChar('=')) {
      hasValue = true;
      
      _parser->skipSpaces();
      
      if(_parser->readChar('"')) {
        strBeginChar = '"';
      } else if(_parser->readChar('\'')) {
        strBeginChar = '\'';
      } else {
        throw ParseException("Double quot expected", begin);
      }
      
      readStringUnescaped(value, strBeginChar);
      if(!_parser->readChar(strBeginChar)) {
        throw ParseException("Closing '" + UniChar(strBeginChar) + "' could not be found", _parser->getCodeLocation());
      }
    }
    
    CodeLocation end = _parser->getCodeLocation();
    
    if(!hasValue) {
      return new XmlAttribute(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), name);
    }
    
    Ptr<XmlAttribute> attrib = new XmlAttribute(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), name, value);
    return attrib.retain();
  }
  
  
  Ptr<XmlElement> XmlObjectStreamReader::readElement() throw(ParseException) {
    string name;
    string id;
    bool hasId = false;
    
    _parser->skipSpacesAndNewLines();
    
    CodeLocation begin = _parser->getCodeLocation();
    
    if(!_parser->readChar('<')) {
      return NULL;
    }
    
    if(!_parser->readIdentifier(name)) {
      throw ParseException("Tag name expected", _parser->getCodeLocation());
    }
    
    CodeLocation end = _parser->getCodeLocation();
    
    _elementStack.push_back(name);
    
    _nextAttrib = readAttribute();
    if(!_nextAttrib.isNull()) {
      if(_nextAttrib->checkName("_id")) {
        id = _nextAttrib->getValue();
        _nextAttrib = NULL;
        hasId = true;
      }
    }
    
    _state = ELEMENT_HEAD;
    
    if(hasId) {
      return new XmlElement(getPtr().AS(XmlObjectStreamReader),
          CodeRange(begin, end), name, id);
    }
 
    return new XmlElement(getPtr().AS(XmlObjectStreamReader),
        CodeRange(begin, end), name);
  }
  
  
  Ptr<Token> XmlObjectStreamReader::readElementOrCollection()
  throw(ParseException)
  {
    Ptr<XmlElement> element = readElement();
    
    if(element.isNull()) {
      return Ptr<Token>();
    }
    
    if(element->getClassName() == ObjectSerializer::COLLECTION_CLASS_NAME) {
      Ptr<XmlCollection> collection;
      if(element->hasIdentifier()) {
        collection = new XmlCollection(getPtr().AS(XmlObjectStreamReader), element->codeRange, element->getIdentifier());
      } else {
        collection = new XmlCollection(getPtr().AS(XmlObjectStreamReader), element->codeRange);
      }
      element.release();
      return collection.AS(Token).retain();
    }
    
    return element.AS(Token).retain();
  }
  
  void XmlObjectStreamReader::readStringUnescaped(string& output, const wchar_t& endChar) {
    while(!_parser->testChar(endChar)) {
      if(_parser->testEndOfStream()) {
        throw ParseException("Stream eneded while reading text section");
      }
      
      if(_parser->readChar(L'&')) {
        if(_parser->readChar(L'#')) {
          if(_parser->readChar((const wchar_t[2]){'x', 'X'}, 2)) {
            string str;
            _parser->readNumber(str);
            wchar_t code = (wchar_t)LongInt::parse(str, LongInt::HEXADECIMAL);
            kf_octet_t buffer[6];
            int s = UniChar::writeUtf8(code, buffer);
            output.append((char*)buffer, s);
          } else {
            long int code = 0;
            _parser->readNumber(code);
            kf_octet_t buffer[6];
            int s = UniChar::writeUtf8((wchar_t)code, buffer);
            output.append((char*)buffer, s);
          }
        } else if(_parser->readSequence(L"qout")) {
          output.append("\"");
        } else if(_parser->readSequence(L"amp")) {
          output.append("&");
        } else if(_parser->readSequence(L"apos")) {
          output.append("'");
        } else if(_parser->readSequence(L"lt")) {
          output.append("<");
        } else if(_parser->readSequence(L"gt")) {
          output.append(">");
        } else {
          throw ParseException("Unknown escape sequence", _parser->getCodeLocation());
        }
        
        if(!_parser->readChar(';')) {
          throw ParseException("Semicolon expected", _parser->getCodeLocation());
        }
      } else {
        kf_octet_t buffer[6];
        wchar_t ch = 0;
        unsigned short int s = _parser->readAny(ch, buffer);
        output.append((char*)buffer, s);
      }
    } // white()
  }
  
  
  Ptr<XmlEndElement> XmlObjectStreamReader::readCloseTag() {
    CodeLocation begin = _parser->getCodeLocation();
    if(!_parser->readSequence(L"</")) {
      return Ptr<XmlEndElement>();
    }
    
    _parser->skipSpaces();
    
    string name;
    _parser->readIdentifier(name);
    
    _parser->skipSpaces();
    
    if(!_parser->readChar('>')) {
      throw ParseException("'>' expected", _parser->getCodeLocation());
    }
    
    CodeLocation end = _parser->getCodeLocation();
    
    if(name != *(_elementStack.end() - 1)) {
      throw ParseException("Closing tag \"" + name + "\" does not match its"
          " corresponding opening tag " + *(_elementStack.end()),
          CodeRange(begin, end));
    }
    
    _elementStack.pop_back();
    
    _state = ELEMENT_END;
    
    return new XmlEndElement(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), name);
  }
  
  
  Ptr<Token> XmlObjectStreamReader::readEndElementOrEndCollection()
  throw(ParseException)
  {
    Ptr<XmlEndElement> endElement = readCloseTag();
    
    if(endElement.isNull()) {
      return Ptr<Token>();
    }
    
    if(endElement->getClassName() == ObjectSerializer::COLLECTION_CLASS_NAME) {
      Ptr<XmlEndCollection> endCollection(
          new XmlEndCollection(getPtr().AS(XmlObjectStreamReader), endElement->codeRange));
      return endCollection.AS(Token).retain();
    }
    
    return endElement.AS(Token).retain();
  }
  
  
  /**
   * Returns the next token in the stream.
   */
  
  Ptr<Token> XmlObjectStreamReader::next() throw(ParseException) {
    _parser->skipSpacesAndNewLines();
    
    if(_parser->testEndOfStream()) {
      return Token::END_STREAM_TOKEN;
    }
    
    switch (_state) {
      case INITIAL:
        return readElement().AS(Token).retain();
      
      case ELEMENT_HEAD: {
        if(!_nextAttrib.isNull()) {
          Ptr<Token> t = _nextAttrib.AS(Token);
          _nextAttrib = NULL;
          return t.retain();
        }
        
        _parser->skipSpacesAndNewLines();
        
        Ptr<XmlAttribute> attr = readAttribute();
        if(!attr.isNull()) {
          return attr.AS(Token).retain();
        }
        
        if(_parser->testSequence(L"/>")) {
          CodeLocation begin = _parser->getCodeLocation();
          _parser->readSequence(L"/>");
          CodeLocation end = _parser->getCodeLocation();
          string name = *(_elementStack.end() - 1);
          _elementStack.pop_back();
          _state = ELEMENT_END;
          
          if(name == ObjectSerializer::COLLECTION_CLASS_NAME) {
            return new XmlEndCollection(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end));
          }
          
          return new XmlEndElement(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), name);
        }
        
        if(!_parser->readChar('>')) {
          throw ParseException("'>' expected", _parser->getCodeLocation());
        }
        
        _parser->skipSpacesAndNewLines();
        
        if(_parser->readSequence(L"<!CDATA[[")) {
          CodeLocation begin = _parser->getCodeLocation();
          string cdata;
          _parser->readAllBeforeSequence(L"]]>", cdata);
          CodeLocation end = _parser->getCodeLocation();
          if(!_parser->readSequence(L"]]>")) {
            throw ParseException("CDATA section is not properly closed with \"]]>\"", begin);
          }
          _state = TEXT;
          return new XmlText(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), cdata);
        }
        
        Ptr<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag.retain();
        }
        
        Ptr<Token> child = readElementOrCollection();
        if(!child.isNull()) {
          return child.retain();
        }
        
        CodeLocation begin = _parser->getCodeLocation();
        string text;
        _parser->readAllBeforeChar('<', text);
        if(text.length() == 0) {
          throw ParseException("Error reading element body", begin);
        }
        _state = TEXT;
        CodeLocation end = _parser->getCodeLocation();
        return new XmlText(getPtr().AS(XmlObjectStreamReader), CodeRange(begin, end), text);
      }
        
      case ELEMENT_END: {
        _parser->skipSpacesAndNewLines();
        if(_parser->testEndOfStream()) {
          if(_elementStack.size() > 0) {
            string str;
            for(int i = 0; i < _elementStack.size(); i++) {
              if(i > 0) {
                str += ", ";
              }
              str += _elementStack[i];
            }
            throw ParseException("The following elements are not closed: " + str);
          }
          _state = END;
          return Token::END_STREAM_TOKEN;
        }
        
        Ptr<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag.retain();
        }
        
        Ptr<Token> nextElement = readElementOrCollection();
        if(!nextElement.isNull()) {
          return nextElement.retain();
        }
        
        throw ParseException("Open or close tag expected", _parser->getCodeLocation());
      }
        
      case TEXT: {
        Ptr<Token> closeTag = readEndElementOrEndCollection();
        if(!closeTag.isNull()) {
          return closeTag.retain();
        }
        
        Ptr<Token> nextElement = readElementOrCollection();
        if(!nextElement.isNull()) {
          return nextElement.retain();
        }
        
        throw ParseException("Open or close tag expected", _parser->getCodeLocation());
      }
        
      case END:
        throw ParseException("Parse is already finished");
        
    } // case(_state)
    
    throw ParseException("Invalid XML reader state");
    
  } // next()
  
} // namespace kfoundation
