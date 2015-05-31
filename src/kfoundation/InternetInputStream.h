//
//  InternetInputStream.h
//  KFoundation
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KFoundation__InternetInputStream__
#define __KFoundation__InternetInputStream__

// ARPA
#include <arpa/inet.h>

// Internal
#include "definitions.h"
#include "IOException.h"
#include "InternetAddress.h"

// Super
#include "InputStream.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  class InternetInputStream : public InputStream, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: InternetAddress _address;
    private: sockaddr_in _sockaddr;
    private: int _hostSocket;
    private: int _readSocket;
    private: bool _isBound;
    private: bool _isOpen;
    private: bool _isEof;
    private: kf_int32_t _nReceived;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: InternetInputStream();
    public: ~InternetInputStream();
    
    
  // --- METHODS --- //
    
    public: const InternetAddress& getAddress() const;
    public: void bind(const InternetAddress& address) throw(IOException);
    public: void unbind();
    public: bool isBound() const;
    public: void listen();
    public: bool isOpen() const;
    public: void close();
    public: kf_int32_t getNReceivedOctets() const;
    
    // Inherited from InputStream //
    public: kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes);
    public: int read();
    public: int peek();
    public: kf_int32_t skip(kf_int32_t bytes);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();
    
    // Inherited from SerializingSTream //
    public: void serialize(PPtr<ObjectSerializer> serializer) const;
    
  };
  
}

#endif /* defined(__KFoundation__InternetInputStream__) */
