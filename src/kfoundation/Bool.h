#ifndef ORG_KNORBA_COMMON_TYPE_BOOL_H
#define ORG_KNORBA_COMMON_TYPE_BOOL_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {
  
  /**
   * Wrapper class for `bool` type.
   *
   * @ingroup containers
   * @headerfile Bool.h <kfoundation/Bool.h>
   */
  
  class Bool : public ManagedObject, public Streamer {
  private:
    bool _value;

  public:
    Bool(bool value);
    inline bool get() const;
    inline void set(const bool value);
    
    static string toString(const bool value);
    
    // From Streamer
    void printToStream(ostream& os) const;
  };

} // namespace kfoundation

#endif
