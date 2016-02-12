//
//  KforObjectSerializer.hpp
//  KFoundation
//
//  Created by Kay Khandan on 12/11/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef KforObjectSerializer_hpp
#define KforObjectSerializer_hpp

//Super
#include "ObjectSerializer.h"

namespace kfoundation {

  class KforObjectSerializer : public ObjectSerializer {

  // --- CONSTRUCTOR --- //

    public: KforObjectSerializer(Ref<OutputStream> stream,
        kf_int8_t indentUnits = 4);


  // --- FIELDS --- //

    private: bool _isFirst;


  // --- METHODS --- //

    // From ObjectSerializer
    public: void printHeader();

    public: void printAttribute(RefConst<UString> name, const Streamer& value,
        value_type_t valueType, bool isLead);

    public: void printObjectBegin(RefConst<UString> className,
        RefConst<UString> name, bool isLead);

    public: void printObjectEnd(RefConst<UString> className, bool isLead);
    public: void printNull(RefConst<UString> name, bool isLead);
    public: void printCollectionBegin(RefConst<UString> name, bool isLead);
    public: void printCollectionEnd(bool isLead);
    
  };

} // namespace kfoundation

#endif /* KforObjectSerializer_hpp */