/*---[FileOutputStream.cpp]------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::FileOutputStream::*
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
  
  /**
   * Constructor, opens the file pointed by the given path object to write. 
   * If the file
   * already exists, the writting begins from the end of the file. If it is 
   * desired to earase the existing contents of the file, use truncate() method.
   *
   * @param path Path to the file to be opened.
   * @see truncate()
   */
  
  FileOutputStream::FileOutputStream(PPtr<Path> path) {
    _fileDescriptor = open(path->getString().c_str(), O_WRONLY | O_CREAT,
                           S_IWUSR | S_IRUSR);
    _path = path;
    if(_fileDescriptor == -1) {
      throw IOException("Failed to open file: " + path->getString()
                        + ". Reason: " + System::getLastSystemError());
    }
  }
  
  
  /**
   * Deconstructor.
   */
  
  FileOutputStream::~FileOutputStream() {
    ::close(_fileDescriptor);
  }
  
  
// --- METHODS --- //
  
  bool FileOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  /**
   * Earases the file contents and resets the stream position to the begining
   * of the file.
   *
   * @throw Throws IOException if the operation failed.
   */
  
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
  
  
  /**
   * Checks if a lock is placed on the file to be read by this stream.
   * Locking mechanism is used to prevent write by more than one process.
   *
   * @see lock()
   * @see unlock()
   * @throw Throws IOException if lock status could not be obtained.
   */
  
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
  
  
  /**
   * Places a lock on the file to be read by this stream.
   * Locking mechanism is used to prevent write by more than one process.
   *
   * @see isLocked()
   * @see unlock()
   * @throw Throws IOException if lock could not be placed.
   */

  void FileOutputStream::lock() const {
    if(flock(_fileDescriptor, LOCK_EX) == -1) {
      throw IOException("Error locking for file " + _path->getString()
                        + ". Cause: " + System::getLastSystemError());
    }
  }

  
  /**
   * Removes the lock placed on this file.
   * Locking mechanism is used to prevent write by more than one process.
   *
   * @see isLocked()
   * @see lock()
   * @throw Throw IOException if lock could not be removed.
   */
  
  void FileOutputStream::unlock() const {
    if(flock(_fileDescriptor, LOCK_UN) == -1) {
      throw IOException("Error locking for file " + _path->getString()
                        + ". Cause: " + System::getLastSystemError());
    }
  }
  
  
  void FileOutputStream::close() {
    ::close(_fileDescriptor);
  }
  
  
  /**
   * Returns the path of the file being written by this stream.
   */
  
  PPtr<Path> FileOutputStream::getPath() const {
    return _path;
  }
  
  
} // namespace kfoundation
