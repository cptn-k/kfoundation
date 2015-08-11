/*---[Path.cpp]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Path::*
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
#include <cstdlib>

// Unix
#include <unistd.h>
#include <sys/stat.h>

// Internal
#include "definitions.h"
#include "Path.h"
#include "Ptr.h"
#include "Array.h"
#include "IOException.h"


namespace kfoundation {
  
  using namespace std;

  /**
   * Path separator symbol on the current platform.
   */
  
#if defined(KF_UNIX) || defined(__doxygen__)
  const char Path::PATH_SEPARATOR = '/';
#else
#  error "Unsupported platform"
#endif
  
  
  /**
   * Constructor.
   *
   * @param str String containing the desired path. It can be empty.
   */
  
  Path::Path(const string& str) {
    _str = str;
    _segments = new Array<int>();
    refresh();
  }
  
  
  /**
   * Deconstructor.
   */
  
  Path::~Path() {
    // Nothing;
  }
  
  
  void Path::refresh() {
    _segments->clear();
    _extention = -1;
    const int s = (int)_str.length();

    _isAbsolute = (_str[0] == PATH_SEPARATOR);
    
    for(int i = 0; i < s; i++) {
      if(_str[i] == PATH_SEPARATOR) {
        _segments->push(i);
        _extention = -1;
      } else if(_str[i] == '.') {
        _extention = i;
      }
    }
    
    _segments->push(s);
  }
  
  
  /**
   * Checks if the path has extention. That is, if the last segment of the path
   * contatins a dot '.'.
   */
  
  bool Path::hasExtention() const {
    return _extention != -1;
  }
  
  
  /**
   * Checks if the path is absolute. That is, if it begins with a separator
   * character.
   */
  
  bool Path::isAbsolute() const {
    return _isAbsolute;
  }
  
  
  /**
   * Returns the number of segments in the path.
   */
  
  int Path::getNSegments() const {
    return (int)_segments->getSize();
  }
  
  
  /**
   * Returns the path segment at the given index.
   */
  
  string Path::getSegement(int index) const {
    int begin = (index == 0)?0:(_segments->at(index - 1) + 1);
    int end = _segments->at(index);
    
    if(end == -1) {
      return "/";
    }
    
    return _str.substr(begin, end - begin);
  }
  
  
  /**
   * Returns the last segment of the path excluding the extension if exists.
   */
  
  string Path::getFileName() const {
    int s = _segments->getSize() - 1;
    int begin = (s == 0)?0:(_segments->at(s - 1) + 1);
    int end = _segments->at(s);
    if(_extention > -1) {
      end = _extention;
    }
    
    return _str.substr(begin, end - begin);
  }
  
  
  /**
   * Returns the extension if exists, otherwise, an empty string.
   */
  
  string Path::getExtention() const {
    if(!hasExtention()) {
      return "";
    }
    
    return _str.substr(_extention + 1, _str.size() - _extention - 1);
  }
  
  
  /**
   * Returns the last segment of the path including extention.
   */
  
  string Path::getFileNameWithExtension() const {
    return getSegement(getNSegments() - 1);
  }
  
  
  /**
   * Add a segment to the path.
   */
  
  Ptr<Path> Path::addSegement(const string &s) {
    string str = _str + PATH_SEPARATOR + s;
    return new Path(str);
  }
  
  
  /**
   * Changes the file extensions. Adds one if it does not exist.
   */
  
  Ptr<Path> Path::changeExtension(const string& ex) {
    if(hasExtention()) {
      string str = _str.substr(0, _extention + 1) + ex;
      return new Path(str);
    }
    
    string str = _str + '.' + ex;
    return new Path(str);
  }
  
  
  /**
   * Removes the extension.
   */
  
  Ptr<Path> Path::removeExtension() {
    if(hasExtention()) {
      string str = _str.substr(0, _extention);
      return new Path(str);
    }
    
    return getPtr().AS(Path).retain();
  }
  
  
  /**
   * Returns the parent path of the current path by removing its last segment.
   * If the path has only one segment or is empty, returns an empty path.
   */
  
  Ptr<Path> Path::parent() {
    int s = _segments->getSize();
    
    if(s <= 1) {
      return new Path("");
    }
    
    string str = _str.substr(0, _segments->at(s - 2));
    return new Path(str);
  }
  
  
  /**
   * Makes a new directory at the path encoded by this object.
   */
  
  void Path::makeDir() const {
#ifdef KF_UNIX
    
    if(mkdir(_str.c_str(), S_IRWXU) == -1) {
      throw IOException("Error making directory " + _str + ". Reason: "
                        + System::getLastSystemError());
    };
    
#else
#  error "Platform not supported."
#endif
  }
  
  
  /**
   * Checks if a file or directory pointed by this path object exists.
   */
  
  bool Path::exists() const {
#ifdef KF_UNIX
    return access(_str.c_str(), F_OK) != -1;
#else
#  error "Platform not supported."
#endif
  }
  
  
  /**
   * Deletes the file or directory pointed by this path object.
   */
  
  void Path::remove() const {
#ifdef KF_UNIX
    if(::remove(_str.c_str()) == -1) {
      throw IOException("Error removing directory " + _str + ". Reason: "
                        + System::getLastSystemError());
    }
#else
#  error "Platform not supported."
#endif
  }
  
  
  /**
   * Returns the string value of this pat.
   */
  
  const string& Path::getString() const {
    return _str;
  }

  
  void Path::printToStream(ostream& os) const {
    os << _str;
  }
  
  
  void Path::serialize(PPtr<ObjectSerializer> builder) const {
    string segments;    
    int s = getNSegments();
    for(int i = 0; i < s; i++) {
      segments += "[" + getSegement(i) + "]";
    }
    
    builder->object("Path")
      ->attribute("segments", segments)
      ->attribute("name", getFileName())
      ->attribute("extension", getExtention())
      ->endObject();
  }
  
} // namespace kfoundation
