/*---[InputStream.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::InputStream::*
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


#ifndef KFOUNDATION_INPUTSTREAM
#define KFOUNDATION_INPUTSTREAM

#include "definitions.h"
#include "KFObject.h"
#include "RefDecl.h"

namespace kfoundation {
  
  /**
   * Abstract interface for all input streams.
   *
   * @ingroup io
   * @headerfile InputStream.h <kfoundation/InputStream.h>
   */
  
  class InputStream : public KFObject {
    
    /**
     * Reads at most the given number of octets from the given buffer.
     * Returns the actual number of octets read.
     *
     * @param buffer The octets to read.
     * @param nOctets Maximum number of octets to read.
     * @return The actual number of octets read.
     */
    
    public: virtual kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nOctets) = 0;
    
    
    /**
     * Reads a single octet.
     *
     * @return The value of read octet, or -1 if no data is available.
     */
    
    public: virtual kf_int16_t read() = 0;
    
    
    /**
     * Reads a single octet without advancing.
     *
     * @return The value of read octet, or -1 if no data is available.
     */
    
    public: virtual kf_int16_t peek() = 0;
    
    
    /**
     * Skips the at most the given number of octets without reading them.
     * If there are less number of octets in the stream, all available octets
     * will be skipped.
     *
     * @param nOctets The desired number of octets to skeep.
     * @return The actual number of octets skipped.
     */
    
    public: virtual kf_int32_t skip(const kf_int32_t nOctets) = 0;
    
    
    /**
     * Checks if the end of stream is reached.
     */
    
    public: virtual bool isEof() = 0;
    
    
    /**
     * If returns true, mark() and reset() methods can be used.
     * @see mark()
     * @see reset()
     */
    
    public: virtual bool isMarkSupported() = 0;
    
    
    /**
     * Marks the current position of the stream. Use reset() to return to this
     * position later and read the data again.
     *
     * @see isMarkSupported()
     * @see reset()
     */
    
    public: virtual void mark() = 0;
    
    
    /**
     * Returns the stream position to where mark() was used last time. If 
     * mark() is never called, it will reset to the begining of the stream.
     *
     * @see isMarkSupported()
     * @see mark()
     */
    
    public: virtual void reset() = 0;
    
    
    /**
     * Checks if the stream is big-endian.
     */
    
    public: virtual bool isBigEndian() = 0;
    
  };
  
  
} // namespace kfoundation

#endif /* defined(KFOUNDATION_INPUTSTREAM) */