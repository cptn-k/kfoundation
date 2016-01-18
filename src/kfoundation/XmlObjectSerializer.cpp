//
//  XmlObjectSerializer.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#include "XmlObjectSerializer.hpp"


// Std
#include <stack>

// Internal
#include "UString.h"
#include "Ref.h"

// Self
#include "ObjectSerializer.h"


namespace kfoundation {

  // --- ObjectDumpBuilderException -------------------------------------------


  /**
   * Constructor.
   *
   * @param message A message describing the problem.
   */

  ObjectDumpBuilderException::ObjectDumpBuilderException(string message)
  : KFException(message)
  {
    setName("ObjectSerializerException");
  }


  // --- ObjectSerializer (private) ------------------------------------------

  Ref<ObjectSerializer> ObjectSerializer::attribute(const string& name,
                                                    const string& value, value_type_t type)
  {
    if(_state != OBJECT) {
      throw ObjectDumpBuilderException("Attribute is only allowed inside an "
                                       "object, before decleration of any member. Path: " + stackToString());
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

    return this;
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


  /**
   * Constructor, sets output stream, type, and indent units.
   *
   * @param stream The stream to print the output to.
   * @param outputType The output format.
   * @param indentUnit Number of spaces for each indention level.
   */

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


  /**
   * Constructor, sets the output stream and type, and sets indent units to 4.
   *
   * @param stream The stream to print the output to.
   * @param outputType The output format.
   */

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


  /**
   * Used to output an object owned by the current one. Only allowed after
   * object() and attribute().
   *
   * @param name The corresponding member variable name (property name).
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::member(const string& name) {
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

    return this;
  } // member(const string&)


  /**
   * Used to output an object. Allowed in the begining or after member().
   *
   * @param className Class name of the designated object.
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::object(const string& className) {
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

    return this;
  } // object(const string&)


  /**
   * Used to output a field which already has SerializingStreamer interface
   * implemented. Allowed only after member(). endObject() should NOT be called
   * for this method.
   *
   * @param ref The field to be printed.
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::object(const SerializingStreamer& ref)
  {
    ref.serialize(this);
    return this;
  }


  /**
   * Marks the end of an object started by the latest call of object() method.
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::endObject() {
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

    return this;
  } // endObject();


  Ref<ObjectSerializer> ObjectSerializer::text(const string& value) {
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

    return this;
  } // text(const string&)


  /**
   * Used to print a field that is `NULL`. Only allowed after member().
   */

  Ref<ObjectSerializer> ObjectSerializer::null() {
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

    return this;
  } // null()


  /**
   * Used to print a collection. Allowed only after member().
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::collection() {
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

    return this;
  } // collection(const string&)


  /**
   * Marks end of a collection started by the latest unclosed call to
   * collection().
   * @return Pointer to self.
   */

  Ref<ObjectSerializer> ObjectSerializer::endCollection() {
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

    return this;
  }


  /**
   * Serializes a `string` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, const string& value) {
    return attribute(name, value, STRING);
  }


  /**
   * Serializes a `char` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, char value) {
    return attribute(name, UChar(value).toString(), CHAR);
  }


  /**
   * Serializes an `int` attribute.
   */

  Ref<ObjectSerializer> ObjectSerializer::attribute(const string& name, int value) {
    return attribute(name, Int(value).toString(), NUMBER);
  }


  /**
   * Serializes an `unsigned int` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, unsigned int value) {
    return attribute(name, Int(value).toString(), NUMBER);
  }


  /**
   * Serializes a `long int` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, long int value) {
    return attribute(name, LongInt::toString(value), NUMBER);
  }


  /**
   * Serializes an `unsigned long int` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, unsigned long int value) {
    return attribute(name, LongInt::toString(value), NUMBER);
  }


  /**
   * Serializes a `double` attribuet.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, double value) {
    return attribute(name, Double(value).toString(), NUMBER);
  }


  /**
   * Serializes a `bool` attribute.
   */
  
  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name, bool value) {
    return attribute(name, Bool::toString(value), BOOL);
  }
  
  
  /**
   * Serializes an attribute with no value.
   */
  
  Ref<ObjectSerializer>
  ObjectSerializer::attribute(const string& name) {
    return attribute(name, "", NONE);
  }
  
  
} // namespace kfoundation