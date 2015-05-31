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

/**
 *
 * @file 
 * Defines the interface of `kfoundation::Streamer` class
 *
 */

#ifndef ORG_KNORBA_COMMON_STRINGSTREAMER
#define ORG_KNORBA_COMMON_STRINGSTREAMER

#include <ostream>
#include <string>


namespace kfoundation {

  using namespace std;
  
  /**
   * Base class for all classes that can print information about themeselves
   * to a `std::ostream`. Subclasses should implement `printToStream(ostream&)`
   * pure virtual function. This class defines and implements `toString()` 
   * function which internally invokes `printToStream(ostream&)` feeding it with
   * `std::stringstream`.
   *
   * @see operator<<(ostream&, Streamer&)
   */
  class Streamer {
    
   /**
    * Prints information related to the implementing class into the given
    * output stream.
    */
    public: virtual void printToStream(ostream& stream) const = 0;

   /**
    * Converts the result of invocation of `printToStream(ostream&)` to a
    * `std::string` object. 
    */
    public: virtual string toString() const;
  };

  
  /**
   * Overloads `<<` operator so that any `Streamer` object can be directly
   * used within standard `ostream` object like `cout`.
   */
  inline ostream& operator<<(ostream& os, const Streamer& streamer) {
    streamer.printToStream(os);
    return os;
  }

  
  inline string operator+(const string& lhs, const Streamer& rhs) {
    return lhs + rhs.toString();
  }

  
  inline string operator+(const Streamer& lhs, const string& rhs) {
    return lhs.toString().append(rhs);
  }

} // namespace kfoundation

#endif