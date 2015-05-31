
// Std
#include <stack>

// Internal
#include "UniString.h"
#include "Ptr.h"

// Self
#include "ObjectSerializer.h"


namespace kfoundation {

  // --- ObjectDumpBuilderException -------------------------------------------
  
  ObjectDumpBuilderException::ObjectDumpBuilderException(string message)
    : KFException(message)
  {
    setName("ObjectSerializerException");
  }
  
  
  // --- ObjectSerializer (private) ------------------------------------------
  
  Ptr<ObjectSerializer> ObjectSerializer::attribute(const string& name,
      const string& value, value_type_t type)
  {
    if(_state != OBJECT) {
      throw ObjectDumpBuilderException("Attribute is only allowed inside an "
                                       "object, before decleration of any member. Path: "
                                       + stackToString());
    }
    
    switch(_outputType) {
      case DUMP:
        if(!_isLead)
          _stream << ", ";
        
        // printIndent();
        
        _stream << name;
        
        if(type != NONE)
          _stream << ": ";
        
        if(type == NUMBER || type == BOOL)
          _stream << value;
        else if(type == CHAR)
          _stream << '\'' << value << '\'';
        else if(type == STRING)
          _stream << '\"' << value << '\"';
        
        break;
        
      case XML:
        _stream << ' ' << name;
        
        if(type != NONE)
          _stream << "=\"" << value << '\"';
        
        break;
        
      case JSON:
        if(!_isLead)
          _stream << ", ";
        
        printIndent();
        
        _stream << '\"' << name << "\" : ";
        if(type == NONE)
          _stream << "null";
        else if(type == NUMBER || type == BOOL)
          _stream << value;
        else if(type == CHAR)
          _stream << '\'' << value << '\'';
        else if(type == STRING)
          _stream << '\"' << value << '\"';
        
        break;
    }
    
    _isLead = false;
    
    return getPtr().AS(ObjectSerializer);
  } // attribute(const string&, const string&, value_type_t)

  
  string ObjectSerializer::stateToString(state_t type) {
    switch(type) {
      case ROOT       : return "root";
      case MEMBER     : return "member";
      case OBJECT     : return "object";
      case OBJECT_BODY: return "object-body";
      case PROPERTY   : return "property";
      case VALUE      : return "value";
      case COLLECTION : return "collection";
    }
    return "unknown";
  } // stateToString(state_t)
  
  
  string ObjectSerializer::stackToString() {
    if(_stack.empty())
      return "/";
    
    string res;
    size_t s = _stack.size();
    for(int i = 0; i < s; i++)
    {
      StackItem it = _stack[i];
      res += it._name + " (" + stateToString(it._state) + ") --> ";
    }
    res += '(' + stateToString(_state) + ')';
    
    return res;
  } // stackToString()
  
  
  void ObjectSerializer::printIndent() {
    if(_indentUnit != -1) {
      _stream.put('\n');
      for(int i = 0; i < _indent * _indentUnit; i++) {
        _stream.put(' ');
      }
    }
  } // printIndent()
  

  // --- ObjectSerializer ----------------------------------------------------
  
  const string ObjectSerializer::ID_ATTRIB_NAME("_id");
  const string ObjectSerializer::COLLECTION_CLASS_NAME("_collection");
  
  ObjectSerializer::ObjectSerializer(ostream& stream,
                                       output_type_t outputType,
                                       int indentUnit)
    : _outputType(outputType),
      _state(ROOT),
      _stream(stream),
      _indentUnit(indentUnit),
      _indent(0)
  {
    if(outputType == XML) {
      stream << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    }
  }

  
  ObjectSerializer::ObjectSerializer(ostream& stream, output_type_t outputType)
    : _outputType(outputType),
      _state(ROOT),
      _stream(stream),
      _indentUnit(4),
      _indent(0)
  {
    if(outputType == XML) {
      stream << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    }
  }

  
  PPtr<ObjectSerializer> ObjectSerializer::member(const string& name) {
    if(_state != OBJECT && _state != OBJECT_BODY) {
      throw ObjectDumpBuilderException("Error adding member \"" + name
                + "\". Members are only allowed within objects. Path: " + 
                stackToString());
    }

    switch(_outputType) {
      case DUMP:
        if(!_isLead)
          _stream << ", ";
        printIndent();
        _stream << name << ": ";
        break;

      case XML:
        if(_state == OBJECT)
          _stream << '>';
        _name = name;
        break;

      case JSON:
        if(!_isLead)
          _stream << ", ";
        printIndent();
        _stream << '\"' << name << "\" : ";
        break;
    }

    _state = OBJECT_BODY;

    if(_isLead)
      _isLead = false;

    _stack.push_back(StackItem(_state, name, _isLead));

    _state = MEMBER;
    _isLead = true;

    return getPtr().AS(ObjectSerializer);
  } // member(const string&)

  
  PPtr<ObjectSerializer> ObjectSerializer::object(const string& className) {
    if(_state != ROOT && _state != MEMBER && _state != COLLECTION) {
      throw ObjectDumpBuilderException("Error adding object \'" + className 
                + "\". Adding object is allowed only at root, in colelction, "
                  "or after member. Path: " + stackToString());
    }

    switch(_outputType) {
      case DUMP:
        if(_state == COLLECTION) {
          if(!_isLead)
            _stream << ", ";
          printIndent();
        }
        _stream << className << '[';
        break;

      case XML:
        printIndent();
        _stream << '<' << className;
        
        if(_state == MEMBER)
          _stream << " _id=\"" << _name << '\"';
        
        break;

      case JSON:
        if(_state == COLLECTION) {
          if(!_isLead)
            _stream << ", ";
          printIndent();
        }
        if(_state == ROOT) {
          _stream << "\n";
        }
        _stream << '{';
        break;
    }

    if(_isLead) {
      _isLead = false;
    }

    _stack.push_back(StackItem(_state, className, _isLead));

    _state = OBJECT;
    _isLead = true;

    _indent++;
    
    return getPtr().AS(ObjectSerializer);
  } // object(const string&)
  
  
  PPtr<ObjectSerializer> ObjectSerializer::object(const SerializingStreamer& ref)
  {
    ref.serialize(getPtr().AS(ObjectSerializer));
    return getPtr().AS(ObjectSerializer);
  }
  
  
  PPtr<ObjectSerializer> ObjectSerializer::endObject() {
    if(_state != OBJECT && _state != OBJECT_BODY) {
      throw ObjectDumpBuilderException("Error attemp to end object"
                                       " doesn't match any prior object decleration. Path: "
                                       + stackToString());
    }
    
    StackItem item = _stack.back();
    _stack.pop_back();
    
    _indent--;
    
    switch(_outputType) {
      case DUMP:
        _stream << ']';
        break;
        
      case XML:
        if(_state == OBJECT)
          _stream << " />";
        else {
          printIndent();
          _stream << "</" << item._name << '>';
        }
        break;
        
      case JSON:
        if(!_isLead)
          printIndent();
        _stream << '}';
        break;
    }
    
    _state = item._state;
    _isLead = item._isLead;
    
    if(_state ==  MEMBER) {
      item = _stack.back();
      _stack.pop_back();
      _state = item._state;
      _isLead = item._isLead;
    }
    
//    if(_stack.empty())
//      _stream << '\n';
    
    return getPtr().AS(ObjectSerializer);
  } // endObject();
  
