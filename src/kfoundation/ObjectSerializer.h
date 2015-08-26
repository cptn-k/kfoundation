#ifndef ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H
#define ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H

// Std
#include <string>
#include <ostream>
#include <vector>

// Internal
#include "ManagedObject.h"
#include "KFException.h"
#include "PtrDecl.h"
#include "Bool.h"
#include "UniChar.h"
#include "Int.h"
#include "LongInt.h"
#include "Double.h"

using namespace std;

namespace kfoundation {
  
  class UniString;
  
  
  /**
   * Thrown when ObjectSerializer is used in an invalid way.
   *
   * @ingroup io
   * @ingroup exceptions
   * @headerfile ObjectSerializer.h <kfoundation/ObjectSerializer.h>
   */
  
  class ObjectDumpBuilderException : public KFException {
  public:
    ObjectDumpBuilderException(string message);
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
   * or object(const PPtr<T>) it is not needed to call endObject() because it
   * is already called in the serializer() method of the given argument.
   *
   * All of these methods can be used chained sytax. Example:
   *
   *     void serialize(PPtr<ObjectSerializer> os) const {
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

  class ObjectSerializer : public ManagedObject {
  public:
    
    /**
     * Output format.
     */
    
    typedef enum {
      DUMP, ///< KFOR (KFoundation Format)
      XML,  ///< XML
      JSON  ///< JSON
    } output_type_t;

  private:
    typedef enum {
      ROOT,
      MEMBER,
      OBJECT,
      OBJECT_BODY,
      PROPERTY,
      VALUE,
      COLLECTION
    } state_t;

    typedef enum {
      NUMBER,
      CHAR,
      STRING,
      BOOL,
      NONE
    } value_type_t;

    class StackItem {
    public:
      state_t _state;
      string  _name;
      bool    _isLead;
      StackItem(state_t state, string name, bool isLead)
        : _state(state), _name(name), _isLead(isLead)
      { 
        // Nothing
      }
    };

    output_type_t _outputType;
    state_t       _state;
    ostream&      _stream;
    bool          _isLead;
    int           _indentUnit;
    int           _indent;
    string        _name;
    vector<StackItem> _stack;

    Ptr<ObjectSerializer> attribute(const string& name, const string& value,
        value_type_t type);
    
    string stackToString();
    string stateToString(state_t state);
    void   printIndent();

  public:
    static const string ID_ATTRIB_NAME;
    static const string COLLECTION_CLASS_NAME;
    
    ObjectSerializer(ostream& stream, output_type_t outputType, int indentUnit);
    ObjectSerializer(ostream& stream, output_type_t outputType);
    
    PPtr<ObjectSerializer> member(const string& name);
    PPtr<ObjectSerializer> object(const string& className);
    PPtr<ObjectSerializer> object(const SerializingStreamer& ref);
    
    template<typename T>
    PPtr<ObjectSerializer> object(const PPtr<T> ptr);
        
    PPtr<ObjectSerializer> endObject();
    PPtr<ObjectSerializer> text(const string& value);
    PPtr<ObjectSerializer> null();

    PPtr<ObjectSerializer> attribute(const string& name, const string& value);
    PPtr<ObjectSerializer> attribute(const string& name, char value);
    PPtr<ObjectSerializer> attribute(const string& name, int value);
    PPtr<ObjectSerializer> attribute(const string& name, unsigned int value);
    PPtr<ObjectSerializer> attribute(const string& name, long int value);
    PPtr<ObjectSerializer> attribute(const string& name, unsigned long int value);
    PPtr<ObjectSerializer> attribute(const string& name, double value);
    PPtr<ObjectSerializer> attribute(const string& name, bool value);
    PPtr<ObjectSerializer> attribute(const string& name);

    PPtr<ObjectSerializer> collection();
    PPtr<ObjectSerializer> endCollection();
  };

  
  template<typename T>
  PPtr<ObjectSerializer> ObjectSerializer::object(const PPtr<T> obj) {
    if(obj.template ISA(SerializingStreamer)) {
      ((SerializingStreamer*)obj.toPurePtr())->serialize(getPtr().AS(ObjectSerializer));
    } else if(obj.template ISA(Streamer)) {
      this->text(((Streamer*)obj.toPurePtr())->toString());
    } else {
      this->null();
    }
    
    return getPtr().AS(ObjectSerializer);
  } // object(const Ptr<T>&)
  
  
} // namespace kfoundation

#endif
