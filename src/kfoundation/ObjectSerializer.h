#ifndef ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H
#define ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H

// Internal
#include "definitions.h"
#include "KFObject.h"
#include "KFException.h"
#include "RefDecl.h"
#include "ArrayDecl.h"

namespace kfoundation {
  
  class UString;
  class OutputStream;
  class PrintWriter;
  class UChar;

  /**
   * Thrown when ObjectSerializer is used in an invalid way.
   *
   * @ingroup io
   * @ingroup exceptions
   * @headerfile ObjectSerializer.h <kfoundation/ObjectSerializer.h>
   */
  
  class ObjectDumpBuilderException : public KFException {
    public: ObjectDumpBuilderException(RefConst<UString> message);
  };
  
  
  /**
   * Provides APIs to serialize an object. This is usually used in conjuction
   * with SerializingStreamer. 
   *
   * Supported output formats are KFOR (KFoundation Format), XML, and JSON.
   * To use, the methods shoud be called in particular order. If the order
   * is not observed, an expcetion will be thrown.
   *
   * * A call to object(const string&) should be made first.
   * * object() can be followed by attribute(), member(), or endObject().
   * * attribute() can be followed by attribute(), member() or endObject().
   * * member() can be followed by object(), collection(), or null().
   * * There shold be an endObjectr() corresponding to each object() and
   *   endCollection() corresponding to each collection().
   *
   * An expception is when using object(const SerializingStreamer&) 
   * or object(const Ref<T>) it is not needed to call endObject() because it
   * is already called in the serializer() method of the given argument.
   *
   * All of these methods can be used chained sytax. Example:
   *
   *     void serialize(Ref<ObjectSerializer> os) const {
   *         os->object("MyClass")
   *           ->attribute("counter", _counter)
   *           ->attribute("name", _name)
   *           ->member("innerObject")->object(innerObject)
   *           ->endObject();
   *     }
   *
   * @ingroup serialization
   * @headerfile ObjectSerializer.h <kfoundation/ObjectSerializer.h>
   */

  class ObjectSerializer : public KFObject {

  // --- NESTED TYPES --- //

    private: typedef enum {
      ROOT,
      MEMBER,
      OBJECT,
      OBJECT_BODY,
      PROPERTY,
      VALUE,
      COLLECTION
    } state_t;


    public: typedef enum {
      NUMBER,
      CHAR,
      STRING,
      BOOL,
      NONE
    } value_type_t;


    private: class StackItem {
      public: state_t _state;
      public: bool    _isLead;
      public: RefConst<UString> _name;

      public: StackItem()
      : _state(ROOT),
        _isLead(false),
        _name()
      {
        // Nothing;
      }

      public: void set(state_t state, RefConst<UString> name, bool isLead);
    };


  // --- STATIC FIELDS --- //

    private: const static StaticRefConst<UChar> SPACE;


  // --- FIELDS --- //

    private: state_t    _state;
    private: bool       _isLead;
    private: kf_int8_t  _indentUnit;
    private: kf_int16_t _indent;
    private: kf_int32_t _index;
    private: RefConst<UString> _name;
    private: Ref<PrintWriter> _writer;
    private: Ref< Array<StackItem> > _stack;


  // --- CONSTRUCTORS --- //

    public: ObjectSerializer(Ref<OutputStream> stream, kf_int8_t indentUnit = 4);


  // --- ABSTRACT METHODS --- //

    protected: virtual void printHeader() = 0;

    protected: virtual void printAttribute(RefConst<UString> name,
        const Streamer& value, value_type_t valueType, bool isLead) = 0;

    protected: virtual void printObjectBegin(RefConst<UString> className,
        RefConst<UString> name, bool isLead) = 0;

    protected: virtual void printObjectEnd(RefConst<UString> className,
         bool isLead) = 0;
    
    protected: virtual void printNull(RefConst<UString> name, bool isLead) = 0;

    protected: virtual void printCollectionBegin(RefConst<UString> name,
        bool isLead) = 0;

    protected: virtual void printCollectionEnd(bool isLead) = 0;


  // --- METHODS --- //

    private: Ref<ObjectSerializer> attribute(RefConst<UString> name,
        const Streamer& value, value_type_t type);
    
    private: RefConst<UString> stackToString() const;
    private: RefConst<UString> stateToString(state_t state) const;

    protected: void printIndent(bool newLine = true);
    protected: PrintWriter& getWriter();

    public: Ref<ObjectSerializer> member(RefConst<UString> name);
    public: Ref<ObjectSerializer> object(RefConst<UString> className);
    public: Ref<ObjectSerializer> object(const SerializingStreamer& obj);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const wchar_t value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const kf_int32_t value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const kf_int64_t value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const double value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const bool value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, const Streamer& value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name, RefConst<UString> value);
    public: Ref<ObjectSerializer> attribute(RefConst<UString> name);
    public: Ref<ObjectSerializer> null();
    public: Ref<ObjectSerializer> endObject();
    public: Ref<ObjectSerializer> collection();
    public: Ref<ObjectSerializer> endCollection();

    public: template<typename T>
    Ref<ObjectSerializer> object(RefConst<T> ptr);

  };


  template<typename T>
  Ref<ObjectSerializer> ObjectSerializer::object(RefConst<T> obj) {
    if(obj.isNull()) {
      null();
    } else {
      obj->serialize(this);
    }
    return this;
  }

} // namespace kfoundation

#endif