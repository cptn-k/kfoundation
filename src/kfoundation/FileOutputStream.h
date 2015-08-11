/*---[FileOutputStream.h]--------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::FileOutputStream::*
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

#ifndef __KFoundation__FileOutptuStream__
#define __KFoundation__FileOutptuStream__

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  using namespace std;
  
  class Path;
  
  /**
   * Output stream used to write data on file.
   * 
   * @ingroup io
   * @headerfile FileOutputStream.h <kfoundation/FileOutputStream.h>
   */
  
  class FileOutputStream : public OutputStream {
    
  // --- FIELDS ---- //
    
    private: int _fileDescriptor;
    private: Ptr<Path> _path;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: FileOutputStream(PPtr<Path> path);
    public: ~FileOutputStream();
    
    
  // --- METHODS --- //
    
    public: bool isLocked() const;
    public: void lock() const;
    public: void unlock() const;
    public: void close();
    public: PPtr<Path> getPath() const;
    
    // Inhertied from OutputStream
    public: bool isBigEndian() const;
    public: void truncate() const;
    public: void write(const kf_octet_t* buffer, const kf_int32_t nBytes);
    public: void write(kf_octet_t byte);
    public: void write(PPtr<InputStream> is);
    
  };
  
}

#endif /* defined(__KFoundation__FileOutptuStream__) */
