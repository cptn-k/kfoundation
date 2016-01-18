#ifndef ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H
#define ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H 

// Super
#include "Streamer.h"
#include "KFObject.h"

namespace kfoundation {

  class OutputStream;
  class UString;

  /**
   * Wrapper class for 'long int' type.
   *
   * @ingroup containers
   * @headerfile LongInt.h <kfoundation/LongInt.h>
   */
  
  class LongInt : public KFObject, public Streamer {

  // --- FIELDS --- //

    private: kf_int64_t _value;


  // --- CONSTRUCTORS --- //

    public: LongInt(const kf_int64_t value);
    public: LongInt(RefConst<UString> str);


  // --- STATIC METHODS --- //

    public: static kf_int64_t parse(RefConst<UString> str);


  // --- METHODS --- //
    
    public: kf_int64_t get() const;
    public: void set(const kf_int64_t v);
    public: Ref<UString> toHexString() const;

    // From Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };
  
} // namespace kfoundation

#endif