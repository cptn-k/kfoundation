//
//  ObjectStreamReader.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "ObjectStreamReader.h"
#include "Ref.h"
#include "Int32.h"
#include "Int64.h"
#include "Double.h"
#include "Bool.h"

namespace kfoundation {
  
  /**
   * @fn kfoundation::ObjectStreamReader::next()
   * Returns the next token in the stream (not the token after this one).
   */

  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////

  
  /**
   * Returns a string corresponding the given parameter.
   */
  
  RefConst<UString> Token::toString(const type_t t) {
    switch (t) {
      case OBJECT:
        return K"object";
        
      case ATTRIBUTE:
        return K"attribute";
        
      case TEXT:
        return K"text";
        
      case COLLECTION:
        return K"collection";
        
      case END_COLLECTION:
        return K"end collection";
        
      case END_OBJECT:
        return K"end object";
        
      case END_STREAM:
        return K"end stream";
    }
    
    return K"unknown";
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
      throw ParseException(K"Expected token of type \"" + toString(t)
          + "\" but \"" + toString(getType()) + "\" found.", codeRange);
    }
  }
  
  
  /**
   * Validates if this token represents an object. If so casts itself into
   * ObjectToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into ObjectToken.
   * @throw ParseException if the type of this token is not ObjectToken.
   */
  
  Ref<ObjectToken> Token::asObject() {
    validateType(OBJECT);
    return Ref<Token>(getRef()).AS(ObjectToken);
  }
  
  
  /**
   * Validates if this token represents end of an object. If so casts itself
   * into EndObjectToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into EndObjectToken.
   * @throw ParseException if the type of this token is not EndObjectToken.
   */

  Ref<EndObjectToken> Token::asEndObject() {
    validateType(END_OBJECT);
    return Ref<Token>(getRef()).AS(EndObjectToken);
  }

  
  /**
   * Validates if this token represents an attribute. If so casts itself
   * into AttributeToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into AttributeToken.
   * @throw ParseException if the type of this token is not AttributeToken.
   */
  
  Ref<AttributeToken> Token::asAttribute() {
    validateType(ATTRIBUTE);
    return Ref<Token>(getRef()).AS(AttributeToken);
  }
  
  
  /**
   * Validates if this token represents begining of a collection. If so casts
   * itself into CollectionToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into CollectionToken.
   * @throw ParseException if the type of this token is not CollectionToken.
   */

  Ref<CollectionToken> Token::asCollection() {
    validateType(COLLECTION);
    return Ref<Token>(getRef()).AS(CollectionToken);
  }
  
  
  /**
   * Validates if this token represents end of a collection. If so casts
   * itself into EndCollectionToken, otherwise throws a ParseException.
   *
   * @return Pointer to this object casted into EndCollectionToken.
   * @throw ParseException if the type of this token is not EndCollectionToken.
   */
  
  Ref<EndCollectionToken> Token::asEndCollection() {
    validateType(END_COLLECTION);
    return Ref<Token>(getRef()).AS(EndCollectionToken);
  }

    
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
  
  bool ObjectToken::checkClass(RefConst<UString> name) const {
    return getClassName()->equals(name);
  }
  
  
  /**
   * Checks if the class name for the parsed object equals the given argument,
   * and if not will throw an appropriate ParseException.
   *
   * @param name The name to check against.
   */
  
  void ObjectToken::validateClass(RefConst<UString> name) const {
    if(!getClassName()->equals(name)) {
      throw ParseException(K"Excepted class name \"" + name + "\" but \""
          + getClassName() + "\" found.", codeRange);
    }
  }
  
  
  /**
   * Throws an exception explaining an attribute with the given name is missing.
   */

  void ObjectToken::throwMissingAttribute(RefConst<UString> name) const {
    throw ParseException(K"Missing required attribute \"" + name + "\"",
        codeRange);
  }
  
  
  /**
   * Throws a ParseException indicating the class name is invalid.
   */
  
  void ObjectToken::throwInvlaidClass() const {
    throw ParseException(K"Invalid class name \"" + getClassName() + "\"",
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
  
  bool EndObjectToken::checkClass(RefConst<UString> name) const {
    return getClassName()->equals(name);
  }

  
  /**
   * Checks if this token represents the end of object with the given name, if
   * not throws a ParseException.
   */
  
  void EndObjectToken::validateClass(RefConst<UString> name) const {
    if(!getClassName()->equals(name)) {
      throw ParseException(K"Excepted class name \"" + name + "\" but \""
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
  
  bool AttributeToken::checkName(RefConst<UString> name) const {
    return name->equals(getName());
  }
  
  
  /**
   * Checks if the name of this attribute equals the given parameter, if not
   * throws a ParseException.
   */
  
  Ref<AttributeToken> AttributeToken::validateName(RefConst<UString> name)
  const
  {
    if(!getName()->equals(name)) {
      throw ParseException(K"Expected attribtue name \"" + name + "\" but \""
          + getName() + "\" found.", codeRange);
    }
    return this;
  }
  
  
  /**
   * Throws a ParseException explanaining the name of this attribute is invalid.
   */
  
  void AttributeToken::throwInvliadName() const {
    throw ParseException(K"Invalid attribute name \"" + getName() + "\""
        , codeRange);
  }


  bool AttributeToken::getBoolValue() const {
    if(getValue()->equals(Bool::TRUE_STR)) {
      return true;
    } else if(getValue()->equals(Bool::FALSE_STR)) {
      return false;
    }
    throw ParseException(K"Unable to parse value as bool \"" + getValue()
        + "\"", Token::codeRange);
  }


  kf_int32_t AttributeToken::getInt32Value() const {
    return Int32::parse(getValue());
  }


  kf_int64_t AttributeToken::getInt64Value() const {
    return Int64::parse(getValue());
  }


  double AttributeToken::getDoubleValue() const {
    return Double::parse(getValue());
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


//\/ EndStreamToken /\/////////////////////////////////////////////////////////

  EndStreamToken::EndStreamToken()
  : Token(CodeRange(CodeLocation(), CodeLocation()))
  {
    // Nothing;
  }

  Token::type_t EndStreamToken::getType() const {
    return END_STREAM;
  }

  Ref<Token> EndStreamToken::next() throw(ParseException) {
    throw ParseException(K"Aleady reached the end of stream.");
  }


  
//\/ END /\////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation