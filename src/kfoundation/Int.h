#ifndef ORG_KNORBA_COMMON_TYPE_INTEGER_H
#define ORG_KNORBA_COMMON_TYPE_INTEGER_H 

#include "definitions.h"
#include "RefDecl.h"
#include "Streamer.h"
#include "KFObject.h"

namespace kfoundation {
  
  /**
   * Wrapper for `int` type.
   *
   * @ingroup containers
   * @headerfile Int.h <kfoundation/Int.h>
   */
  
  class Int : public KFObject, public Streamer {

  // --- FIELDS --- //

    private: kf_int32_t _value;


  // --- CONSTRUCTORS --- //

    public: Int(kf_int32_t value);
    public: Int(RefConst<UString> str);


  // --- STATIC METHODS --- //

    public: static kf_int32_t parse(RefConst<UString> str);


  // --- METHODS --- //

    public: kf_int32_t get() const;
    public: void set(const kf_int32_t value);

    public: Ref<UString> toHexString() const;

    // From Streamer
    public: void printToStream(Ref<OutputStream> os) const;
    public: RefConst<UString> toString() const;
    
  };

} // namespace kfoundation

#endif