#ifndef ORG_KNORBA_COMMON_TYPE_DOUBLE_H
#define ORG_KNORBA_COMMON_TYPE_DOUBLE_H 

#include <ostream>

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {
  
class Double : public ManagedObject, public Streamer {
private:
  double _value;

public:
  Double(double value);
  inline double get() const;
  inline void set(double v);
  static double parse(const string& str);
  
  // From Streamer
  void printToStream(ostream& os) const;
};

} // kfoundation

#endif
