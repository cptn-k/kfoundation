/*---[FileInputStream.cpp]-------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::FileInputStream::*
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// Internal
#include "UString.h"
#include "Path.h"
#include "Ref.h"
#include "IOException.h"

// Self
#include "FileInputStream.h"

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor, opens the file pointed by the given Path object.
   *
   * @param path The path to the file to open.
   */
  
  FileInputStream::FileInputStream(RefConst<Path> path) {
    construct(path);
  }

  
  /**
   * Constructor, opens file pointed by the given string to read.
   *
   * @param fileName The path to the file to open.
   */
  
  FileInputStream::FileInputStream(RefConst<UString> path) {
    construct(new Path(path));
  }
  
  
  /**
   * Deconstructor.
   */
  
  FileInputStream::~FileInputStream() {
    if(isOpen()) {
      close();
    }
  }


// --- METHODS --- //

  void FileInputStream::construct(RefConst<Path> path) {
    _fileDescriptor = ::open(path->toString()->getCString(), O_RDONLY);
    if(_fileDescriptor == -1) {
      throw IOException(K"Unable to open " + *path + ". Reason: "
          + System::getLastSystemError());
    }

    mark();

    if(read() == 0) {
      _mark = -1;
    } else {
      reset();
    }
  }


  /**
   * Checks if the file is open.
   */
  
  bool FileInputStream::isOpen() const {
    return _fileDescriptor == KF_NOT_FOUND;
  }
  
  
  /**
   * Closes the file.
   */
  
  void FileInputStream::close() {
    ::close(_fileDescriptor);
  }


  /**
   * Checks if the file is locked by another process.
   *
   * @see lock()
   * @see unlock()
   */

  bool FileInputStream::isLocked() const {
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
   * Puts a lock on this file, blocking access from other processes until
   * unlockced.
   *
   * @see unlock()
   * @see isLocked()
   */

  void FileInputStream::lock() const {
    if(flock(_fileDescriptor, LOCK_EX) == -1) {
      throw IOException(K"Error locking for file " + *_path
          + ". Cause: " + System::getLastSystemError());
    }
  }


  /**
   * Unlocks the this file if it is locked by this process.
   *
   * @see lock()
   * @see isLocked()
   */

  void FileInputStream::unlock() const {
    if(flock(_fileDescriptor, LOCK_UN) == -1) {
      throw IOException(K"Error locking file " + *_path + ". Cause: "
          + System::getLastSystemError());
    }
  }
  
  
  kf_int32_t FileInputStream::read(kf_octet_t *buffer, const kf_int32_t nOctets)
  {
    kf_int32_t nRead = (kf_int32_t)::read(_fileDescriptor, buffer, nOctets);

    if(nRead == -1) {
      throw IOException(K"Error reading from file " + *_path + ". Reason: "
          + System::getLastSystemError());
    } else if(nRead < nOctets) {
      _mark = -1;
    }

    return nRead;
  }
  
  
  kf_int16_t FileInputStream::read() {
    kf_octet_t value;
    if(read(&value, 1) == 0) {
      return -1;
    }
    return value;
  }
  
  
  kf_int16_t FileInputStream::peek() {
    kf_octet_t value;
    if(read(&value, 1) == 0) {
      return -1;
    };
    lseek(_fileDescriptor, -1, SEEK_CUR);
    return value;
  }
  
  
  kf_int32_t FileInputStream::skip(kf_int32_t nBytes) {
    kf_int32_t nSkipped = (kf_int32_t)lseek(_fileDescriptor, nBytes, SEEK_CUR);
    if(nSkipped < nBytes) {
      _mark = -1;
    }
    return nSkipped;
  }
  
  
  bool FileInputStream::isEof() {
    return _mark == -1;
  }
  
  
  bool FileInputStream::isMarkSupported() {
    return true;
  }
  
  
  void FileInputStream::mark() {
    _mark = lseek(_fileDescriptor, 0, SEEK_CUR);
  }
  
  
  void FileInputStream::reset() {
    lseek(_fileDescriptor, _mark, SEEK_SET);
  }
  
  
  bool FileInputStream::isBigEndian() {
    return System::isBigEndian();
  }
  
} // namespace kfoundation