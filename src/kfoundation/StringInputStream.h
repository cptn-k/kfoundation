//
//  StringInputStream.h
//  KFoundation
//
//  Created by Kay Khandan on 10/11/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KFoundation__StringInputStream__
#define __KFoundation__StringInputStream__

#include "InputStream.h"

namespace kfoundation {
  
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
  
}

#endif /* defined(__KFoundation__StringInputStream__) */
