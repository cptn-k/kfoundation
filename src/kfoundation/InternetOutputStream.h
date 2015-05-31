//
//  InternetOutputStream.h
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__InternetOutputStream__
#define __KFoundation__InternetOutputStream__

// Arpa
#include <arpa/inet.h>

// Internal
#include "InternetAddress.h"

// Super
#include "OutputStream.h"
#include "SerializingStreamer.h"
#include "IOException.h"

namespace kfoundation {
  
  class InternetOutputStream : public OutputStream, public SerializingStreamer
  {
    
  // --- FIELDS --- //
    
    private: InternetAddress _address;
    private: sockaddr_in _target;
    private: int _socket;
    private: bool _isOpen;
    private: kf_int32_t _nSent;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: InternetOutputStream(const InternetAddress& address);
    public: ~InternetOutputStream();
    
    
  // --- METHODS --- //
    
    public: const InternetAddress& getAddress() const;
    public: void connect() throw(IOException);
    public: bool isOpen() const;
    public: kf_int32_t getNSentOctets() const;
    
    // Inherited from OutputStream //
    public: bool isBigEndian() const;
    public: void write(const kf_octet_t* buffer, const kf_int32_t nBytes);
    public: void write(kf_octet_t byte);
    public: void write(PPtr<InputStream> os);
    public: void close();
    
    // Inherited from SerializingStreamer //
    public: void serialize(PPtr<ObjectSerializer> serializer) const;
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__InternetOutputStream__) */
