//
//  JsonObjectSerializer.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#include "JsonObjectSerializer.h"

// Internal
#include "UString.h"
#include "Ref.h"
#include "PrintWriter.h"
#include "BufferOutputStream.h"

// Self
#include "ObjectSerializer.h"


namespace kfoundation {

// --- STATIC METHODS --- //

  Ref<UString> JsonObjectSerializer::toString(SerializingStreamer& obj) {
    Ref<BufferOutputStream> stream = new BufferOutputStream();
    Ref<JsonObjectSerializer> serializer
        = new JsonObjectSerializer(stream.AS(OutputStream));
    obj.serialize(serializer.AS(ObjectSerializer));
    return stream->getString();
  }


  void JsonObjectSerializer::printToStream(SerializingStreamer& obj,
      Ref<OutputStream> os)
  {
    Ref<JsonObjectSerializer> serializer = new JsonObjectSerializer(os);
    obj.serialize(serializer.AS(ObjectSerializer));
  }


  Ref<PrintWriter> JsonObjectSerializer::print(Ref<PrintWriter> pw,
      SerializingStreamer& obj)
  {
    printToStream(obj, pw->getStream());
    return pw;
  }


// --- CONSTRUCTOR --- //

  JsonObjectSerializer::JsonObjectSerializer(Ref<OutputStream> stream,
      kf_int8_t indentUnits)
  : ObjectSerializer(stream, indentUnits)
  {
    // Nothing;
  }


// --- METHODS --- //

  void JsonObjectSerializer::printHeader() {
    _isFirst = true;
  }


  void JsonObjectSerializer::printAttribute(RefConst<UString> name,
      const Streamer& value, value_type_t valueType, bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }

    printIndent();

    getWriter() << *name << ": ";

    switch(valueType) {
      case ObjectSerializer::NONE:
        getWriter() << "null";
        break;

      case ObjectSerializer::NUMBER:
      case ObjectSerializer::BOOL:
        getWriter() << value;
        break;

      case ObjectSerializer::CHAR:
        getWriter() << '\'' << value << '\'';
        break;

      default:
        getWriter() << '\"' << value << '\"';
    }

    _isFirst = false;
  }


  void JsonObjectSerializer::printObjectBegin(RefConst<UString> className,
      RefConst<UString> name, bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }

    printIndent(!_isFirst);

    if(!name.isNull()) {
      getWriter() << '\"' << *name << "\": ";
    }

    getWriter() << '{';
  }


  void JsonObjectSerializer::printObjectEnd(RefConst<UString> className,
      bool isLead)
  {
    if(!isLead) {
      printIndent();
    }
    getWriter() << '}';
  }


  void JsonObjectSerializer::printNull(RefConst<UString> name, bool isLead) {
    if(!isLead) {
      getWriter() << ", ";
    }
    printIndent();
    getWriter() << '\"' << *name << "\": null";
  }


  void JsonObjectSerializer::printCollectionBegin(RefConst<UString> name,
      bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }
    printIndent();
    if(!name.isNull()) {
      getWriter() << '\"' << *name << "\": ";
    }
    getWriter() << '[';
  }


  void JsonObjectSerializer::printCollectionEnd(bool isLead) {
    if(!isLead) {
      printIndent();
    }
    getWriter() << ']';
  }

} // namespace kfoundation