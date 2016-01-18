//
//  ObjectStreamReader.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_OBJECTSTREAM_READER
#define KFOUNDATION_OBJECTSTREAM_READER

#include "definitions.h"
#include "KFObject.h"
#include "InputStream.h"
#include "ParseException.h"
#include "CodeRange.h"

namespace kfoundation {
  
  class Token;
  class UniString;
  class AttributeToken;
  class ObjectToken;
  class EndObjectToken;
  class CollectionToken;
  class EndCollectionToken;
  
  
  /**
   * Generic interface for utility object used to read objets from an stream
   * of a given format. No matter what the format of the stream is, it should
   * be organized as if produced by ObjectSerializer.
   * 
   * @see Token
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */
  
  class ObjectStreamReader : public KFObject {
    public: virtual Ref<Token> next() throw(ParseException) = 0;
  };
  
  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////

  /**
   * Represents a token in a stream. This is an abstract class. The actual
   * object might be of any of the following types:
   *
   * * ObjectToken
   * * AttributeToken
   * * EndObjectToken
   * * CollectionToken
   * * EndCollectionToken
   * * TextToken
   *
   * Use getType() method to determine the type and cast accordingly.
   * Most often this object is used in predictive parsing manner. For
   * example:
   *
   *      void deserialize(Ref<ObjectToken> headToken) {
   *          headToken->validateClass("MyClass");
   *          Ref<Token> token = headToken->next();
   *          token->validateType(Token::ATTRIBUTE);
   *          _name = token.AS(AttributeToken)->validateName("name")->getValue();
   *          token->next()->validateType(END_OBJECT);
   *      }
   *
   * Conditional statements can be added if desired:
   *
   *      void deserialize(Ref<ObjectToken> headToken) {
   *          headToken->validateClass("MyClass");
   *          Ref<Token> token = headToken->next();
   *          if(token.is(Token::ATTRIBUTE)) {
   *              Ref<AttributeToken> attrib = token.AS(Attribute);
   *              if(attrib->checkName("attrib1")) {
   *                  _attrib1 = attrib->getValue();
   *              } else if(attrib->checkName("attrib2") {
   *                  _attrib2 = attrib->getValue();
   *              } else {
   *                  attrib->throwInvalidName();
   *              }
   *          } else {
   *              token->validateType(Token::OBJECT);
   *              ...
   *          }
   *          token->next()->validateType(END_OBJECT);
   *      }
   *
   * validateXXX() methods cause an expection to be thrown if the given
   * argument does not match the current token. The exception message will
   * include a code location that helps the user to understand the problem.
   *
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   * @see ObjectDeserializer
   */
  
  class Token : public ObjectStreamReader {

  // --- NESTED TYPES --- //

    public: typedef enum {
      OBJECT,
      ATTRIBUTE,
      TEXT,
      COLLECTION,
      END_COLLECTION,
      END_OBJECT,
      END_STREAM
    } type_t;


  // --- CONSTRUCTOR --- //

    public: Token(const CodeRange& cr);


  // --- STATIC METHODS --- //

    public: static RefConst<UString> toString(const type_t t);

    /** CodeRange marking begining and end of this token */
    public: const CodeRange codeRange;


  // --- METHODS --- //

    public: virtual type_t getType() const = 0;
    public: bool is(const type_t& t) const;
    public: void validateType(const type_t& t) const;
    
    public: Ref<ObjectToken> asObject();
    public: Ref<EndObjectToken> asEndObject();
    public: Ref<AttributeToken> asAttribute();
    public: Ref<CollectionToken> asCollection();
    public: Ref<EndCollectionToken> asEndCollection();
    
  };

  
//\/ ObjectToken /\////////////////////////////////////////////////////////////

  /**
   * Represents begining of an object in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */
  
  class ObjectToken : public Token {

  // --- STATIC FIELDS --- //

    public: static const type_t TYPE;


  // --- CONSTRUCTORS --- //
    
    public: ObjectToken(const CodeRange& range);


  // --- ABSTRACT METHODS --- //

    public: virtual RefConst<UString> getClassName() const = 0;
    public: virtual RefConst<UString> getIdentifier() const = 0;
    public: virtual bool hasIdentifier() const = 0;


  // --- METHODS --- //
    
