//
//  ObjectStreamReader.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KFOUNDATION_OBJECTSTREAM_READER
#define KFOUNDATION_OBJECTSTREAM_READER

#include "ManagedObject.h"
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
  
  class ObjectStreamReader : public ManagedObject {
    public: virtual Ptr<Token> next() throw(ParseException) = 0;
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
   *      void deserialize(PPtr<ObjectToken> headToken) {
   *          headToken->validateClass("MyClass");
   *          Ptr<Token> token = headToken->next();
   *          token->validateType(Token::ATTRIBUTE);
   *          _name = token.AS(AttributeToken)->validateName("name")->getValue();
   *          token->next()->validateType(END_OBJECT);
   *      }
   *
   * Conditional statements can be added if desired:
   *
   *      void deserialize(PPtr<ObjectToken> headToken) {
   *          headToken->validateClass("MyClass");
   *          Ptr<Token> token = headToken->next();
   *          if(token.is(Token::ATTRIBUTE)) {
   *              PPtr<AttributeToken> attrib = token.AS(Attribute);
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
  public:
    typedef enum {
      OBJECT,
      ATTRIBUTE,
      TEXT,
      COLLECTION,
      END_COLLECTION,
      END_OBJECT,
      END_STREAM
    } type_t;
    
    static const SPtr<Token> END_STREAM_TOKEN;
    static string toString(const type_t& t);
    const CodeRange codeRange; ///< CodeRange marking begining and end of this
                               ///  token.
    
  public:
    Token(const CodeRange& cr);
    virtual type_t getType() const = 0;
    bool is(const type_t& t) const;
    void validateType(const type_t& t) const;
    
    PPtr<ObjectToken> asObject();
    PPtr<EndObjectToken> asEndObject();
    PPtr<AttributeToken> asAttribute();
    PPtr<CollectionToken> asCollection();
    PPtr<EndCollectionToken> asEndCollection();
    
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
  public:
    static const type_t TYPE;
    
    ObjectToken(const CodeRange& range);
    
    bool checkClass(const string& name) const;
    void validateClass(const string& name) const;
    void throwMissingAttribute(const string& name) const;
    void throwInvlaidClass() const;
    
    virtual const string& getClassName() const = 0;
    virtual const string& getIdentifier() const = 0;
    virtual bool hasIdentifier() const = 0;
    
    // From ObjectStreamToken
    inline type_t getType() const {
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
  public:
    static const type_t TYPE;
    
    EndObjectToken(const CodeRange& range);
    
    virtual const string& getClassName() const = 0;
    bool checkClass(const string& name) const;
    void validateClass(const string& name) const;
    
    // From ObjectStreamToken
    inline type_t getType() const {
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
  public:
    static const type_t TYPE;
    
    AttributeToken(const CodeRange& range);
    
    bool checkName(const string& name) const;
    PPtr<AttributeToken> validateName(const string& name) const;
    void throwInvliadName() const;
    
    virtual const string& getName() const = 0;
    virtual const string& getValue() const = 0;
    virtual bool isEmpty() const = 0;
    
    // From ObjectStreamToken
    inline type_t getType() const {
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
  public:
    static const type_t TYPE;
    
    TextToken(const CodeRange& range);
    
    virtual const string& get() const = 0;
    
    // From ObjectStreamToken
    inline type_t getType() const {
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
  public:
    static type_t TYPE;
    
    CollectionToken(const CodeRange& range);
    
    virtual const string& getIdentifier() const = 0;
    virtual bool hasIdentifier() const = 0;
    
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
  public:
    static type_t TYPE;
    
    EndCollectionToken(const CodeRange& range);
    
    // From ObjectStreamToken
    inline type_t getType() const {
      return END_COLLECTION;
    }
  };
  
  
//\/ END /\////////////////////////////////////////////////////////////////////
  
} // namespace kfoundation


#endif /* defined(KFOUNDATION_OBJECTSTREAM_READER) */
