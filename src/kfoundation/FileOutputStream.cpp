//
//  FileOutptuStream.cpp
//  KFoundation
//
//  Created by Kay Khandan on 11/2/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

// Std
#include <fstream>

// Unix
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>

// Internal
#include "Ptr.h"
#include "Path.h"
#include "System.h"
#include "IOException.h"

// Self
#include "FileOutputStream.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  FileOutputStream::FileOutputStream(PPtr<Path> path) {
    _fileDescriptor = open(path->getString().c_str(), O_WRONLY | O_CREAT,
                           S_IWUSR | S_IRUSR);
    _path = path;
    if(_fileDescriptor == -1) {
      throw IOException("Failed to open file: " + path->getString()
                        + ". Reason: " + System::getLastSystemError());
    }
  }
  
  
  FileOutputStream::~FileOutputStream() {
    ::close(_fileDescriptor);
  }
  
  
// --- METHODS --- //
  
  bool FileOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  void FileOutputStream::truncate() const {
    if(ftruncate(_fileDescriptor, 0) == -1) {
      throw IOException("Error truncating file " + _path->getString());
    }
  }
  
  
  void FileOutputStream::write(const kf_octet_t* buffer, const kf_int32_t nBytes)
  {
    ::write(_fileDescriptor, buffer, nBytes);
  }
  
  
  void FileOutputStream::write(kf_octet_t byte) {
    ::write(_fileDescriptor, &byte, 1);
  }
  
  
  void FileOutputStream::write(PPtr<InputStream> is) {
    kf_octet_t tmp[1024];
    while(!is->isEof()) {
      kf_int32_t s = is->read(tmp, 1024);
      write(tmp, s);
    }
  }
  
  
  bool FileOutputStream::isLocked() const {
    if(flock(_fileDescriptor, LOCK_EX | LOCK_NB) == -1) {
      if(errno == EWOULDBLOCK) {
        return true;
      } else {
        throw IOException("Error checking lock for file " + _path->getString()
                          + ". Cause: " + System::getLastSystemError());
      }
    }
    unlock();
    return false;
  }
  
  
  void FileOutputStream::lock() const {
    if(flock(_fileDescriptor, LOCK_EX) == -1) {
      throw IOException("Error locking for file " + _path->getString()
                        + ". Cause: " + System::getLastSystemError());
    }
  }
  
  
  void FileOutputStream::unlock() const {
    if(flock(_fileDescriptor, LOCK_UN) == -1) {
      throw IOException("Error locking for file " + _path->getString()
                        + ". Cause: " + System::getLastSystemError());
    }
  }
  
  
  void FileOutputStream::close() {
    ::close(_fileDescriptor);
  }
  
  
  PPtr<Path> FileOutputStream::getPath() const {
    return _path;
  }
  
  
} // namespace kfoundation
