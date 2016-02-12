//
//  KforObjectSerializer.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "UString.h"
#include "Ref.h"
#include "PrintWriter.h"

// Self
#include "KforObjectSerializer.h"


namespace kfoundation {

// --- CONSTRUCTOR --- //

  KforObjectSerializer::KforObjectSerializer(Ref<OutputStream> stream,
      kf_int8_t indentUnits)
  : ObjectSerializer(stream, indentUnits)
  {
    // Nothing;
  }


// --- METHODS --- //

  void KforObjectSerializer::printHeader() {
    _isFirst = true;
  }


  void KforObjectSerializer::printAttribute(RefConst<UString> name,
      const Streamer& value, value_type_t valueType, bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }

    if(!name.isNull()) {
      getWriter() << *name << ": ";
    }

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


  void KforObjectSerializer::printObjectBegin(RefConst<UString> className,
      RefConst<UString> name, bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }
    printIndent(!_isFirst);
    if(!name.isNull()) {
      getWriter() << *name << ": ";
    }
    getWriter() << *className << '[';
    _isFirst = false;
  }


  void KforObjectSerializer::printObjectEnd(RefConst<UString> className,
      bool isLead)
  {
    getWriter() << ']';
  }


  void KforObjectSerializer::printNull(RefConst<UString> name, bool isLead) {
    if(!isLead) {
      getWriter() << ", ";
    }
    printIndent();
    if(name.isNull()) {
      getWriter() << "null";
    } else {
      getWriter() << *name << ": null";
    }
  }


  void KforObjectSerializer::printCollectionBegin(RefConst<UString> name,
      bool isLead)
  {
    if(!isLead) {
      getWriter() << ", ";
    }

    if(!name.isNull()) {
      getWriter() << *name << ": ";
    }

    getWriter() << '{';
  }


  void KforObjectSerializer::printCollectionEnd(bool isLead) {
    getWriter() << '}';
  }

} // namespace kfoundation