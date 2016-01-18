/*---[OutputStream.h]------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::OutputStream::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef KFoundation_OutputStream_h
#define KFoundation_OutputStream_h

#include "definitions.h"
#include "KFObject.h"
#include "RefDecl.h"

namespace kfoundation {
  
  class InputStream;
  
  /**
   * Abstract inferface for all output streams.
   *
   * @ingroup io
   * @headerfile OutputStream.h <kfoundation/OutputStream.h>
   */
  
  class OutputStream : public KFObject {
    
    
    /**
     * Writes the given number of octets of the given buffer to the stream.
     *
     * @param buffer The octets to write.
     * @param nOctets Number of octets to write.
     */
    
    public: virtual void write(const kf_octet_t* buffer, const kf_int32_t nOctets) = 0;
    
    
    /**
     * Writes a single octet to the stream.
     *
     * @param octet The octet to write
     */
    
    public: virtual void write(const kf_octet_t octet) = 0;
    
    
    /**
     * Writes the available contents from the given input stream to this
     * output stream.
     *
     * @param is The stream to read from.
     */
    
    public: virtual void write(Ref<InputStream> is) = 0;
    
    
    /**
     * Closes the stream. It will no longer be readable.
     */
    
    public: virtual void close() = 0;


    /**
     * Flushes the output.
     */

    public: virtual void flush() = 0;
    
    
    /**
     * Checks if the stream is big-endian.
     */
    
    public: virtual bool isBigEndian() const = 0;
    
  };
  
} // namespace kfoundation


#endif