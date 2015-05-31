#ifndef ORG_KNORBA_COMMON_TYPE_CHARACTER_H
#define ORG_KNORBA_COMMON_TYPE_CHARACTER_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"
#include "InputStream.h"

using namespace std;

namespace kfoundation {
  
  class UniChar : public ManagedObject, public Streamer {
  private:
    wchar_t _value;

  public:
    UniChar(const char value);
    UniChar(const wchar_t value);
    UniChar(Ptr<UniChar> value);
    
    inline wchar_t get() const;
    inline void set(const wchar_t value);
    
    unsigned short int getNumberOfUtf8Octets() const;
    unsigned short int writeUtf8(kf_octet_t* buffer) const;
    unsigned short int readUtf8(kf_octet_t* buffer);
    unsigned short int readUtf8(Ptr<InputStream> stream);
    bool isLowerCase() const;
    bool isUpperCase() const;
    bool isNumeric() const;
    bool isAlphabet() const;
    bool isAlphanumeric() const;
    bool isWhiteSpace() const;
    void toLowerCase();
    void toUpperCase();
    
    Ptr<UniChar> duplicateToLowerCase() const;
    Ptr<UniChar> duplicateToUpperCase() const;
    Ptr<UniChar> duplicateUniChar() const;
    
    static unsigned short int getNumberOfUtf8Octets(const wchar_t ch);
    static unsigned short int writeUtf8(const wchar_t ch, kf_octet_t* buffer);
    static unsigned short int readUtf8(const unsigned char* buffer, wchar_t& output);
    static unsigned short int readUtf8(const Ptr<InputStream>& stream, wchar_t& output, kf_octet_t* readOctets);
    static bool isLowerCase(const wchar_t ch);
    static bool isUpperCase(const wchar_t ch);
    static bool isNumeric(const wchar_t ch);
    static bool isAlpabet(const wchar_t ch);
    static bool isAlphanumeric(const wchar_t ch);
    static bool isWhiteSpace(const wchar_t ch);
    static wchar_t toLowerCase(const wchar_t ch);
    static wchar_t toUpperCase(const wchar_t ch);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };

} // namespace kfoundation

#endif
