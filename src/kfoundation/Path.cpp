/*---[Path.cpp]------------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: Path
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

#ifdef KF_UNIX
  const char Path::PATH_SEPARATOR = '/';
#else
#  error "Unsupported platform"
#endif
  
  
  Path::Path(const string& str) {
    _str = str;
    _segments = new Array<int>();
    refresh();
  }
  
  
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
  
  
  bool Path::hasExtention() const {
    return _extention != -1;
  }
  
  
  bool Path::isAbsolute() const {
    return _isAbsolute;
  }
  
  
  int Path::getNSegments() const {
    return (int)_segments->getSize();
  }
  
  
  string Path::getSegement(int index) const {
    int begin = (index == 0)?0:(_segments->at(index - 1) + 1);
    int end = _segments->at(index);
    
    if(end == -1) {
      return "/";
    }
    
    return _str.substr(begin, end - begin);
  }
  
  
  string Path::getFileName() const {
    int s = _segments->getSize() - 1;
    int begin = (s == 0)?0:(_segments->at(s - 1) + 1);
    int end = _segments->at(s);
    if(_extention > -1) {
      end = _extention;
    }
    
    return _str.substr(begin, end - begin);
  }
  
  
  string Path::getExtention() const {
    if(!hasExtention()) {
      return "";
    }
    
    return _str.substr(_extention + 1, _str.size() - _extention - 1);
  }
  
  
  string Path::getFileNameWithExtension() const {
    return getSegement(getNSegments() - 1);
  }
  
  
  Ptr<Path> Path::addSegement(const string &s) {
    string str = _str + PATH_SEPARATOR + s;
    return new Path(str);
  }
  
  
  Ptr<Path> Path::changeExtension(const string& ex) {
    if(hasExtention()) {
      string str = _str.substr(0, _extention + 1) + ex;
      return new Path(str);
    }
    
    string str = _str + '.' + ex;
    return new Path(str);
  }
  
  
  Ptr<Path> Path::removeExtension() {
    if(hasExtention()) {
      string str = _str.substr(0, _extention);
      return new Path(str);
    }
    
    return getPtr().AS(Path).retain();
  }
  
  
  Ptr<Path> Path::parent() {
    int s = _segments->getSize();
    
    if(s <= 1) {
      return new Path("");
    }
    
    string str = _str.substr(0, _segments->at(s - 2));
    return new Path(str);
  }
  
  
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
  
  bool Path::exists() const {
#ifdef KF_UNIX
    return access(_str.c_str(), F_OK) != -1;
#else
#  error "Platform not supported."
#endif
  }
  
  
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
