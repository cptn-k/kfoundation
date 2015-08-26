#ifndef ORG_KNORBA_COMMON_TYPE_DOUBLE_H
#define ORG_KNORBA_COMMON_TYPE_DOUBLE_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {
  
/**
 * Wrapper class for `double` type.
 *
 * @ingroup containers
 * @headerfile Double.h <kfoundation/Double.h>
 */
  
class Double : public ManagedObject, public Streamer {
private:
  double _value;

public:
  Double(const double value);
  inline double get() const;
  inline void set(const double v);
  static double parse(const string& str);
  
  // From Streamer
  void printToStream(ostream& os) const;
};

} // kfoundation

#endif
