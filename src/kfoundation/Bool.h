#ifndef ORG_KNORBA_COMMON_TYPE_BOOL_H
#define ORG_KNORBA_COMMON_TYPE_BOOL_H 

#include "RefDecl.h"
#include "Streamer.h"
#include "KFObject.h"

namespace kfoundation {
  
  /**
   * Wrapper class for `bool` type.
   *
   * @ingroup containers
   * @headerfile Bool.h <kfoundation/Bool.h>
   */
  
  class Bool : public KFObject, public Streamer {

  // --- STATIC FIELDS --- //

    public: static const StaticRefConst<UString> TRUE_STR;
    public: static const StaticRefConst<UString> FALSE_STR;
    

  // --- FIELDS --- //

    private: bool _value;


  // --- CONSTRUCTORS --- //

    public : Bool(bool value);


  // --- METHODS --- //

    public: bool get() const;
    public: void set(const bool value);

    // From Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };

} // namespace kfoundation

#endif