#ifndef ORG_KNORBA_COMMON_TYPE_DOUBLE_H
#define ORG_KNORBA_COMMON_TYPE_DOUBLE_H 

#include "Streamer.h"
#include "KFObject.h"

namespace kfoundation {

  class UString;

  /**
   * Wrapper class for `double` type.
   *
   * @ingroup containers
   * @headerfile Double.h <kfoundation/Double.h>
   */
    
  class Double : public KFObject, public Streamer {

  // --- FIELDS --- //

    private: double _value;


  // --- CONSTRUCTOR --- //

    public: Double(const double value);
    public: Double(RefConst<UString> str);


  // --- STATIC METHODS --- //

    public: static double parse(RefConst<UString> str);


  // --- METHODS --- //

    public: double get() const;
    public: void set(const double v);


    // From Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };

} // kfoundation

#endif
