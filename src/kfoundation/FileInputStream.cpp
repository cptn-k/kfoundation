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

#include "Path.h"
#include "FileInputStream.h"
#include "Ptr.h"

namespace kfoundation {
  
  
  /**
   * Constructor, opens the file pointed by the given Path object.
   *
   * @param path The path to the file to open.
   */
  
  FileInputStream::FileInputStream(PPtr<Path> path)
  : _ifs( new ifstream(path->getString().c_str()) )
  {
    mark();
    _ifs->seekg(0, ios_base::end);
    _size = _ifs->tellg();
    _ifs->seekg(0);
  }

  
  /**
   * Constructor, opens file pointed by the given string to read.
   *
   * @param fileName The path to the file to open.
   */
  
  FileInputStream::FileInputStream(const string& fileName)
  : _ifs(new ifstream(fileName.c_str()))
  {
    mark();
    _ifs->seekg(0, ios_base::end);
    _size = _ifs->tellg();
    _ifs->seekg(0);
  }
  
  
  /**
   * Deconstructor.
   */
  
  FileInputStream::~FileInputStream() {
    _ifs->close();
    delete _ifs;
  }
  
  
  /**
   * Returns the size of the openned file.
   */
  
  kf_int64_t FileInputStream::getSize() const {
    return _size;
  }
  
  
  /**
   * Checks if the file is open.
   */
  
  bool FileInputStream::isOpen() const {
    return _ifs->is_open();
  }
  
  
  /**
   * Closes the file.
   */
  
  void FileInputStream::close() {
    _ifs->close();
  }
  
  
  kf_int32_t FileInputStream::read(kf_octet_t *buffer, const kf_int32_t nBytes)
  {
    _ifs->read((istream::char_type*)buffer, nBytes);
    return (kf_int32_t)_ifs->gcount();
  }
  
  
  int FileInputStream::read() {
    return _ifs->get();
  }
  
  
  int FileInputStream::peek() {
    return _ifs->peek();
  }
  
  
  kf_int32_t FileInputStream::skip(kf_int32_t nBytes) {
    _ifs->ignore(nBytes);
    return (kf_int32_t)_ifs->gcount();
  }
  
  
  bool FileInputStream::isEof() {
    return _ifs->eof();
  }
  
  
  bool FileInputStream::isMarkSupported() {
    return true;
  }
  
  
  void FileInputStream::mark() {
    _mark = _ifs->tellg();
  }
  
  
  void FileInputStream::reset() {
    _ifs->seekg(_mark);
  }
  
  
  bool FileInputStream::isBigEndian() {
    return System::isBigEndian();
  }
  
} // namespace kfoundation