//
//  BufferOutputStream.cpp
//  KFoundation
//
//  Created by Hamed KHANDAN on 10/28/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

// Std
#include <cstdio>
#include <cstring>

// Internal
#include "System.h"
#include "Ptr.h"
#include "InputStream.h"

// Self
#include "BufferOutputStream.h"

#define BUFFER_GROWTH_RATE 2

namespace kfoundation {

  string BufferOutputStream::toBinaryString(const kf_octet_t *data, int size) {
    using namespace std;
    char* chars = new char[size*3 + 3];
    char* p = chars;
    for(int i = 0; i < size; i++) {
      p += sprintf(p, "%02X ", (int)data[i]);
    }
    string str(chars);
    delete[] chars;
    return str;
  }
  
  
  BufferOutputStream::BufferOutputStream(const kf_int32_t capacity) {
    _capacity = capacity;
    _size = 0;
    _data = new kf_octet_t[_capacity];
    _debug = false;
  }
  
  
  BufferOutputStream::~BufferOutputStream() {
    delete[] _data;
  }
  
  
  void BufferOutputStream::grow() {
    kf_int32_t newCapacity = _capacity * BUFFER_GROWTH_RATE;
    kf_octet_t* newData = new kf_octet_t[newCapacity];
    memcpy(newData, _data, _size);
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
  }
  
  
  kf_octet_t* BufferOutputStream::getData() const {
    return _data;
  }
  
  
  kf_int32_t BufferOutputStream::getSize() const {
    return _size;
  }
  
  
  void BufferOutputStream::print() const {
    using namespace std;
    for(int i = 0; i < _size; i++) {
      printf("%02X ", (int)_data[i]);
    }
    printf("\n");
  }
  
  
  void BufferOutputStream::debug(bool on) {
    _debug = on;
  }
  
  
  bool BufferOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  void BufferOutputStream::write(const kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    while(_size + nBytes > _capacity) {
      grow();
    }
    
    memcpy(_data + _size, buffer, nBytes);
    _size += nBytes;
    
    if(_debug) {
      for(int i = 0; i < nBytes; i++) {
        printf("%02X ", (int)buffer[i]);
      }
      fflush(stdout);
    }
  }
  
  
  void BufferOutputStream::write(kf_octet_t byte) {
    if(_size >= _capacity) {
      grow();
    }
    
    _data[_size] = byte;
    _size++;
    
    if(_debug) {
      printf("%02X ", (int)byte);
      fflush(stdout);
    }
  }
  
  
  void BufferOutputStream::write(PPtr<InputStream> is) {
    kf_octet_t tmp[1024];
    
    while(!is->isEof()) {
      int s = is->read(tmp, 1024);
      write(tmp, s);
    }
  }
  
  
  void BufferOutputStream::close() {
    if(_debug) {
      printf("\n");
    }
  }
  
} // namespace kfoundation

