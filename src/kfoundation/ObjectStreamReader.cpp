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
  
  /**
   * @fn kfoundation::ObjectStreamReader::next()
   * Returns the next token in the stream (not the token after this one).
   */

  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////

  
  /**
   * Returns a string corresponding the given parameter.
   */
  
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
  
  
  /**
   * Constructor.
   *
   * @param cr The range marking the begining and the end of input containing
   *           this token.
   */
  
  Token::Token(const CodeRange& cr)
  : codeRange(cr)
  {
    // Nothing;
  }
  
  
  /**
   * Checks if the type of this token matches the given argument.
   */
  
  bool Token::is(const type_t& t) const {
    return getType() == t;
  }
  
  
  /**
   * Checks of the type of this token matches the given argument, and if
   * not, produces a ParseException explaning the problem.
   */
  
  void Token::validateType(const type_t& t) const {
    if(t != getType()) {
      throw ParseException("Expected token of type " + toString(t) + " but "
          + toString(getType()) + " found.", codeRange);
    }
  }
  
  
  /**
   * Validates if this token represents an object. If so casts itself into
   * ObjectToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into ObjectToken.
   * @throw ParseException if the type of this token is not ObjectToken.
   */
  
  PPtr<ObjectToken> Token::asObject() {
    validateType(OBJECT);
    return getPtr().AS(ObjectToken);
  }
  
  
  /**
   * Validates if this token represents end of an object. If so casts itself
   * into EndObjectToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into EndObjectToken.
   * @throw ParseException if the type of this token is not EndObjectToken.
   */

  PPtr<EndObjectToken> Token::asEndObject() {
    validateType(END_OBJECT);
    return getPtr().AS(EndObjectToken);
  }

  
  /**
   * Validates if this token represents an attribute. If so casts itself
   * into AttributeToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into AttributeToken.
   * @throw ParseException if the type of this token is not AttributeToken.
   */
  
  PPtr<AttributeToken> Token::asAttribute() {
    validateType(ATTRIBUTE);
    return getPtr().AS(AttributeToken);
  }
  
  
  /**
   * Validates if this token represents begining of a collection. If so casts
   * itself into CollectionToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into CollectionToken.
   * @throw ParseException if the type of this token is not CollectionToken.
   */

  PPtr<CollectionToken> Token::asCollection() {
    validateType(COLLECTION);
    return getPtr().AS(CollectionToken);
  }
  
  
  /**
   * Validates if this token represents end of a collection. If so casts
   * itself into EndCollectionToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into EndCollectionToken.
   * @throw ParseException if the type of this token is not EndCollectionToken.
   */
  
  PPtr<EndCollectionToken> Token::asEndCollection() {
    validateType(END_COLLECTION);
    return getPtr().AS(EndCollectionToken);
  }
  
  
  /**
   * @fn kfoundation::Token::getType()
   * Returns the type of this token.
   */
  
    
//\/ ObjectToken /\////////////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::OBJECT.
   */
  
  const Token::type_t ObjectToken::TYPE = Token::OBJECT;
  
  
  /**
   * Constructor.
   */
  
  ObjectToken::ObjectToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
  /**
   * Checks if the class name for the parsed object equals the given argument.
   */
  
  bool ObjectToken::checkClass(const string& name) const {
    return getClassName() == name;
  }
  
  
  /**
   * Checks if the class name for the parsed object equals the given argument,
   * and if not will throw an appropriate ParseException.
   *
   * @param name The name to check against.
   */
  
  void ObjectToken::validateClass(const string& name) const {
    if(getClassName() != name) {
      throw ParseException("Excepted class name \"" + name + "\" but \""
          + getClassName() + "\" found.", codeRange);
    }
  }
  
  
  /**
   * Throws an exception explaining an attribute with the given name is missing.
   */

  void ObjectToken::throwMissingAttribute(const string& name) const {
    throw ParseException("Missing required attribute \"" + name + "\"",
        codeRange);
  }
  
  
  /**
   * Throws a ParseException indicating the class name is invalid.
   */
  
  void ObjectToken::throwInvlaidClass() const {
    throw ParseException("Invalid class name \"" + getClassName() + "\"",
        codeRange);
  }
  
  
//\/ EndObjectToken /\/////////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::END_OBJECT.
   */
  
  const Token::type_t EndObjectToken::TYPE = Token::END_OBJECT;

  
  /**
   * Constructor.
   */
  
  EndObjectToken::EndObjectToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
  /**
   * Checks if this token represents the end of object with the given name.
   */
  
  bool EndObjectToken::checkClass(const string& name) const {
    return getClassName() == name;
  }

  
  /**
   * Checks if this token represents the end of object with the given name, if
   * not throws a ParseException.
   */
  
  void EndObjectToken::validateClass(const string& name) const {
    if(getClassName() != name) {
      throw ParseException("Excepted class name \"" + name + "\" but \""
          + getClassName() + "\" found.", codeRange);
    }
  }
  
  
//\/ AttributeToken /\/////////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::ATTRIBUTE.
   */
  
  const Token::type_t AttributeToken::TYPE = Token::ATTRIBUTE;
  
  
  /**
   * Constructor.
   */
  
  AttributeToken::AttributeToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
  /**
   * Checks if the name of this attribute equals the given parameter.
   */
  
  bool AttributeToken::checkName(const string& name) const {
    return name == getName();
  }
  
  
  /**
   * Checks if the name of this attribute equals the given parameter, if not
   * throws a ParseException.
   */
  
  PPtr<AttributeToken> AttributeToken::validateName(const string& name) const {
    if(getName() != name) {
      throw ParseException("Expected attribtue name \"" + name + "\" but \""
          + getName() + "\" found.", codeRange);
    }
    return getPtr().AS(AttributeToken);
  }
  
  
  /**
   * Throws a ParseException explanaining the name of this attribute is invalid.
   */
  
  void AttributeToken::throwInvliadName() const {
    throw ParseException("Invalid attribute name \"" + getName() + "\""
        , codeRange);
  }
  
  
//\/ TextToken /\//////////////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::TEXT.
   */
  
  const Token::type_t TextToken::TYPE = Token::TEXT;
  
  
  /**
   * Constructor.
   */
  
  TextToken::TextToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }

  
//\/ CollectionToken /\////////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::COLLECTION.
   */

  Token::type_t CollectionToken::TYPE = Token::COLLECTION;
  
  
  /**
   * Constructor.
   */
  
  CollectionToken::CollectionToken(const CodeRange& range)
  : Token(range)
  {
    // Nothing;
  }
  
  
//\/ EndCollectionToken /\/////////////////////////////////////////////////////
  
  /**
   * Type this token, that is Token::TEXT.
   */
  
  Token::type_t EndCollectionToken::TYPE = Token::END_COLLECTION;
  
  
  /**
   * Constructor.
   */
  
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
  
  /**
   * End stream token.
   */
  
  const SPtr<Token> Token::END_STREAM_TOKEN = new EndStreamItem();
  
  
//\/ END /\////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation