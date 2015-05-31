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
  
  class ObjectStreamReader : public ManagedObject {
    public: virtual Ptr<Token> next() throw(ParseException) = 0;
  };
  
//\/ ObjectStreamToken /\//////////////////////////////////////////////////////
  
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
    const CodeRange codeRange;
    
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
