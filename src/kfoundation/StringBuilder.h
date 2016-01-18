/*---[TextTransformation.h]-----------------------------------m(._.)m--------*\
|
|  Part of KnoRBA CXX Precompiler
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

#ifndef ORG_KNORBA_CXXPREPROCESSOR_SOURCETRANSFORMATION_H
#define ORG_KNORBA_CXXPREPROCESSOR_SOURCETRANSFORMATION_H

#include "definitions.h"
#include "RefDecl.h"
#include "KFObject.h"
#include "RefArray.h"
#include "SerializingStreamer.h"
#include "ObjectSerializer.h"
#include "UniString.h"

namespace kfoundation {

  class StringBuilder : public KFObject, public SerializingStreamer {
  public:
    class Insertion : public KFObject, public SerializingStreamer {
    private:
      Ref<Insertion> _next;
      
    protected:
      const Ref<Insertion>& getNext() const;
      Ref<Insertion>& setNext(Insertion* const& next);
      
    public:
      Insertion();
      ~Insertion();
      
      virtual Ref<Insertion>& ch(const wchar_t& ch);
      virtual Ref<Insertion>& str(Ref<UniString>& str);
      virtual Ref<Insertion>& str(Ref<StringBuilder>& builder);
      virtual Ref<Insertion>& number(const long int& n);
    
      // From SerializingStreamer::Streamer
      virtual void printToStream(ostream& os) const;
      
      // From SerializingStreamer
      virtual void serialize(Ref<ObjectSerializer> builder) const = 0;
    };
    
    class Removal : public KFObject, public SerializingStreamer {
    private:
      kf_int64_t _begin;
      kf_int64_t _end;
      
    public:
      Removal(const kf_int64_t& begin, const kf_int64_t& end);
      kf_int64_t getBegin() const;
      kf_int64_t getEnd() const;
      bool isInRange(const kf_int64_t& pos);
      
      // From SerializingStreamer
      void serialize(Ref<ObjectSerializer> builder) const;
    };
    
  private:
    kf_int64_t     _begin;
    kf_int64_t     _end;
    Ref<UniString> _str;
    Ref< RefArray<Insertion> > _insertions;
    Ref< RefArray<Removal> >   _removals;
    
    void insertIntoInsertions(Ref<Insertion> insertion);
    
  public:
    StringBuilder();
    StringBuilder(Ref<UniString> str);
    StringBuilder(Ref<UniString> str, const kf_int64_t& begin,
                  const kf_int64_t& end);
    
    ~StringBuilder();
    
    AutoPtr<Insertion> insertAfter(const kf_int64_t& pos);
    AutoPtr<Insertion> insertBefore(const kf_int64_t& pos);
    AutoPtr<Insertion> append();
    AutoPtr<Removal> remove(kf_int64_t begin, kf_int64_t end);
    
    // From SerializingStreamer::Streamer
    public: virtual void printToStream(ostream& os) const;
    
    // From SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> builder) const;
  };
  
} // kfoundation

#endif
