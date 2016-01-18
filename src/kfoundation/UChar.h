#ifndef ORG_KNORBA_COMMON_TYPE_CHARACTER_H
#define ORG_KNORBA_COMMON_TYPE_CHARACTER_H 

// Internal
#include "definitions.h"

// Super
#include "Streamer.h"
#include "KFObject.h"


namespace kfoundation {

  class UString;
  class InputStream;


  /**
   * Wrapper class for unicode character. Uses a 4-byte internal represenation.
   *
   * @ingroup containers
   * @headerfile UChar.h <kfoundation/UChar.h>
   */
  
  class UChar : public Streamer, public KFObject {

  // --- FIELDS --- //

    private: kf_octet_t _octets[6];


  // --- CONSTRUCTORS --- //

    public: UChar();
    public: UChar(const kf_octet_t* utf);
    public: UChar(const char ascii);
    public: UChar(const wchar_t unicode);


  // --- STATIC METHODS --- //

    public: static kf_int8_t getNumberOfUtf8Octets(const kf_octet_t firstOctet);
    public: static kf_int8_t getNumberOfUtf8Octets(const wchar_t ch);
    public: static kf_int8_t writeUtf8(const wchar_t ch, kf_octet_t* buffer);
    public: static kf_int8_t readUtf8(const unsigned char* buffer, wchar_t& output);
    public: static kf_int8_t readUtf8(const Ref<InputStream>& stream, wchar_t& output, kf_octet_t* readOctets);
    public: static bool isLowerCase(const wchar_t ch);
    public: static bool isUpperCase(const wchar_t ch);
    public: static bool isNumeric(const wchar_t ch);
    public: static bool isAlpabet(const wchar_t ch);
    public: static bool isAlphanumeric(const wchar_t ch);
    public: static bool isWhiteSpace(const wchar_t ch);
    public: static wchar_t toLowercase(const wchar_t ch);
    public: static wchar_t toUppercase(const wchar_t ch);


  // --- METHODS --- //

    public: inline const kf_octet_t* get() const;
    public: wchar_t toWChar() const;
    public: void set(const wchar_t value);
    public: kf_int8_t read(const kf_octet_t* value);
    public: kf_int8_t read(Ref<InputStream> stream);
    public: kf_int8_t write(kf_octet_t* buffer) const;
    public: inline kf_int8_t getOctetCount() const;
    public: bool isLowerCase() const;
    public: bool isUpperCase() const;
    public: bool isNumeric() const;
    public: bool isAlphabet() const;
    public: bool isAlphanumeric() const;
    public: bool isWhiteSpace() const;
    public: UChar toLowercase() const;
    public: UChar toUppercase() const;
    public: bool equals(const UChar ch) const;

    // From Streamer
    public: void printToStream(Ref<OutputStream> os) const;
    public: RefConst<UString> toString() const;

  };


  inline const kf_octet_t* UChar::get() const {
    return _octets;
  }


  inline kf_int8_t UChar::getOctetCount() const {
    return _octets[5];
  }

} // namespace kfoundation

#endif