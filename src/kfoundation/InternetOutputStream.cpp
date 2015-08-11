/*---[InternetOutputStream.cpp]-------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::InternetOutputStream::*
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

// Internal
#include "Ptr.h"
#include "System.h"
#include "InputStream.h"
#include "ObjectSerializer.h"

// Self
#include "InternetOutputStream.h"

#define SOCK_CAPACITY 4096

namespace kfoundation {
  
// --- (DE)CONSTRUCTOR --- //
  
  /**
   * Constructor, the object will dedicated to read from the given address.
   * To use, invoke connect() first.
   */
  
  InternetOutputStream::InternetOutputStream(const InternetAddress& address)
  : _address(address)
  {
    memset(&_target, 0, sizeof(sockaddr_in));
    _target.sin_family = AF_INET;
    memcpy(&_target.sin_addr.s_addr, address.getIp(), 4);
    _target.sin_port = htons(address.getPort());
    _isOpen = false;
  }
  
  
  /**
   * Deconstructor.
   */
  
  InternetOutputStream::~InternetOutputStream() {
    if(_isOpen) {
      close();
    }
  }
  
  
  
// --- METHODS --- //
  
  /**
   * Returns the address this stream is assigend to.
   */
  
  const InternetAddress& InternetOutputStream::getAddress() const {
    return _address;
  }
  
  
  /**
   * Connects to the given remote address.
   * Blocks the current thread until the connection is established.
   *
   * @throw Throws IOException if the connection could not be established.
   */
  
  void InternetOutputStream::connect() throw(IOException){
    if(_isOpen) {
      throw IOException("Attempt to reconnect a stream that is already "
          "connected (Address: " + _address + ")");
    }
    
    _nSent = 0;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    
    int err = ::connect(_socket, (sockaddr*)&_target, sizeof(_target));
    
    if(err != 0) {
      throw IOException("Connection faild  (Address: " + _address
          + "). Reason: " + System::getLastSystemError());
    }
    
    _isOpen = true;
  }
  
  
  /**
   * Checks if the connection is open.
   */
  
  bool InternetOutputStream::isOpen() const {
    return _isOpen;
  }
  
  
  /**
   * Get the number of octets written since the connection is established.
   */
  
  kf_int32_t InternetOutputStream::getNSentOctets() const {
    return _nSent;
  }
  
  
// Inherited from OutputStream //
  
  bool InternetOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }
  
  
  void InternetOutputStream::write(const kf_octet_t* buffer,
      const kf_int32_t nBytes)
  {
    if(!_isOpen) {
      throw IOException("Attemp to write to a closed socket. " + toString());
    }
    ssize_t octetsLeftToSend = nBytes;
    ssize_t totalSent = 0;
    
    while(octetsLeftToSend > 0) {
      ssize_t octetsToSend
        = (octetsLeftToSend <= SOCK_CAPACITY)?octetsLeftToSend:SOCK_CAPACITY;
      
      ssize_t s = ::write(_socket, buffer + totalSent, octetsToSend);
      
      if(s < 0) {
        throw IOException("Failed to write to output (Address: " + _address
            + "). Reason: " + System::getLastSystemError());
      }
      
      octetsLeftToSend -= s;
      totalSent += s;
    }
    
    _nSent += totalSent;
  }
  
  
  void InternetOutputStream::write(kf_octet_t byte) {
    ssize_t s = ::write(_socket, &byte, 1);
    if(s < 0) {
      throw IOException("Failed to write to output (Address: " + _address
          + "). Reason: " + System::getLastSystemError());
    }
    _nSent++;
  }
  
  
  void InternetOutputStream::write(PPtr<InputStream> os) {
    if(!_isOpen) {
      throw IOException("Attemp to write to a closed socket (Address: "
          + _address + ")");
    }
    
    kf_octet_t buffer[SOCK_CAPACITY];
    while(!os->isEof()) {
      int s = os->read(buffer, SOCK_CAPACITY);
      write(buffer, s);
    }
  }
  
  
  void InternetOutputStream::close() {
    ::close(_socket);
    _isOpen = false;
  }
  
  
// Inherited from SerializingStreamer //
  
  void InternetOutputStream::serialize(PPtr<ObjectSerializer> serializer) const {
    serializer->object("InternetOutputStream")
      ->attribute("targetAddress", _address.toString())
      ->attribute("isOpen", _isOpen)
      ->endObject();
  }
  
  
} // namespace kfoundation