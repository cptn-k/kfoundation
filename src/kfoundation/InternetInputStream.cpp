/*---[InternetInputStream.cpp]---------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::InternetInputStream::*
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
#include <cstring>

// Unix
#include <unistd.h>

// Internal
#include "Ref.h"
#include "Int.h"
#include "System.h"
#include "ObjectSerializer.h"
#include "UString.h"

// Self
#include "InternetInputStream.h"

#define MAX_QUEUE_SIZE 5

namespace kfoundation {
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor, creates a closed input stream.
   * To use, first, bind this stream to an address using
   * bind(const InternetAddress&) method, then invoke listen() method.
   * listen() is a blocking method. It will be unblocked once a connection
   * from a remote host is established.
   * The following is the common pattern to use this
   * class:
   *
   *     Ref<InternetInputStream> iis = new InternetInputStream();
   *     iis.bind(InternetAddress("1.2.3.4:5678"));
   *     iis.listen();
   *     while(!iis.isEof()) {
   *         iis.read();
   *     }
   *.
   */
  
  InternetInputStream::InternetInputStream() {
    _isBound = false;
    _isOpen = false;
    _isEof = true;
  }
  
  
  /**
   * Deconstructor.
   */
  
  InternetInputStream::~InternetInputStream() {
    unbind();
  }
  
  
  
// --- METHODS --- //
  
  /**
   * Returns the address that this stream is assigned to,.
   */
  
  const InternetAddress& InternetInputStream::getAddress() const {
    return _address;
  }
  
  
  /**
   * Binds this stream to an Internet address. If the stream is already open and
   * bound to another address, it will be closed and unbound. If the process
   * does not have enough privilages to bind to the given address and port,
   * or the port is already in use, this function will throw and exception.
   *
   * @param address The address to bind this stream to.
   * @throw Throws IOException if binding fails.
   * @see unbind()
   * @see isBound()
   */
  
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

    bool opVal = true;
    setsockopt(_hostSocket, SOL_SOCKET, SO_REUSEADDR, &opVal, sizeof(opVal));

    int err = ::bind(_hostSocket, (sockaddr*)&_sockaddr, sizeof(_sockaddr));

    if(err != 0) {
      throw IOException(K"Could not bind to host "
        + UString((kf_octet_t*)inet_ntoa(_sockaddr.sin_addr)) + ':'
        + ntohs(_sockaddr.sin_port) + ". Reason: "
        + System::getLastSystemError());
    }
    
    _isBound = true;
  }
  
  
  /**
   * Closes and unbinds this stream.
   *
   * @see bind()
   * @see isBound()
   */
  
  void InternetInputStream::unbind() {
    if(_isOpen) {
      close();
    }
    
    if(_isBound) {
      _isBound = false;
      shutdown(_hostSocket, SHUT_RDWR);
      ::close(_hostSocket);
    }    
  }
  
  
  /**
   * Checks if this stream is bound to an address.
   *
   * @see bind()
   * @see unbind()
   */
  
  bool InternetInputStream::isBound() const {
    return _isBound;
  }
  
  
  /**
   * Blocks the current thread until an incomming connection is stablished. 
   *
   * @throw Throws IOException if listening could not be initiated.
   */
  
  void InternetInputStream::listen() {
    _nReceived = 0;
    
    int err = ::listen(_hostSocket, MAX_QUEUE_SIZE);
    if(err != 0) {
      throw IOException(K"Could not initiate listening (Address: " + _address
          + "). Reason: " + System::getLastSystemError());
    }
    
    sockaddr_in senderAddress;
    socklen_t len = sizeof(senderAddress);
    memset(&senderAddress, 0, len);
    
    _readSocket = ::accept(_hostSocket, (sockaddr*)&senderAddress, &len);
    
    _isOpen = true;
    _isEof = false;
  }
  
  
  /**
   * Checks if the port is open.
   */
  
  bool InternetInputStream::isOpen() const {
    return _isOpen;
  }
  
  
  /**
   * Closes the stream if it is open.
   */
  
  void InternetInputStream::close() {
    if(_isOpen) {
      ::close(_readSocket);
      _isOpen = false;
    }
    _isEof = true;
  }
  
  
  /**
   * Returns the number of octets received since the connection is stablished.
   */
  
  kf_int32_t InternetInputStream::getNReceivedOctets() const {
    return _nReceived;
  }
  
  
// Inherited from InputStream //
  
  kf_int32_t InternetInputStream::read(kf_octet_t* buffer, const kf_int32_t nBytes) {
    if(!_isOpen) {
      throw IOException(K"Attemp to read a closed socket (Address: "
          + _address + ")");
    }
    
    kf_int32_t totalRead = 0;
    
    while(totalRead < nBytes) {
      kf_int32_t s = (kf_int32_t)::read(_readSocket, buffer + totalRead,
          nBytes - totalRead);
      
      if(s == 0) {
        _isEof = true;
        break;
      }
      
      totalRead += s;
    }
    
    _nReceived += totalRead;
    
    return totalRead;
  }
  
  
  kf_int16_t InternetInputStream::read() {
    if(!_isOpen) {
      throw IOException(K"Attemp to read a closed socket (Address: "
          + _address + ")");
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
  
  
  kf_int16_t InternetInputStream::peek() {
    throw KFException(K"peek() is not supported.");
  }
  
  
  kf_int32_t InternetInputStream::skip(kf_int32_t bytes) {
    throw KFException(K"skip() is not supported.");
  }
  
  
  bool InternetInputStream::isEof() {
    return _isEof;
  }
  
  
  bool InternetInputStream::isMarkSupported() {
    return false;
  }
  
  
  void InternetInputStream::mark() {
    throw KFException(K"mark() is not supported.");
  }
  
  
  void InternetInputStream::reset() {
    throw KFException(K"reset() is not supported.");
  }
  
  
  bool InternetInputStream::isBigEndian() {
    return System::isBigEndian();
  }
  
  
// Inherited from Serializing Stream //
  
  void InternetInputStream::serialize(Ref<ObjectSerializer> serializer) const {
    serializer->object(K"InternetInputStream")
        ->attribute(K"address", _address.toString())
        ->attribute(K"isOpen", _isOpen)
        ->attribute(K"isBound", _isBound)
        ->endObject();
  }

} // namespace kfoundation