// Internal
#include "UString.h"
#include "Ref.h"
#include "OutputStream.h"
#include "PrintWriter.h"
#include "StringPrintWriter.h"
#include "Array.h"
#include "UChar.h"
#include "Int32.h"
#include "Int64.h"
#include "Double.h"
#include "Bool.h"
#include "Array.h"

// Self
#include "ObjectSerializer.h"


namespace kfoundation {

//\/ ObjectDumpBuilderException /\/////////////////////////////////////////////

  /**
   * Constructor.
   *
   * @param message A message describing the problem.
   */
  
  ObjectDumpBuilderException::ObjectDumpBuilderException(RefConst<UString> message)
  : KFException(message)
  {
    setName(K"ObjectSerializerException");
  }
  

//\/ ObjectSerializer::StackItem /\////////////////////////////////////////////

  void ObjectSerializer::StackItem::set(state_t state, RefConst<UString> name,
      bool isLead)
  {
    _state = state;
    _name = name;
    _isLead = isLead;
  }


//\/ ObjectSerializer /\///////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const StaticRefConst<UChar> ObjectSerializer::SPACE = new UChar(' ');


// --- CONSTRUCTORS --- //

  /**
   * Constructor, sets output stream, type, and indent units.
   *
   * @param stream The stream to print the output to.
   * @param outputType The output format.
   * @param indentUnit Number of spaces for each indention level.
   */

  ObjectSerializer::ObjectSerializer(Ref<OutputStream> stream,
      kf_int8_t indentUnit)
  : _state(ROOT),
    _indentUnit(indentUnit),
    _indent(0),
    _isLead(true),
    _writer(new PrintWriter(stream)),
    _stack(new Array<StackItem>())
  {
    // Nothing
  }


// --- METHODS --- //
  
  Ref<ObjectSerializer> ObjectSerializer::attribute(RefConst<UString> name,
      const Streamer& value, value_type_t type)
  {
    if(_state != OBJECT) {
      throw ObjectDumpBuilderException(K"Attribute is only allowed inside an "
          "object, before decleration of any member. Path: " + stackToString());
    }

    printAttribute(name, value, type, _isLead);

    _isLead = false;
    
    return this;
  } // attribute(const string&, const string&, value_type_t)

  
  RefConst<UString> ObjectSerializer::stateToString(state_t type) const {
    switch(type) {
      case ROOT       : return K"root";
      case MEMBER     : return K"member";
      case OBJECT     : return K"object";
      case OBJECT_BODY: return K"object-body";
      case PROPERTY   : return K"property";
      case VALUE      : return K"value";
      case COLLECTION : return K"collection";
    }
    return K"unknown";
  } // stateToString(state_t)
  

  RefConst<UString> ObjectSerializer::stackToString() const {
    if(_stack->isEmpty()) {
      return K"/";
    }

    StringPrintWriter writer;
    size_t s = _stack->getSize();
    for(int i = 0; i < s; i++) {
      StackItem& it = _stack->at(i);
      if(it._name.isNull()) {
        writer << "__NULL__";
      } else {
        writer << it._name;
      }
      writer << " (" << stateToString(it._state) << ") --> ";
    }
    writer << '(' << stateToString(_state) << ')';

    return writer.toString();
  } // stackToString()
  
  
  void ObjectSerializer::printIndent(bool newLine) {
    if(newLine) {
      _writer->newLine();
    }
    if(_indentUnit != -1) {
      _writer->print(*SPACE, _indent * _indentUnit);
    }
  }

  
  /**
   * Used to output an object owned by the current one. Only allowed after
   * object() and attribute().
   *
   * @param name The corresponding member variable name (property name).
   * @return Pointer to self.
   */
  
  Ref<ObjectSerializer> ObjectSerializer::member(RefConst<UString> name) {
    if(_state != OBJECT && _state != OBJECT_BODY) {
      throw ObjectDumpBuilderException(K"Error adding member \"" + name
          + "\". Members are only allowed within objects. Path: "
          + stackToString());
    }

    _stack->push().set(_state, name, false);

    _name = name;
    _state = MEMBER;

    return this;
  } // member(const string&)


  /**
   * Used to output an object. Allowed in the begining or after member().
   * 
   * @param className Class name of the designated object.
   * @return Pointer to self.
   */
  
