/*---[FileInputStream.h]---------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::FileInputStream::*
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

#ifndef __KFoundation_XCode_Wrapper__FileInputStream__
#define __KFoundation_XCode_Wrapper__FileInputStream__

// Internal
#include "RefDecl.h"

// Super
#include "InputStream.h"

namespace kfoundation {
  
  class Path;
  class UString;

  /**
   * Input stream to read from file.
   *
   * @ingroup io
   * @headerfile FileInputStream.h <kfoundation/FileInputStream.h>
   */
  
  class FileInputStream : public InputStream {

  // --- FIELDS --- //

    private: Ref<Path>  _path;
    private: kf_int32_t _fileDescriptor;
    private: kf_int64_t _mark;


  // --- (DE)CONSTRUCTORS --- //

    public: FileInputStream(RefConst<Path> path);
    public: FileInputStream(RefConst<UString> path);
    public: ~FileInputStream();


  // --- METHODS --- //

   private: void construct(RefConst<Path> path);
    public: bool isOpen() const;
    public: void close();
    public: bool isLocked() const;
    public: void lock() const;
    public: void unlock() const;
    
    // From InputStream
    public: kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nOctets);
    public: kf_int16_t read();
    public: kf_int16_t peek();
    public: kf_int32_t skip(kf_int32_t nOctets);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation_XCode_Wrapper__FileInputStream__) */