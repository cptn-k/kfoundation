/*---[SourceTransformation.cpp]-------------------------------m(._.)m--------*\
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

/**
 *
 * @file
 * Implements the `org::knorba::cxxprecompiler::TextTransformation` class.
 *
 */

#include "Ptr.h"
#include "UniChar.h"
#include "StringBuilder.h"

namespace kfoundation {

  namespace {
    
    // --- InsertionHead ------------------------------------------------------
    
    class InsertionHead : public StringBuilder::Insertion {
    private:
      kf_int64_t _pos;
      
    public:
      InsertionHead(const kf_int64_t pos);
      kf_int64_t getPos() const;
      
      void serialize(Ptr<ObjectSerializer> builder) const;
    };
    
    InsertionHead::InsertionHead(const kf_int64_t pos)
      : _pos(pos)
    {
      // Nothing
    }
    
    kf_int64_t InsertionHead::getPos() const {
      return _pos;
    }
    
    void InsertionHead::serialize(Ptr<ObjectSerializer> builder) const {
      builder->object("InsertionHead")
             ->attribute("pos", _pos)
             ->member("next")->object<StringBuilder::Insertion>(getNext())
             ->endObject();
    }
    
    
    // --- CharInsertion ------------------------------------------------------

    class CharInsertion : public StringBuilder::Insertion {
    private:
      wchar_t _buffer[32];
      short int _length;
      Ptr<StringBuilder::Insertion> _thisPtr;
      
    public:
      CharInsertion(const wchar_t& ch);
      
      Ptr<StringBuilder::Insertion>& ch(const wchar_t& ch);
      void printToStream(ostream& os) const;
      void serialize(Ptr<ObjectSerializer> builder) const;
    };
    
    CharInsertion::CharInsertion(const wchar_t& ch)
      : _length(1),
        _thisPtr(this)
    {
      _buffer[0] = ch;
    }
    
    Ptr<StringBuilder::Insertion>& CharInsertion::ch(const wchar_t &ch) {
      if(_length == 32) {
        return Insertion::setNext(new CharInsertion(ch));
      }
      
      _buffer[_length] = ch;
      _length++;
      
      return _thisPtr;
    }
    
    void CharInsertion::printToStream(ostream& os) const {
      kf_octet_t utf8[6];
      short int n;
      
      for(int i = 0; i < _length; i++) {
        n = UniChar::writeUtf8(_buffer[i], utf8);
        os.write((char*)utf8, n);
      }
      
      Insertion::printToStream(os);
    }
    
    void CharInsertion::serialize(Ptr<ObjectSerializer> builder) const {
      string str;
      
      kf_octet_t utf8[6];
      short int n;
      
      for(int i = 0; i < _length; i++) {
        n = UniChar::writeUtf8(_buffer[i], utf8);
        str.append((char*)utf8, n);
      }
      
      builder->object("CharInsertion")
        ->attribute("length", _length)
        ->attribute("chars", str)
        ->member("next")->object<StringBuilder::Insertion>(getNext())
        ->endObject();
    }
    
    
    // --- UniStringInsertion -------------------------------------------------
    
    class UniStringInsertion : public StringBuilder::Insertion {
    private:
      Ptr<UniString> _str;
      
    public:
      UniStringInsertion(Ptr<UniString>& str);
      ~UniStringInsertion();
      
      void printToStream(ostream& os) const;
      void serialize(Ptr<ObjectSerializer> builder) const;
    };
    
    UniStringInsertion::UniStringInsertion(Ptr<UniString>& str)
      : _str(str.retain())
    {
      // Nothing;
    }
    
    UniStringInsertion::~UniStringInsertion() {
      _str.release();
    }
    
    void UniStringInsertion::printToStream(ostream& os) const {
      _str->printToStream(os);
      Insertion::printToStream(os);
    }
    
    void UniStringInsertion::serialize(Ptr<ObjectSerializer> builder) const {
      builder->object("UniStringInsertion")
             ->member("content")->object<UniString>(_str)
             ->member("next")->object<StringBuilder::Insertion>(getNext())
             ->endObject();
    }
    
    
    // --- StringBuilderInsertion ---------------------------------------------
    
    class StringBuilderInsertion : public StringBuilder::Insertion {
    private:
      Ptr<StringBuilder> _str;
      
