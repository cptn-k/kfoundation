/*---[InternetAddress.cpp]-------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: InternetAddress
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

// Internal
#include "PredictiveParserBase.h"
#include "StringInputStream.h"

// Self
#include "InternetAddress.h"

namespace kfoundation {

  
// --- (DE)CONSTRUCTORS --- //
  
  InternetAddress::InternetAddress() {
    memset(_ip, 0, sizeof(_ip));
    _port = -1;
  }
  
  
  InternetAddress::InternetAddress(const kf_octet_t* ip, kf_int32_t port)
  {
     memcpy(_ip, ip, sizeof(_ip));
    _port = port;
  }
  
  
  InternetAddress::InternetAddress(const string& str) {
    _port = -1;
    parseString(str);
  }
  
  
  InternetAddress::InternetAddress(const string& str, kf_int32_t port)
  {
    parseString(str);
    _port = port;
  }
  
  
  InternetAddress::InternetAddress(const InternetAddress& other) {
    _port = other._port;
    memcpy(_ip, other._ip, sizeof(_ip));
  }
  
  
// --- METHODS --- //
  
  void InternetAddress::parseString(const string& str) {
    memset(_ip, 0, sizeof(_ip));
    
    Ptr<StringInputStream> stringInput = new StringInputStream(str);
    Ptr<PredictiveParserBase> parser = new PredictiveParserBase(stringInput.AS(InputStream));
    
    long int n = 0;
    
    parser->readNumber(n);
    _ip[0] = (kf_octet_t)n;
    
    parser->readChar('.');
    parser->readNumber(n);
    _ip[1] = (kf_octet_t)n;
    
    parser->readChar('.');
    parser->readNumber(n);
    _ip[2] = (kf_octet_t)n;
    
    parser->readChar('.');
    parser->readNumber(n);
    _ip[3] = (kf_octet_t)n;
    
    if(parser->testChar(':')) {
      parser->readChar(':');
      parser->readNumber(n);
      _port = (kf_int32_t)n;
    }
  }
  
  
  const kf_octet_t* InternetAddress::getIp() const {
    return _ip;
  }
  
  
  kf_int32_t InternetAddress::getPort() const {
    return _port;
  }
  
  
  InternetAddress InternetAddress::copyWithPort(kf_int32_t port) const {
    return InternetAddress(_ip, port);
  }
  
  
  bool InternetAddress::equals(const InternetAddress& other) const {
    if(_port != other._port) {
      return false;
    }
    
    return memcmp(_ip, other._ip, sizeof(_ip)) == 0;
  }
  
  
  InternetAddress::class_t InternetAddress::getClass() const {
    if(_ip[0] >= 1 && _ip[0] <= 126) {
      return A;
    } else if(_ip[0] >= 128 && _ip[0] <= 191) {
      return B;
    } else if(_ip[0] >= 192 && _ip[0] <= 223) {
      return C;
    }
    
    return NONE;
  }
  
  
  InternetAddress InternetAddress::getBroadcastAddress() const {
    InternetAddress mask = getSubnetMask();
    kf_octet_t masked[4];
    for(int i = 0; i < 4; i++) {
      masked[i] = _ip[i] | mask._ip[i];
    }
    return InternetAddress(masked, -1);
  }
  
  
  InternetAddress InternetAddress::getSubnetMask() const {
    switch (getClass()) {
      case A:
        return InternetAddress((kf_octet_t[4]){0, 255, 255, 255}, -1);
        
      case B:
        return InternetAddress((kf_octet_t[4]){0, 0, 255, 255}, -1);
        
      case C:
        return InternetAddress((kf_octet_t[4]){0, 0, 0, 255}, -1);
        
      default:
        return InternetAddress((kf_octet_t[4]){0, 0, 0, 0}, -1);
    }
  }
  
  
  void InternetAddress::printToStream(ostream& stream) const {
    stream << (int)_ip[0] << "." << (int)_ip[1] << "." << (int)_ip[2]
        << "." << (int)_ip[3];
    if(_port >= 0) {
        stream << ":" << _port;
    }
  }
  
} // namespace kfoundation