  Ref<ObjectSerializer> ObjectSerializer::object(RefConst<UString> className)
  {
    if(_state != ROOT && _state != MEMBER && _state != COLLECTION) {
      throw ObjectDumpBuilderException(K"Error adding object \'"
          + className
          + "\". Adding object is allowed only at root, in colelction, "
            "or after member. Path: "
          + stackToString());
    }

    if(_state == ROOT) {
      printHeader();
    }

    printObjectBegin(className, _name, _isLead);

    _stack->push().set(_state, className, false);

    _state = OBJECT;
    _isLead = true;
    _name = NULL;

    _indent++;
    
    return this;
  } // object(const string&)
  

  Ref<ObjectSerializer> ObjectSerializer::object(const SerializingStreamer &obj)
  {
    obj.serialize(this);
    return this;
  }


  /**
   * Serializes a `char` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const wchar_t value) {
    return attribute(name, UChar(value), CHAR);
  }


  /**
   * Serializes an `int` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const kf_int32_t value) {
    return attribute(name, Int32(value), NUMBER);
  }


  /**
   * Serializes an `unsigned int` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const kf_int64_t value) {
    return attribute(name, Int64(value), NUMBER);
  }


  /**
   * Serializes a `double` attribuet.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const double value) {
    return attribute(name, Double(value), NUMBER);
  }


  /**
   * Serializes a `bool` attribute.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const bool value) {
    return attribute(name, Bool(value), BOOL);
  }


  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, const Streamer& value) {
    return attribute(name, value, STRING);
  }


  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name, RefConst<UString> value) {
    return attribute(name, *value, STRING);
  }


  /**
   * Serializes an attribute with no value.
   */

  Ref<ObjectSerializer>
  ObjectSerializer::attribute(RefConst<UString> name) {
    return attribute(name, *name, NONE);
  }


  /**
   * Used to print a field that is `NULL`. Only allowed after member().
   */

  Ref<ObjectSerializer> ObjectSerializer::null() {
    if(_state != MEMBER && _state != COLLECTION) {
      throw ObjectDumpBuilderException(K"null is only allowed after member or"
          " in collection. Path: " + stackToString());
    }

    if(_state == MEMBER) {
      printNull(_name, _isLead);
      StackItem item = _stack->pop();
      _state = item._state;
      _isLead = item._isLead;
      _name = NULL;
    } else /*if(_state == COLLECTION)*/ {
      printNull(NULL, _isLead);
      _isLead = false;
    }

    return this;
  } // null()


  /**
   * Marks the end of an object started by the latest call of object() method.
   * @return Pointer to self.
   */
  
  Ref<ObjectSerializer> ObjectSerializer::endObject() {
    if(_state != OBJECT && _state != OBJECT_BODY) {
      throw ObjectDumpBuilderException(K"Error attemp to end object"
          " doesn't match any prior object decleration. Path: "
          + stackToString());
    }

    StackItem item = _stack->pop();

    _indent--;
    printObjectEnd(item._name, _isLead);

    _state = item._state;
    _isLead = item._isLead;



    if(_state ==  MEMBER) {
      item = _stack->pop();
      _state = item._state;
      _isLead = item._isLead;
    }
    
    return this;
  } // endObject();


  /**
   * Used to print a collection. Allowed only after member().
   * @return Pointer to self.
   */
  
  Ref<ObjectSerializer> ObjectSerializer::collection() {
    if(_state != ROOT && _state != MEMBER) {
      throw ObjectDumpBuilderException(
          K"Collection is only allowed as root or member. Path: "
          + stackToString());
    }

    printCollectionBegin(_name, _isLead);

    _isLead = false;

    _stack->push().set(_state, NULL, _isLead);

    _isLead = true;
    _state = COLLECTION;
    _name = NULL;

    _indent++;

    return this;
  } // collection(const string&)


  PrintWriter& ObjectSerializer::getWriter() {
    return *_writer;
  }


  /**
   * Marks end of a collection started by the latest unclosed call to 
   * collection().
   * @return Pointer to self.
   */
  
  Ref<ObjectSerializer> ObjectSerializer::endCollection() {
    if(_state != COLLECTION) {
      throw ObjectDumpBuilderException(
          K"End collection doesn't match the opening of any collection. Path: "
          + stackToString());
    }

    _indent--;
    printCollectionEnd(_isLead);

    StackItem item = _stack->pop();
    _state = item._state;
    _isLead = item._isLead;


    if(_state ==  MEMBER) {
      item = _stack->pop();
      _state = item._state;
      _isLead = item._isLead;
    }
    
    if(_stack->isEmpty()) {
      _writer->newLine();
    }

    return this;
  }

} // namespace kfoundation