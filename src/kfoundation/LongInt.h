#ifndef ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H
#define ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {

  /**
   * Wrapper class for 'long int' type.
   *
   * @ingroup containers
   * @headerfile LongInt.h <kfoundation/LongInt.h>
   */
  
  class LongInt : public ManagedObject, public Streamer {
  private:
    long int _value;

  public:
    
    /**
     * Numeral Base
     */
    
    typedef enum {
      DECIMAL,    ///< Base 10
      HEXADECIMAL ///< Base 16
    } encoding_t;
    
    LongInt(const long int value);
    LongInt(const string& str, const encoding_t& encoding = DECIMAL);
    
    inline long int get() const;
    inline void set(const long int v);
    
    static long int parse(const string& str, const encoding_t encoding = DECIMAL);
    static string toHexString(const long int v);
    static string toHexString(void* ptr);
    static string toString(const long int v);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };
  
} // namespace kfoundation

#endif