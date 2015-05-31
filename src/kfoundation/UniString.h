//
//  String.h
//  KnoRBA-Common
//
//  Created by Hamed KHANDAN on 9/2/13.
//
//

#ifndef ORG_KNORBA_COMMON_STRING_H
#define ORG_KNORBA_COMMON_STRING_H

#include "definitions.h"
#include "ManagedObject.h"
#include "Streamer.h"
#include "PtrDecl.h"
#include "IndexOutOfBoundException.h"

#define U(X) AutoPtr<UniString>(new UniString(L ## X))

namespace kfoundation {
  
  class UniChar;

  class UniString : public ManagedObject, public Streamer {
  private:
    kf_int32_t _nOctets;
    kf_int32_t _nCodePoints;
    kf_octet_t* _buffer;
    kf_int32_t _hash;
    
    void from(const char* const& cstr, kf_int32_t size);
    void from(const wchar_t* const& cstr, kf_int32_t size);
    void recalculateHash();
    
  protected:
    UniString(kf_octet_t* externalBuffer);
    
  public:
    const static kf_int32_t NOT_FOUND = -1;
    
    UniString();
    UniString(const char* const& cstr);
    UniString(const char* const& cstr, kf_int32_t size);
    UniString(const wchar_t* const& cstr);
    UniString(const wchar_t* const& cstr, kf_int32_t size);
    UniString(const string& str);
    UniString(const wstring& str);
    UniString(const Ptr<UniString>& str);
    ~UniString();
    
    const kf_octet_t* raw() const;
    kf_int32_t getLength() const;
    kf_int32_t getNumberOfOctets() const;
    kf_octet_t getOctetAt(const kf_int32_t& index) const throw(IndexOutOfBoundException);
    wchar_t    getCodePointAt(const kf_int32_t& index) const throw(IndexOutOfBoundException);
    Ptr<UniChar> getCodePointAsUniChar(const kf_int32_t& index) const throw(IndexOutOfBoundException);
    kf_int32_t getIndexOf(const wchar_t& ch) const;
    kf_int32_t getIndexOf(const wchar_t& ch, const kf_int32_t& offset) const;
    bool       isEmpty() const;
    kf_int32_t getHash() const;
    
    bool equals(const Ptr<UniString>& str) const;
    bool equals(const string& str) const;
    bool equals(const wstring& str) const;
    bool equalsIgnoreCases(const UniString& str) const;
    
    void toLowerCase();
    void toUpperCase();
    
    Ptr<UniString> duplicateUniString() const;
    Ptr<UniString> duplicateToLowerCase() const;
    Ptr<UniString> duplicateToUpperCase() const;
    Ptr<UniString> substring(const kf_int32_t& offset, const kf_int32_t& count) const;
    Ptr<UniString> append(const Ptr<UniString>& str) const;
    Ptr<UniString> replace(const wchar_t& a, const wchar_t& b) const;
    
    wstring toWString() const;
    string  toUtf8String() const;
    
    static bool areEqualIgnoreCases(const string& first, const string& second);

    // From Streamer
    void printToStream(ostream& os) const;
    
    // From ManagedObject
    bool equals(const ManagedObject& obj) const;
  };
  
} // namespace kfoundation

#endif
