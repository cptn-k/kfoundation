//
//  ObjectStreamReader.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "ObjectStreamReader.h"
#include "Ptr.h"

namespace kfoundation {
  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////
  
  string Token::toString(const type_t &t) {
    switch (t) {
      case OBJECT:
        return "object";
        
      case ATTRIBUTE:
        return "attribute";
        
      case TEXT:
        return "text";
        
      case COLLECTION:
        return "collection";
        
      case END_COLLECTION:
        return "end collection";
        
      case END_OBJECT:
        return "end object";
        
      case END_STREAM:
        return "end stream";
    }
    
    return "unknown";
  }
  
  
  Token::Token(const CodeRange& cr)
  : codeRange(cr)
  {
    // Nothing;
  }
  
  
  bool Token::is(const type_t& t) const {
    return getType() == t;
  }
  
  
  void Token::validateType(const type_t& t) const {
    if(t != getType()) {
      throw ParseException("Expected token of type " + toString(t) + " but "
          + toString(getType()) + " found.", codeRange);
    }
  }
  
  
  PPtr<ObjectToken> Token::asObject() {
    validateType(OBJECT);
    return getPtr().AS(ObjectToken);
  }
  
  
  PPtr<EndObjectToken> Token::asEndObject() {
    validateType(END_OBJECT);
    return getPtr().AS(EndObjectToken);
  }

  
  PPtr<AttributeToken> Token::asAttribute() {
    validateType(ATTRIBUTE);
    return getPtr().AS(AttributeToken);
  }
  
  
  PPtr<CollectionToken> Token::asCollection() {
    validateType(COLLECTION);
    return getPtr().AS(CollectionToken);
  }
  
  
  PPtr<EndCollectionToken> Token::asEndCollection() {
    validateType(END_COLLECTION);
    return getPtr().AS(EndCollectionToken);
  }
  
    
//\/ ObjectToken /\////////////////////////////////////////////////////////////
  
  const Token::type_t ObjectToken::TYPE = Token::OBJECT;
  
  ObjectToken::ObjectToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  bool ObjectToken::checkClass(const string& name) const {
    return getClassName() == name;
  }
  
  void ObjectToken::validateClass(const string& name) const {
    if(getClassName() != name) {
      throw ParseException("Excepted class name \"" + name + "\" but \""
          + getClassName() + "\" found.", codeRange);
    }
  }

  void ObjectToken::throwMissingAttribute(const string& name) const {
    throw ParseException("Missing required attribute \"" + name + "\"",
        codeRange);
  }
  
  void ObjectToken::throwInvlaidClass() const {
    throw ParseException("Invalid class name \"" + getClassName() + "\"",
        codeRange);
  }
  
  
//\/ EndObjectToken /\/////////////////////////////////////////////////////////
  
  const Token::type_t EndObjectToken::TYPE = Token::END_OBJECT;

  EndObjectToken::EndObjectToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  bool EndObjectToken::checkClass(const string& name) const {
    return getClassName() == name;
  }
  
  void EndObjectToken::validateClass(const string& name) const {
    if(getClassName() != name) {
      throw ParseException("Excepted class name \"" + name + "\" but \""
          + getClassName() + "\" found.", codeRange);
    }
  }
  
  
//\/ AttributeToken /\/////////////////////////////////////////////////////////
  
  const Token::type_t AttributeToken::TYPE = Token::ATTRIBUTE;
  
  AttributeToken::AttributeToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  bool AttributeToken::checkName(const string& name) const {
    return name == getName();
  }
  
  PPtr<AttributeToken> AttributeToken::validateName(const string& name) const {
    if(getName() != name) {
      throw ParseException("Expected attribtue name \"" + name + "\" but \""
          + getName() + "\" found.", codeRange);
    }
    return getPtr().AS(AttributeToken);
  }
  
  void AttributeToken::throwInvliadName() const {
    throw ParseException("Invalid attribute name \"" + getName() + "\""
        , codeRange);
  }
  
  
//\/ TextToken /\//////////////////////////////////////////////////////////////
  
  const Token::type_t TextToken::TYPE = Token::TEXT;
  
  TextToken::TextToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }

  
//\/ CollectionToken /\////////////////////////////////////////////////////////
  
  Token::type_t CollectionToken::TYPE = Token::COLLECTION;
  
  CollectionToken::CollectionToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
//\/ EndCollectionToken /\/////////////////////////////////////////////////////
  
  Token::type_t EndCollectionToken::TYPE = Token::END_COLLECTION;
  
  EndCollectionToken::EndCollectionToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
//\/ EndStreamItem /\//////////////////////////////////////////////////////////
  
  class EndStreamItem : public Token {
  public:
    EndStreamItem();
    
    type_t getType() const;
    
    Ptr<Token> next() throw(ParseException);
  };
  
  EndStreamItem::EndStreamItem()
  : Token(CodeRange(CodeLocation(), CodeLocation()))
  {
    // Nothing;
  }
  
  Token::type_t EndStreamItem::getType() const {
    return END_STREAM;
  }
  
  Ptr<Token> EndStreamItem::next() throw(ParseException) {
    throw ParseException("Aleady reached the end of stream.");
  }
  
  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////
  
  const SPtr<Token> Token::END_STREAM_TOKEN = new EndStreamItem();
  
  
//\/ END /\////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation