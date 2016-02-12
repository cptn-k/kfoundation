//
//  XmlObjectSerializer.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

// Internal
#include "UString.h"
#include "Ref.h"
#include "PrintWriter.h"
#include "BufferOutputStream.h"

// Self
#include "XmlObjectSerializer.h"

namespace kfoundation {


// --- STATIC METHODS --- //

  Ref<UString> XmlObjectSerializer::toString(SerializingStreamer& obj) {
    Ref<BufferOutputStream> stream = new BufferOutputStream();
    Ref<XmlObjectSerializer> serializer
    = new XmlObjectSerializer(stream.AS(OutputStream));
    obj.serialize(serializer.AS(ObjectSerializer));
    return stream->getString();
  }


  void XmlObjectSerializer::printToStream(SerializingStreamer& obj,
      Ref<OutputStream> os)
  {
    Ref<XmlObjectSerializer> serializer = new XmlObjectSerializer(os);
    obj.serialize(serializer.AS(ObjectSerializer));
  }

  Ref<PrintWriter> XmlObjectSerializer::print(Ref<PrintWriter> pw,
     SerializingStreamer& obj)
  {
    printToStream(obj, pw->getStream());
    return pw;
  }


// --- CONSTRUCTOR --- //

  XmlObjectSerializer::XmlObjectSerializer(Ref<OutputStream> stream,
      kf_int8_t indentUnits)
  : ObjectSerializer(stream, indentUnits)
  {
    _tagIsOpen = false;
  }


// --- METHODS --- //

  void XmlObjectSerializer::printHeader() {
    getWriter() << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
  }


  void XmlObjectSerializer::printAttribute(RefConst<UString> name,
      const Streamer& value, value_type_t type, bool isLead)
  {
    PrintWriter& writer = getWriter();

    writer << ' ' << name;

    if(type != NONE) {
      writer << "=\"" << value << '\"';
    }
  }


  void XmlObjectSerializer::printObjectBegin(RefConst<UString> className,
      RefConst<UString> name, bool isLead)
  {
    if(_tagIsOpen) {
      getWriter() << '>';
    }

    printIndent();

    if(name.isNull()) {
      getWriter() << '<' << className;
    } else {
      getWriter() << '<' << className << " _id=\"" << name << '\"';
    }

    _tagIsOpen = true;
  }


  void XmlObjectSerializer::printObjectEnd(RefConst<UString> className,
      bool isLead)
  {
    if(_tagIsOpen) {
      getWriter() << " />";
    } else {
      printIndent();
      getWriter() << "</" << className << '>';
    }

    _tagIsOpen = false;
  }


  void XmlObjectSerializer::printNull(RefConst<UString> name, bool isLead) {
    if(_tagIsOpen) {
      getWriter() << '>';
    }

    printIndent();
    if(name.isNull()) {
      getWriter() << "<null />";
    } else {
      getWriter() << "<null _id=\"" << name << "\">";
    }

    _tagIsOpen = false;
  }


  void XmlObjectSerializer::printCollectionBegin(RefConst<UString> name,
      bool isLead)
  {
    if(_tagIsOpen) {
      getWriter() << '>';
    }

    printIndent();
    if(name.isNull()) {
      getWriter() << "<collection ";
    } else {
      getWriter() << "<collection _id=\"" << name << '\"';
    }

    _tagIsOpen = true;
  }


  void XmlObjectSerializer::printCollectionEnd(bool hadMembers) {
    if(_tagIsOpen) {
      getWriter() << " />";
    } else {
      printIndent();
      getWriter() << "</collection>";
    }

    _tagIsOpen = false;
  }

} // namespace kfoundation