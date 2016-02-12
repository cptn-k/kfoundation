//
//  UString.h
//  KnoRBA-Common
//
//  Created by Hamed KHANDAN on 9/2/13.
//
//

#ifndef ORG_KNORBA_COMMON_KFString_H
#define ORG_KNORBA_COMMON_KFString_H

// Internal
#include "definitions.h"
#include "RefDecl.h"
#include "IndexOutOfBoundException.h"
#include "UChar.h"

// Super
#include "KFObject.h"
#include "SerializingStreamer.h"
#include "Comparable.h"

#define K UStringLiteral::EMPTY +

namespace kfoundation {

  class UChar;
  class OctetIterator;
  class UCharIterator;

//\/ UString /\////////////////////////////////////////////////////////////////

  class UString
  : public KFObject, public Streamer, public Comparable<UString>
  {

  // --- STATIC FIELDS --- //

    public: static const StaticRefConst<UString> EMPTY;
    private: static const kf_int8_t HEADER_SIZE = 4;


  // --- FIELDS --- //

    private: mutable kf_octet_t* _buffer;
    private: mutable kf_int32_t _hash;


  // --- (DE)CONSTRUCTORS --- //

    public: UString();
    public: UString(const char* cstring);
    public: UString(const kf_octet_t* raw);
   private: UString(const kf_int32_t nOctets);
    public: UString(const kf_octet_t* raw, const kf_int32_t size);
    public: UString(RefConst<UString> str);
    public: ~UString();


  // --- STATIC METHODS --- //

    public: static kf_int32_t cStringLength(const kf_octet_t* src);
    private: static kf_octet_t* makeBuffer(const kf_octet_t* src, kf_int32_t size);

    
  // --- METHODS --- //

    protected: virtual void releaseOwner() const;
    protected: virtual const kf_octet_t* getExternalOctets() const;
    protected: virtual kf_int32_t getExternalOctetCount() const;

    public: inline const kf_octet_t* getOctets() const;
    public: inline kf_int32_t  getOctetCount() const;

    public: inline kf_octet_t at(const kf_int32_t octetIndex) const throw(IndexOutOfBoundException);
    public: const char* getCString() const;
    public: UChar       getUCharAtOffset(const kf_int32_t offset) const;
    public: kf_int32_t  getHashCode() const;
    public: kf_int32_t  getCharCount() const;
    public: bool        isEmpty() const;

    public: void internalSet(kf_octet_t* buffer);
    public: void set(const kf_octet_t* raw);
    public: void set(const kf_octet_t* raw, kf_int32_t size);
    public: void set(RefConst<UString> other);

    public: bool equalsIgnoreCase(RefConst<UString> str) const;

    public: kf_int32_t find(const kf_octet_t octet, const kf_int32_t offset = 0) const;
    public: kf_int32_t find(UChar uchar, const kf_int32_t offset = 0) const;
    public: kf_int32_t find(RefConst<UString> str, const kf_int32_t offset = 0) const;

    public: Ref<UString> toLowercase() const;
    public: Ref<UString> toUppercase() const;
    public: Ref<UString> subString(const kf_int32_t begin, const kf_int32_t end) const;
    public: Ref<UString> append(RefConst<UString> str) const;
    public: Ref<UString> append(const kf_octet_t* raw, const kf_int32_t size) const;

    public: OctetIterator getOctetIterator() const;
    public: UCharIterator getUCharIterator() const;

    // From Comparable<UString>
    public: bool equals(RefConst<UString> other) const;

    // From SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };


  Ref<UString> operator+(RefConst<UString> str, const bool v);
  Ref<UString> operator+(RefConst<UString> str, const kf_int32_t v);
  Ref<UString> operator+(RefConst<UString> str, const kf_int64_t v);
  Ref<UString> operator+(RefConst<UString> str, const float v);
  Ref<UString> operator+(RefConst<UString> str, const double v);
  Ref<UString> operator+(RefConst<UString> str, const wchar_t ch);
  Ref<UString> operator+(RefConst<UString> str, const char* cstr);
  Ref<UString> operator+(RefConst<UString> str, const Streamer& v);
  Ref<UString> operator+(RefConst<UString> str, RefConst<UString> v);


  inline const kf_octet_t* UString::getOctets() const {
    return IS_NULL(_buffer)?getExternalOctets():_buffer + HEADER_SIZE;
  }


  /**
   * Returns the number of characters in this UString.
   */

  inline kf_int32_t UString::getOctetCount() const {
    return IS_NULL(_buffer)?getExternalOctetCount():*(kf_int32_t*)_buffer;
  }


  inline kf_octet_t UString::at(const kf_int32_t octetIndex) const
  throw(kfoundation::IndexOutOfBoundException)
  {
    if(octetIndex >= getOctetCount()) {
      throw IndexOutOfBoundException(RefConst<UString>(new UString())
          + "Attempt to access octet " + octetIndex
          + " of an string of size " + getOctetCount());
    }
    return getOctets()[octetIndex];
  }


//\/ UStringWindow /\//////////////////////////////////////////////////////////

  class UStringWindow : public UString {

  // --- FIELDS --- //

    private: mutable RefConst<UString> _bufferOwner;
    private: kf_int32_t _offset;
    private: kf_int32_t _size;


  // --- CONSTRUCTOR --- //

    public: UStringWindow(RefConst<UString> str, kf_int32_t begin, kf_int32_t end);


  // --- METHODS --- //

    // From UString
    public: void releaseOwner() const;
    public: inline const kf_octet_t* getExternalOctets() const;
    public: inline kf_int32_t getExternalOctetCount() const;

  };


  inline const kf_octet_t* UStringWindow::getExternalOctets() const {
    return _bufferOwner->getOctets() + _offset;
  }


  inline kf_int32_t UStringWindow::getExternalOctetCount() const {
    return _size;
  }



//\/ UStringLiteral /\/////////////////////////////////////////////////////////

  class UStringLiteral : public UString {

  // --- STATIC FIELDS --- //

    public: static const UStringLiteral EMPTY;


  // --- FIELDS --- //

    private: const kf_octet_t* _memory;
    private: kf_int32_t _size;


  // --- CONSTRUCTORS --- //

    private: UStringLiteral();
    private: UStringLiteral(const kf_octet_t* memory, const kf_int32_t size);


  // --- METHODS --- //

    public: inline const kf_octet_t* getExternalOctets() const;
    public: inline kf_int32_t getExternalOctetCount() const;


  // --- OPERATORS --- //

    public: RefConst<UString> operator+(const char* cstr) const;

  };


  inline const kf_octet_t* UStringLiteral::getExternalOctets() const {
    return _memory;
  }


  inline kf_int32_t UStringLiteral::getExternalOctetCount() const {
    return _size;
  }


//\/ UStringOctetIterator /\///////////////////////////////////////////////////

  class OctetIterator {

  // --- FIELDS --- //

    private: RefConst<UString> _bufferOwner;
    private: const kf_octet_t* _begin;
    private: const kf_octet_t* _end;
    private: const kf_octet_t* _pos;


  // --- CONSTRUCTORS --- //

    public: OctetIterator(RefConst<UString> str);
    public: OctetIterator(const kf_octet_t* octets, kf_int32_t size);


  // --- STATIC METHODS --- //

    public: static const kf_octet_t* find(const kf_octet_t* seq1,
        const kf_octet_t* end1, const kf_octet_t* seq2, const kf_octet_t* end2);

  // --- METHODS --- //

    public: inline kf_octet_t first();
    public: inline kf_octet_t next();
    public: inline bool hasMore() const;
    public: kf_int32_t seek(const kf_octet_t octet);
    public: kf_int32_t seek(UChar ch);
    public: kf_int32_t seek(RefConst<UString> str);
    public: kf_int32_t getIndex() const;
    public: kf_octet_t get() const;

  };


  inline kf_octet_t OctetIterator::first() {
    _pos = _begin;
    return *_pos;
  }


  inline kf_octet_t OctetIterator::next() {
    _pos++;
    return *_pos;
  }


  inline bool OctetIterator::hasMore() const {
    return _pos < _end;
  }


//\/ UCharIterator /\//////////////////////////////////////////////////////////

  class UCharIterator : public UChar {

  // --- FIELDS --- //

    private: RefConst<UString> _bufferOwner;
    private: const kf_octet_t* _begin;
    private: const kf_octet_t* _end;
    private: const kf_octet_t* _pos;
    private: kf_int32_t _charIndex;


  // --- CONSTRUCTOR --- //

    public: UCharIterator(RefConst<UString> str);


  // --- METHODS --- //

    public: inline void first();
    public: inline void next();
    public: inline bool hasMore() const;
//    public: kf_int32_t seek(UChar ch);
//    public: kf_int32_t seek(RefConst<UString> str);
    public: kf_int32_t getOctetIndex() const;
    public: kf_int32_t getCharIndex() const;

  };


  inline void UCharIterator::first() {
    _pos = _begin + read(_begin);
    _charIndex = 0;
  }


  inline void UCharIterator::next() {
    _pos += read(_pos);
    _charIndex++;
  }


  bool UCharIterator::hasMore() const {
    return _pos <= _end;
  }

} // namespace kfoundation

#endif