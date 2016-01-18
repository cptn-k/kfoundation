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

#include "RefDecl.h"
#include "InputStream.h"

namespace kfoundation {

  class UString;

  /**
   * Input stream to read from string.
   *
   * @ingroup io
   * @headerfile StringInputStream.h <kfoundation/StringInputStream.h>
   */
  
  class StringInputStream : public InputStream {

  // --- FIELDS --- //

    private: RefConst<UString> _str;
    private: const kf_octet_t* _pos;
    private: const kf_octet_t* _end;
    private: const kf_octet_t* _mark;


  // --- (DE)CONSTRUCTORS --- //
    
    public: StringInputStream(RefConst<UString> str);


  // --- METHODS --- //

    // From InputStream
    public: kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes);
    public: kf_int16_t read();
    public: kf_int16_t peek();
    public: kf_int32_t skip(kf_int32_t bytes);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();

  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__StringInputStream__) */