    public:
      StringBuilderInsertion(Ptr<StringBuilder>& _str);
      ~StringBuilderInsertion();
      
      void printToStream(ostream& os) const;
      void serialize(Ptr<ObjectSerializer> builder) const;
    };
    
    StringBuilderInsertion::StringBuilderInsertion(Ptr<StringBuilder>& str)
      : _str(str.retain())
    {
      // Nothing
    }
    
    StringBuilderInsertion::~StringBuilderInsertion() {
      _str.release();
    }
    
    void StringBuilderInsertion::printToStream(ostream& os) const {
      _str->printToStream(os);
      Insertion::printToStream(os);
    }
    
    void StringBuilderInsertion::serialize(Ptr<ObjectSerializer> builder) const
    {
      builder->object("StringBuilderInsertion")
             ->member("content")->object<StringBuilder>(_str)
             ->member("next")->object<StringBuilder::Insertion>(getNext())
             ->endObject();
    }
    
    
    // --- NumberInsertion ----------------------------------------------------
    
    class NumberInsertion : public StringBuilder::Insertion {
    private:
      long int _value;
      
    public:
      NumberInsertion(const long int& value);
      
      void printToStream(ostream& os) const;
      void serialize(Ptr<ObjectSerializer> buidler) const;
    };
    
    NumberInsertion::NumberInsertion(const long int& value)
      : _value(value)
    {
      // Nothing;
    }
    
    void NumberInsertion::printToStream(ostream& os) const {
      os << _value;
      Insertion::printToStream(os);
    }
    
    void NumberInsertion::serialize(Ptr<ObjectSerializer> builder) const {
      builder->object("NumberInsertion")
            ->attribute("value", _value)
            ->member("next")->object<StringBuilder::Insertion>(getNext())
            ->endObject();
    }
    
  } // namespace

  
  // --- StringBuilder::Insertion ---------------------------------------------
  
  StringBuilder::Insertion::Insertion()
    : _next(NULL)
  {
    // Nothing;
  }
  
  StringBuilder::Insertion::~Insertion() {
    if(_next.isValid()) {
      _next.release();
    }
  }
  
  Ptr<StringBuilder::Insertion>&
  StringBuilder::Insertion::setNext(StringBuilder::Insertion* const& next)
  {
    _next = next;
    return _next;
  }
  
  const Ptr<StringBuilder::Insertion>& StringBuilder::Insertion::getNext() const
  {
    return _next;
  }
  
  Ptr<StringBuilder::Insertion>& StringBuilder::Insertion::ch(const wchar_t &ch)
  {
    return setNext(new CharInsertion(ch));
  }
  
  Ptr<StringBuilder::Insertion>&
  StringBuilder::Insertion::str(Ptr<UniString>& str) {
    return setNext(new UniStringInsertion(str));
  }
  
  Ptr<StringBuilder::Insertion>&
  StringBuilder::Insertion::str(Ptr<StringBuilder>& str) {
    return setNext(new StringBuilderInsertion(str));
  }
  
  Ptr<StringBuilder::Insertion>&
  StringBuilder::Insertion::number(const long& n) {
    return setNext(new NumberInsertion(n));
  }
  
  void StringBuilder::Insertion::printToStream(ostream& stream) const {
    if(_next.isValid()) {
      _next->printToStream(stream);
    }
  }
  

  // -- StringBuidler::Removal ------------------------------------------------

  StringBuilder::Removal::Removal(const kf_int64_t& begin, const kf_int64_t& end)
    : _begin(begin),
      _end(end)
  {
    // Nothing
  }
  
  kf_int64_t StringBuilder::Removal::getBegin() const {
    return _begin;
  }
  
  kf_int64_t StringBuilder::Removal::getEnd() const {
    return _end;
  }
  
  bool StringBuilder::Removal::isInRange(const kf_int64_t& pos) {
    return pos >= _begin && pos <= _end;
  }
  
  void StringBuilder::Removal::serialize(Ptr<ObjectSerializer> builder) const
  {
    builder->object("Removal")
      ->attribute("begin", _begin)
      ->attribute("end", _end)
      ->endObject();
  }
  

  // -- StringBuilder ---------------------------------------------------------

