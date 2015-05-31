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
  
  class ObjectDumpBuilderException : public KFException {
  public:
    ObjectDumpBuilderException(string message);
  };

  class ObjectSerializer : public ManagedObject {
  public:
    typedef enum {
      DUMP,
      XML,
      JSON
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