  PPtr<ObjectSerializer> ObjectSerializer::text(const string& value) {
    if(_state != OBJECT) {
      throw ObjectDumpBuilderException("String is only allowed immidiately after"
                                       " member declerations. Path: " + stackToString());
    }
    
    switch(_outputType) {
      case DUMP:
        attribute("_cdata", value);
        break;
        
      case XML:
        _stream << '>' << value;
        break;
        
      case JSON:
        attribute("_cdata", value);
        break;
    }
    
    endObject();
    
    return getPtr().AS(ObjectSerializer);
  } // text(const string&)
  

  PPtr<ObjectSerializer> ObjectSerializer::null() {
    if(_state != MEMBER) {
      throw ObjectDumpBuilderException("null is only allowed immidiately after"
                  " member declerations. Path: " + stackToString());
    }
    
    StackItem item = _stack.back();
    _stack.pop_back();

    switch(_outputType) {
      case DUMP:
        _stream << "null";
        break;

      case XML:
        printIndent();
        _stream << '<' << item._name << "/>";
        break;

      case JSON:
        _stream << "null";
        break;
    }

    _state = item._state;
    _isLead = item._isLead;

    return getPtr().AS(ObjectSerializer);
  } // null()

  
  PPtr<ObjectSerializer> ObjectSerializer::collection() {
    if(_state != ROOT && _state != MEMBER) {
      throw ObjectDumpBuilderException("Collection is only allowed as root or member."
                " Path: " + stackToString());
    }

    switch(_outputType) {
      case DUMP:
        if(!_isLead)
          _stream << ", ";
        _stream << "{";
        break;
      
      case XML:
        printIndent();
        _stream << '<' << COLLECTION_CLASS_NAME;
        if(_state == MEMBER) {
          _stream << " _id=\"" << _name << "\">";
        }
        break;

      case JSON:
        if(!_isLead)
          _stream << ", ";
        _stream << "[";
        break;
    }

    _isLead = false;

    _stack.push_back(StackItem(_state, "", _isLead));

    _isLead = true;
    _state = COLLECTION;

    _indent++;

    return getPtr().AS(ObjectSerializer);
  } // collection(const string&)

  PPtr<ObjectSerializer> ObjectSerializer::endCollection() {
    if(_state != COLLECTION) {
      throw ObjectDumpBuilderException("End collection doesn't match the opening"
                 " of any collection. Path: " + stackToString());
    }

    StackItem item = _stack.back();
    _stack.pop_back();

    _indent--;

    switch(_outputType) {
      case DUMP:
        _stream << '}';
        break;

      case XML:
        printIndent();
        _stream << "</" << COLLECTION_CLASS_NAME << '>';
        break;

      case JSON:
        printIndent();
        _stream << "]";
        break;
    }

    _state = item._state;
    _isLead = item._isLead;
    
    if(_state ==  MEMBER) {
      item = _stack.back();
      _stack.pop_back();
      _state = item._state;
      _isLead = item._isLead;
    }
    
    if(_stack.empty()) {
      _stream << '\n';
    }

    return getPtr().AS(ObjectSerializer);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, const string& value) {
    return attribute(name, value, STRING);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, char value) {
    return attribute(name, UniChar(value).toString(), CHAR);
  }
  
  PPtr<ObjectSerializer> ObjectSerializer::attribute(const string& name, int value) {
    return attribute(name, Int(value).toString(), NUMBER);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, unsigned int value) {
    return attribute(name, Int(value).toString(), NUMBER);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, long int value) {
    return attribute(name, LongInt::toString(value), NUMBER);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, unsigned long int value) {
    return attribute(name, LongInt::toString(value), NUMBER);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, double value) {
    return attribute(name, Double(value).toString(), NUMBER);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, bool value) {
    return attribute(name, Bool::toString(value), BOOL);
  }
  
  PPtr<ObjectSerializer>
  ObjectSerializer::attribute(const string& name) {
    return attribute(name, "", NONE);
  }
  
  
} // namespace kfoundation
