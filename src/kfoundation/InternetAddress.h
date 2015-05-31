/*---[InternetAddress.h]---------------------------------------m(._.)m--------*\
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

#ifndef __KFoundation__InternetAddress__
#define __KFoundation__InternetAddress__

#include "definitions.h"

// Super
#include "Streamer.h"

namespace kfoundation {
  
  using namespace std;
  
  class InternetAddress : public Streamer {
    
  // --- NESTED TYPES --- //
    
    enum class_t {
      A, B, C, NONE
    };
    
    
  // --- FIELDS --- //
    
    private: kf_octet_t _ip[4];
    private: kf_int32_t _port;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: InternetAddress();
    public: InternetAddress(const kf_octet_t* ip, kf_int32_t port);
    public: InternetAddress(const string& str);
    public: InternetAddress(const string& str, kf_int32_t port);
    public: InternetAddress(const InternetAddress& other);
    
  
  // --- METHODS --- //
    
    private: void parseString(const string& str);
    public: const kf_octet_t* getIp() const;
    public: kf_int32_t getPort() const;
    public: InternetAddress copyWithPort(kf_int32_t port) const;
    public: bool equals(const InternetAddress& other) const;
    public: class_t getClass() const;
    public: InternetAddress getBroadcastAddress() const;
    public: InternetAddress getSubnetMask() const;
    
    // Inherited from Streamer //
    public: void printToStream(ostream& stream) const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__InternetAddress__) */
