//
//  XmlObjectSerializer.hpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef XmlObjectSerializer_hpp
#define XmlObjectSerializer_hpp

// Internal
#include "definitions.h"

// Super
#include "ObjectSerializer.h"

namespace kfoundation {

  class XmlObjectSerializer : public ObjectSerializer {

  // --- STATIC METHODS --- //

    public: static Ref<UString> toString(SerializingStreamer& obj);
    public: static void printToStream(SerializingStreamer& obj,
        Ref<OutputStream> os);
    public: static Ref<PrintWriter> print(Ref<PrintWriter> pw,
        SerializingStreamer& obj);


  // --- FIELDS --- //

    private: bool _tagIsOpen;


  // --- CONSTRUCTOR --- //

    public: XmlObjectSerializer(Ref<OutputStream> stream, kf_int8_t indentUnits = 4);


  // --- METHODS --- //

    // From ObjectSerializer
    public: void printHeader();

    public: void printAttribute(RefConst<UString> name, const Streamer& value,
                                value_type_t valueType, bool isLead);

    public: void printObjectBegin(RefConst<UString> className,
                                  RefConst<UString> name, bool isLead);

    public: void printObjectEnd(RefConst<UString> className, bool hadMembers);
    public: void printNull(RefConst<UString> name, bool isLead);
    public: void printCollectionBegin(RefConst<UString> name, bool isLead);
    public: void printCollectionEnd(bool isLead);
    
  };
  
} // namespace kfoundation

#endif /* XmlObjectSerializer_hpp */