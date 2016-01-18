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

// Unix
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>

// Internal
#include "Ref.h"
#include "Path.h"
#include "System.h"
#include "IOException.h"
#include "InputStream.h"

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
  
  FileOutputStream::FileOutputStream(RefConst<Path> path) {
    construct(path);
  }


  FileOutputStream::FileOutputStream(RefConst<UString> path) {
    construct(new Path(path));
  }
  
  
  /**
   * Deconstructor.
   */
  
  FileOutputStream::~FileOutputStream() {
    ::close(_fileDescriptor);
  }
  
  
// --- METHODS --- //

  void FileOutputStream::construct(RefConst<Path> path) {
    _fileDescriptor = open(path->toString()->getCString(), O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
    if(_fileDescriptor == -1) {
      throw IOException(K"Failed to open file: " + *path
          + ". Reason: " + System::getLastSystemError());
    }
    _path = path;
  }


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
      throw IOException(K"Error truncating file " + *_path);
    }
  }
  
  
  void FileOutputStream::write(const kf_octet_t* buffer, const kf_int32_t nBytes)
  {
    ::write(_fileDescriptor, buffer, nBytes);
  }
  
  
  void FileOutputStream::write(kf_octet_t byte) {
    ::write(_fileDescriptor, &byte, 1);
  }
  
  
  void FileOutputStream::write(Ref<InputStream> is) {
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
        throw IOException(K"Error checking lock for file " + *_path
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
      throw IOException(K"Error locking for file " + *_path
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
      throw IOException(K"Error locking file " + *_path + ". Cause: "
          + System::getLastSystemError());
    }
  }
  
  
  void FileOutputStream::close() {
    ::close(_fileDescriptor);
  }
  
  
  /**
   * Returns the path of the file being written by this stream.
   */
  
  RefConst<Path> FileOutputStream::getPath() const {
    return _path;
  }


  void FileOutputStream::flush() {
    // Nothing;
  }
  
  
} // namespace kfoundation
