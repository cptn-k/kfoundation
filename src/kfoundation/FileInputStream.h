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

#include <fstream>

#include "InputStream.h"

using namespace std;

namespace kfoundation {
  
  class Path;

  /**
   * Input stream to read from file.
   *
   * @ingroup io
   * @headerfile FileInputStream.h <kfoundation/FileInputStream.h>
   */
  
  class FileInputStream : public InputStream {
  private:
    ifstream* _ifs;
    streampos _mark;
    kf_int64_t _size;
    
  public:
    FileInputStream(PPtr<Path> path);
    FileInputStream(const string& fileName);
    virtual ~FileInputStream();
    
    kf_int64_t getSize() const;
    bool isOpen() const;
    void close();
    
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


#endif /* defined(__KFoundation_XCode_Wrapper__FileInputStream__) */
