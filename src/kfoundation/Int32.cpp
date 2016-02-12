// Std
#include <cstdio>

// Internal
#include "Ref.h"
#include "StreamParser.h"
#include "UString.h"
#include "StringInputStream.h"
#include "OutputStream.h"

// Self
#include "Int32.h"

namespace kfoundation {

// --- CONSTRUCTORS --- //

  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Int32::Int32(kf_int32_t value)
  : _value(value)
  {
    // Nothing;
  }
  
  
  /**
   * Constructor. Parses the given string and sets the internal value
   * accordingly.
   */
  
  Int32::Int32(RefConst<UString> str)
  : _value(parse(str))
  {
    // Nothing;
  }


// --- STATIC METHODS --- //

  /**
   * Parses the given string to corresponding integer value.
   *
   * @param str The string to be parsed.
   * @return The numeric value extracted from the given string.
   */
    
  kf_int32_t Int32::parse(RefConst<UString> str) {
    StreamParser parser(new StringInputStream(str));
    long int value;
    parser.readNumber(value);
    return (kf_int32_t)value;
  }


// --- METHODS --- //

  /**
   * Getter method. Returns the internal value.
   */

  int Int32::get() const {
    return _value;
  }


  /**
   * Setter method. Sets the internal value to the given parameter.
   */

  void Int32::set(const kf_int32_t value) {
    _value = value;
  }


  /**
   * Returns the hexadecimal representation of the given value as a string.
   *
   * @param v The value to convert to hexadecimal.
   * @return The hexadecimal representation of the given value.
   */
  
  Ref<UString> Int32::toHexString() const {
    char buffer[50];
    int n = sprintf(buffer, "%X", _value);
    return new UString((kf_octet_t*)buffer, n);
  }
  
  
  /**
   * Converts the given integer value to string.
   */
  
  void Int32::printToStream(Ref<OutputStream> stream) const {
    char buffer[50];
    int n = sprintf(buffer, "%d", _value);
    if(n > 0) {
      stream->write((kf_octet_t*)buffer, n);
    }
  }


  RefConst<UString> Int32::toString() const {
    char buffer[50];
    int n = sprintf(buffer, "%d", _value);
    return new UString((kf_octet_t*)buffer, n);
  }

} // namespace kfoundation
