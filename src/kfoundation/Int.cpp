
#include <cstdlib>
#include <cstdio>

#include "Int.h"

namespace kfoundation {

  using namespace std;

  Int::Int(int value)
    : _value(value)
  {
    // Nothing;
  }
  
  Int::Int(const string& str)
    : _value(parse(str))
  {
    // Nothing;
  }
    
  int Int::parse(const string& str) {
    return atoi(str.c_str());
  }
  
  string Int::toHexString(const int v) {
    char buffer[8];
    sprintf(buffer, "%x", v);
    return string(buffer);
  }
  
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
  
  bool operator==(Int& a, Int& b) {
    return a.get() == b.get();
  }
  
} // namespace kfoundation
