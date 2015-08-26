
#include <cstdlib>
#include <cstdio>

#include "Int.h"

namespace kfoundation {

  using namespace std;

  /**
   * Constructor. Sets the internal value to the given parameter.
   */
  
  Int::Int(int value)
    : _value(value)
  {
    // Nothing;
  }
  
  
  /**
   * Constructor. Parses the given string and sets the internal value
   * accordingly.
   */
  
  Int::Int(const string& str)
    : _value(parse(str))
  {
    // Nothing;
  }
  
  
  /**
   * Parses the given string to corresponding integer value.
   *
   * @param str The string to be parsed.
   * @return The numeric value extracted from the given string.
   */
    
  int Int::parse(const string& str) {
    return atoi(str.c_str());
  }
  
  
  /**
   * Returns the hexadecimal representation of the given value as a string.
   *
   * @param v The value to convert to hexadecimal.
   * @return The hexadecimal representation of the given value.
   */
  
  string Int::toHexString(const int v) {
    char buffer[8];
    sprintf(buffer, "%x", v);
    return string(buffer);
  }
  
  
  /**
   * Converts the given integer value to string.
   */
  
  string Int::toString(const int v) {
    char buffer[15];
    sprintf(buffer, "%d", v);
    return string(buffer);
  }
  
  void Int::printToStream(ostream& os) const {
    os << _value;
  }
  
  string Int::toString() const {
    return Streamer::toString();
  }
  
} // namespace kfoundation
