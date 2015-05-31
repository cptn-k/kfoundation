//
//  FileInputStream.h
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KFoundation_XCode_Wrapper__FileInputStream__
#define __KFoundation_XCode_Wrapper__FileInputStream__

#include <fstream>

#include "InputStream.h"

using namespace std;

namespace kfoundation {
  
  class Path;
  
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
