/*---[Streamer.h]----------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common / Streamer
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

#ifndef ORG_KNORBA_COMMON_STRINGSTREAMER
#define ORG_KNORBA_COMMON_STRINGSTREAMER

namespace kfoundation {

  template<typename T> class RefConst;
  template<typename T> class Ref;

  class OutputStream;
  class UString;

  /**
   * Base class for all classes that can print information about themeselves
   * to a `std::ostream`. Subclasses should implement `printToStream(ostream&)`
   * pure virtual function. This class defines and implements `toString()` 
   * function which internally invokes `printToStream(ostream&)` feeding it with
   * `std::stringstream`.
   *
   * @see operator<<(ostream&, const Streamer&)
   * @see operator+(const string&, const Streamer&)
   * @see operator+(const Streamer&, const string&)
   * @ingroup io
   * @headerfile Streamer.h <kfoundation/Streamer.h>
   */
  
  class Streamer {
    
   /**
    * Implements compatibility with Streamer interface.
    */
    
    public: virtual void printToStream(Ref<OutputStream> writer) const = 0;
    public: virtual RefConst<UString> toString() const;

  };

} // namespace kfoundation

#endif