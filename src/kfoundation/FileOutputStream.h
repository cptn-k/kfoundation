//
//  FileOutptuStream.h
//  KFoundation
//
//  Created by Kay Khandan on 11/2/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__FileOutptuStream__
#define __KFoundation__FileOutptuStream__

// Super
#include "OutputStream.h"

namespace kfoundation {
  
  using namespace std;
  
  class Path;
  
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
