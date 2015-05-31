//
//  FileInputStream.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include "Path.h"
#include "FileInputStream.h"
#include "Ptr.h"

namespace kfoundation {
  
  FileInputStream::FileInputStream(PPtr<Path> path)
  : _ifs( new ifstream(path->getString().c_str()) )
  {
    mark();
    _ifs->seekg(0, ios_base::end);
    _size = _ifs->tellg();
    _ifs->seekg(0);
  }

  
  FileInputStream::FileInputStream(const string& fileName)
  : _ifs(new ifstream(fileName.c_str()))
  {
    mark();
    _ifs->seekg(0, ios_base::end);
    _size = _ifs->tellg();
    _ifs->seekg(0);
  }
  
  
  FileInputStream::~FileInputStream() {
    _ifs->close();
    delete _ifs;
  }
  
  kf_int64_t FileInputStream::getSize() const {
    return _size;
  }
  
  bool FileInputStream::isOpen() const {
    return _ifs->is_open();
  }
  
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