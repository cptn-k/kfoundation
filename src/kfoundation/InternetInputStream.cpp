//
//  InternetInputStream.cpp
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// Std
#include <unistd.h>

// Internal
#include "Ptr.h"
#include "Int.h"
#include "System.h"
#include "ObjectSerializer.h"

// Self
#include "InternetInputStream.h"

#define MAX_QUEUE_SIZE 5

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  InternetInputStream::InternetInputStream()
  {
    _isBound = false;
    _isOpen = false;
    _isEof = true;
  }
  
  
  InternetInputStream::~InternetInputStream() {
    unbind();
  }
  
  
  
// --- METHODS --- //
  
  const InternetAddress& InternetInputStream::getAddress() const {
    return _address;
  }
  
  
  void InternetInputStream::bind(const InternetAddress& address)
  throw(IOException)
  {
    if(_isBound) {
      unbind();
    }
    
    _address = address;
    
    memset(&_sockaddr, 0, sizeof(sockaddr_in));
    _sockaddr.sin_family = AF_INET;
    memcpy(&_sockaddr.sin_addr.s_addr, address.getIp(), 4);
    _sockaddr.sin_port = htons(address.getPort());
    
    _hostSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    int err = ::bind(_hostSocket, (sockaddr*)&_sockaddr, sizeof(_sockaddr));
    
    if(err != 0) {
      throw IOException("Could not bind to host "
                        + string(inet_ntoa(_sockaddr.sin_addr)) + ":"
                        + Int::toString(ntohs(_sockaddr.sin_port))
                        + ". Reason: " + System::getLastSystemError());
    }
    
    _isBound = true;
  }
  
  
  void InternetInputStream::unbind() {
    if(_isOpen) {
      close();
    }
    
    if(_isBound) {
      _isBound = false;
      ::close(_hostSocket);
    }    
  }
  
  
  bool InternetInputStream::isBound() const {
    return _isBound;
  }
  
  
  void InternetInputStream::listen() {
    _nReceived = 0;
    
    int err = ::listen(_hostSocket, MAX_QUEUE_SIZE);
    if(err != 0) {
      throw IOException("Could not initiate listening (Address: " + _address
          + "). Reason: " + System::getLastSystemError());
    }
    
    sockaddr_in senderAddress;
    socklen_t len = sizeof(senderAddress);
    memset(&senderAddress, 0, len);
    
    _readSocket = ::accept(_hostSocket, (sockaddr*)&senderAddress, &len);
    
    _isOpen = true;
    _isEof = false;
  }
  
  
  bool InternetInputStream::isOpen() const {
    return _isOpen;
  }
  
  
  void InternetInputStream::close() {
    if(_isOpen) {
      ::close(_readSocket);
      _isOpen = false;
    }
    _isEof = true;
  }
  
  
  kf_int32_t InternetInputStream::getNReceivedOctets() const {
    return _nReceived;
  }
  
  
// Inherited from InputStream //
  
  kf_int32_t InternetInputStream::read(kf_octet_t* buffer, const kf_int32_t nBytes) {
    if(!_isOpen) {
      throw IOException("Attemp to read a closed socket (Address: " + _address + ")");
    }
    
    kf_int32_t totalRead = 0;
    
    while(totalRead < nBytes) {
      kf_int32_t s = (kf_int32_t)::read(_readSocket, buffer + totalRead, nBytes - totalRead);
      
      if(s == 0) {
        _isEof = true;
        break;
      }
      
      totalRead += s;
    }
    
    _nReceived += totalRead;
    
    return totalRead;
  }
  
  
  int InternetInputStream::read() {
    if(!_isOpen) {
      throw IOException("Attemp to read a closed socket (Address: " + _address + ")");
    }
    
    kf_octet_t v;
    size_t s = ::read(_readSocket, &v, 1);
    
    if(s <= 0) {
      _isEof = true;
      return -1;
    }
    
    _nReceived++;
    
    return v;
  }
  
  
  int InternetInputStream::peek() {
    throw KFException("peek() is not supported.");
  }
  
  
  kf_int32_t InternetInputStream::skip(kf_int32_t bytes) {
    throw KFException("skip() is not supported.");
  }
  
  
  bool InternetInputStream::isEof() {
    return _isEof;
  }
  
  
  bool InternetInputStream::isMarkSupported() {
    return false;
  }
  
  
  void InternetInputStream::mark() {
    throw KFException("mark() is not supported.");
  }
  
  
  void InternetInputStream::reset() {
    throw KFException("reset() is not supported.");
  }
  
  
  bool InternetInputStream::isBigEndian() {
    return System::isBigEndian();
  }
  
  
// Inherited from Serializing Stream //
  
  void InternetInputStream::serialize(PPtr<ObjectSerializer> serializer) const {
    serializer->object("InternetInputStream")
        ->attribute("address", _address.toString())
        ->attribute("isOpen", _isOpen)
        ->attribute("isBound", _isBound)
        ->endObject();
  }
  
  
} // namespace kfoundation