    public: bool checkClass(RefConst<UString> name) const;
    public: void validateClass(RefConst<UString> name) const;
    public: void throwMissingAttribute(RefConst<UString> name) const;
    public: void throwInvlaidClass() const;

    // From ObjectStreamToken
    public: inline type_t getType() const {
      return OBJECT;
    }

  };

  
//\/ EndObjectToken /\/////////////////////////////////////////////////////////
  
  /**
   * Represents end of an object in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */

  class EndObjectToken : public Token {

  // --- STATIC FIELDS --- //

    public: static const type_t TYPE;


  // --- CONSTRUCTORS --- //
    
    public: EndObjectToken(const CodeRange& range);


  // --- ABSTRACT METHODS --- //

    public: virtual RefConst<UString> getClassName() const = 0;


  // --- METHODS --- //
    
    public: bool checkClass(RefConst<UString> name) const;
    public: void validateClass(RefConst<UString> name) const;
    
    // From ObjectStreamToken
    public: inline type_t getType() const {
      return END_OBJECT;
    }

  };
  
  
//\/ AttributeToken /\/////////////////////////////////////////////////////////
  
  /**
   * Represents an attribute in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */
  
  class AttributeToken : public Token {

  // --- STATIC FIELDS --- //

    public: static const type_t TYPE;


  // --- CONSTRUCTORS --- //

    public: AttributeToken(const CodeRange& range);


  // --- ABSTRACT METHODS --- //

    public: virtual RefConst<UString> getName() const = 0;
    public: virtual RefConst<UString> getValue() const = 0;
    public: virtual bool isEmpty() const = 0;


  // --- METHODS --- //
    
    public: bool checkName(RefConst<UString> name) const;
    public: Ref<AttributeToken> validateName(RefConst<UString> name) const;
    public: void throwInvliadName() const;
    public: bool getBoolValue() const;
    public: kf_int32_t getInt32Value() const;
    public: kf_int64_t getInt64Value() const;
    public: double getDoubleValue() const;

    // From ObjectStreamToken
    public: inline type_t getType() const {
      return ATTRIBUTE;
    }

  };

  
//\/ TextToken /\//////////////////////////////////////////////////////////////

  /**
   * Represents a text body (CDATA) in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */

  class TextToken : public Token {

  // --- STATIC FIELDS --- //

    public: static const type_t TYPE;


  // --- CONSTRUCTORS --- //
    
    public: TextToken(const CodeRange& range);


  // --- METHODS --- //
    
    public: virtual RefConst<UString> get() const = 0;
    
    // From ObjectStreamToken
    public: inline type_t getType() const {
      return TEXT;
    }

  };

  
//\/ CollectionToken /\////////////////////////////////////////////////////////
  
  /**
   * Represents begining of a collection in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */
  
  class CollectionToken : public Token {

  // --- STATIC FIELDS --- //

    public: static type_t TYPE;


  // --- CONSTRUCTORS --- //
    
    public: CollectionToken(const CodeRange& range);


  // --- VIRTUAL METHODS --- //

    public: virtual RefConst<UString> getIdentifier() const = 0;
    public: virtual bool hasIdentifier() const = 0;


  // --- METHODS --- //
    
    // From ObjectStreamToken
    inline type_t getType() const {
      return COLLECTION;
    }

  };

  
//\/ EndCollectionToken /\/////////////////////////////////////////////////////
  
  /**
   * Represents end of a collection in the parsed stream.
   *
   * @see Token
   * @see ObjectStreamReader
   * @ingroup serialization
   * @headerfile ObjectStreamReader.h <kfoundation/ObjectStreamReader.h>
   */
  
  class EndCollectionToken : public Token {

  // --- STATIC FIELDS --- //

    public: static type_t TYPE;


  // --- CONSTRUCTORS --- //
    
    public: EndCollectionToken(const CodeRange& range);


  // --- METHODS --- //
    
    // From ObjectStreamToken
    inline type_t getType() const {
      return END_COLLECTION;
    }

  };


//\/ EndStreamToken /\/////////////////////////////////////////////////////////

  class EndStreamToken : public Token {

  // --- CONSTRUCTOR --- //
    public: EndStreamToken();


  // --- METHODS --- //

    // From Token
    type_t getType() const;
    Ref<Token> next() throw(ParseException);

  };

  
//\/ END /\////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation


#endif /* defined(KFOUNDATION_OBJECTSTREAM_READER) */