#ifndef ORG_KNORBA_COMMON_TYPE_INTEGER_H
#define ORG_KNORBA_COMMON_TYPE_INTEGER_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {
  
  /**
   * Wrapper for `int` type.
   *
   * @ingroup containers
   * @headerfile Int.h <kfoundation/Int.h>
   */
  
  class Int : public ManagedObject, public Streamer {
  private:
    int _value;

  public:
    Int(int _value);
    Int(const string& str);
    
    inline int get() const;
    inline void set(const int value);
    
    static int parse(const string& str);
    static string toHexString(const int v);
    static string toString(const int v);
    
    // From Streamer
    void printToStream(ostream& os) const;
    string toString() const;
    
  };
  
  
  /**
   * Getter method. Returns the internal value.
   */
  
  inline int Int::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given parameter.
   */
  
  inline void Int::set(const int value) {
    _value = value;
  }

} // namespace kfoundation

#endif
