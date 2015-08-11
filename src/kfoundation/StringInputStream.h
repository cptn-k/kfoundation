/*---[StringInputStream.h]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::StringInputStream::*
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

#ifndef __KFoundation__StringInputStream__
#define __KFoundation__StringInputStream__

#include "InputStream.h"

namespace kfoundation {
  
  /**
   * Input stream to read from string.
   *
   * @ingroup io
   * @headerfile StringInputStream.h <kfoundation/StringInputStream.h>
   */
  
  class StringInputStream : public InputStream {
  private:
    string _str;
    kf_int32_t _pos;
    kf_int32_t _mark;
    bool _eof;
    
  public:
    StringInputStream(const string& str);
    ~StringInputStream();
    
    // From InputStream
    kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes);
    int read();
    int peek();
    kf_int32_t skip(kf_int32_t bytes);
    bool isEof();
    bool isMarkSupported();
    void mark();
    void reset();
    bool isBigEndian();
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__StringInputStream__) */