  StringBuilder::StringBuilder()
    : _str(NULL),
      _begin(0),
      _end(0),
      _insertions(new ManagedArray<Insertion>()),
      _removals(new ManagedArray<Removal>())
  {
    _insertions.retain();
    _removals.retain();
  }

  StringBuilder::StringBuilder(Ptr<UniString> str)
    : _str(str.retain()),
      _begin(0),
      _end(str->getLength() - 1),
      _insertions(new ManagedArray<Insertion>()),
      _removals(new ManagedArray<Removal>())
  {
    _insertions.retain();
    _removals.retain();
  }
  
  StringBuilder::StringBuilder(Ptr<UniString> str, const kf_int64_t& begin,
                               const kf_int64_t& end)
    : _str(str.retain()),
      _begin(begin),
      _end(end),
      _insertions(new ManagedArray<Insertion>()),
      _removals(new ManagedArray<Removal>)
  {
    if(_end >= str->getLength())
      _end = str->getLength() - 1;
    
    _insertions.retain();
    _removals.retain();
  }
  
  StringBuilder::~StringBuilder() {
    if(_str.isValid())
      _str.release();
    
    _insertions.release();
    _removals.release();
  }
  
  void StringBuilder::insertIntoInsertions(
      Ptr<StringBuilder::Insertion> insertion)
  {
    int insertionIndex = _insertions->getSize();
    kf_int64_t pos = insertion.AS(InsertionHead)->getPos();
    
    for(int i = insertionIndex - 1; i >= 0; i--) {
      if(pos < _insertions->at(i).AS(InsertionHead)->getPos())
        insertionIndex = i;
      else
        break;
    }
    
    _insertions->insert(insertionIndex, insertion);
  }

  Ptr<StringBuilder::Insertion>
  StringBuilder::insertAfter(const kf_int64_t& pos) {
    if(_end == 0)
      return insertBefore(pos);
    
    Ptr<Insertion> insertion(new InsertionHead(pos + 1));
    insertIntoInsertions(insertion);
    return insertion.retain();
  }
  
  AutoPtr<StringBuilder::Insertion>
  StringBuilder::insertBefore(const kf_int64_t& pos) {
    Ptr<Insertion> insertion(new InsertionHead(pos));
    insertIntoInsertions(insertion);
    return insertion.retain();
  }
  
  AutoPtr<StringBuilder::Insertion> StringBuilder::append() {
    return insertAfter(0).retain();
  }
  
  AutoPtr<StringBuilder::Removal> StringBuilder::remove(long begin,
      long end)
  {
    Ptr<Removal> removal(new Removal(begin, end));

    int insertionIndex = _removals->getSize();
    
    for(int i = insertionIndex - 1; i >= 0; i--) {
      if(begin < _removals->get(i)->getBegin())
        insertionIndex = i;
      else
        break;
    }
    
    _removals->insert(insertionIndex, removal);

    return removal.retain();
  }
  
  void StringBuilder::printToStream(ostream &os) const {
    int activeRemovalIndex = 0;
    int nextInsertionIndex = 0;
    
    for(long int i = _begin; i <= _end; i++) {
      if(_insertions->getSize() > nextInsertionIndex) {
        if(_insertions->get(nextInsertionIndex).AS(InsertionHead)->getPos() == i) {
          _insertions->get(nextInsertionIndex)->printToStream(os);
          nextInsertionIndex++;
        }
      }
      
      if(activeRemovalIndex < _removals->getSize()) {
        Ptr<Removal> r = _removals->get(activeRemovalIndex);
        
        if(r->getEnd() < i) {
          while(r->getEnd() < i)
          {
            activeRemovalIndex++;
            if(activeRemovalIndex == _removals->getSize())
              break;
            r = _removals->get(activeRemovalIndex);
          }
        }
        
        if(r->isInRange(i)) {
          //os.put('@');
          continue;
        }
      }
      
      if(_str != NULL) {
        os.put((char)_str->getOctetAt((kf_int32_t)i));
      }
    }
  }
  
  void StringBuilder::serialize(Ptr<ObjectSerializer> builder) const {
    builder->object("StringBuilder")
      ->attribute("begin", _begin)
      ->attribute("end", _end)
      ->member("insertions")->object< ManagedArray<Insertion> >(_insertions)
      ->member("removals")->object< ManagedArray<Removal> >(_removals)
      ->endObject();
  }
  
} // kfoundation
