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
#include "PtrDecl.h"
#include "ManagedObject.h"
#include "ManagedArray.h"
#include "SerializingStreamer.h"
#include "ObjectSerializer.h"
#include "UniString.h"

namespace kfoundation {

  class StringBuilder : public ManagedObject, public SerializingStreamer {
  public:
    class Insertion : public ManagedObject, public SerializingStreamer {
    private:
      Ptr<Insertion> _next;
      
    protected:
      const Ptr<Insertion>& getNext() const;
      Ptr<Insertion>& setNext(Insertion* const& next);
      
    public:
      Insertion();
      ~Insertion();
      
      virtual Ptr<Insertion>& ch(const wchar_t& ch);
      virtual Ptr<Insertion>& str(Ptr<UniString>& str);
      virtual Ptr<Insertion>& str(Ptr<StringBuilder>& builder);
      virtual Ptr<Insertion>& number(const long int& n);
    
      // From SerializingStreamer::Streamer
      virtual void printToStream(ostream& os) const;
      
      // From SerializingStreamer
      virtual void serialize(Ptr<ObjectSerializer> builder) const = 0;
    };
    
    class Removal : public ManagedObject, public SerializingStreamer {
    private:
      kf_int64_t _begin;
      kf_int64_t _end;
      
    public:
      Removal(const kf_int64_t& begin, const kf_int64_t& end);
      kf_int64_t getBegin() const;
      kf_int64_t getEnd() const;
      bool isInRange(const kf_int64_t& pos);
      
      // From SerializingStreamer
      void serialize(Ptr<ObjectSerializer> builder) const;
    };
    
  private:
    kf_int64_t     _begin;
    kf_int64_t     _end;
    Ptr<UniString> _str;
    Ptr< ManagedArray<Insertion> > _insertions;
    Ptr< ManagedArray<Removal> >   _removals;
    
    void insertIntoInsertions(Ptr<Insertion> insertion);
    
  public:
    StringBuilder();
    StringBuilder(Ptr<UniString> str);
    StringBuilder(Ptr<UniString> str, const kf_int64_t& begin,
                  const kf_int64_t& end);
    
    ~StringBuilder();
    
    AutoPtr<Insertion> insertAfter(const kf_int64_t& pos);
    AutoPtr<Insertion> insertBefore(const kf_int64_t& pos);
    AutoPtr<Insertion> append();
    AutoPtr<Removal> remove(kf_int64_t begin, kf_int64_t end);
    
    // From SerializingStreamer::Streamer
    public: virtual void printToStream(ostream& os) const;
    
    // From SerializingStreamer
    public: void serialize(Ptr<ObjectSerializer> builder) const;
  };
  
} // kfoundation

#endif
