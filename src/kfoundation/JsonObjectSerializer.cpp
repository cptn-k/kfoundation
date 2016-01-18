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

// Self
#include "ObjectSerializer.h"


namespace kfoundation {

// --- CONSTRUCTOR --- //

  JsonObjectSerializer::JsonObjectSerializer(Ref<OutputStream> stream,
      kf_int8_t indentUnits)
  : ObjectSerializer(stream, indentUnits)
  {
    // Nothing;
  }


// --- METHODS --- //

  void JsonObjectSerializer::printHeader() {
    // Nothing;
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

  }


  void JsonObjectSerializer::printObjectBegin(RefConst<UString> className,
      RefConst<UString> name, bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }

    printIndent();

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