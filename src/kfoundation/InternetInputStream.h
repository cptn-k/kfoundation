/*---[InternetInputStream.h]-----------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::InternetInputStream::*
 |  Implements: -
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
  
  /**
   * Input stream used to read from a TCP/IP port.
   *
   * @ingroup io
   * @headerfile InternetInputStream.h <kfoundation/InternetInputStream.h>
   */
  
  class InternetInputStream : public InputStream, public SerializingStreamer {
    
  // --- FIELDS --- //

    private: RefConst<InternetAddress> _address;
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
    
    public: RefConst<InternetAddress> getAddress() const;
    public: void bind(RefConst<InternetAddress> address) throw(IOException);
    public: void unbind();
    public: bool isBound() const;
    public: void listen();
    public: bool isOpen() const;
    public: void close();
    public: kf_int32_t getNReceivedOctets() const;
    
    // Inherited from InputStream //
    public: kf_int32_t read(kf_octet_t* buffer, const kf_int32_t nBytes);
    public: kf_int16_t read();
    public: kf_int16_t peek();
    public: kf_int32_t skip(kf_int32_t bytes);
    public: bool isEof();
    public: bool isMarkSupported();
    public: void mark();
    public: void reset();
    public: bool isBigEndian();
    
    // Inherited from SerializingSTream //
    public: void serialize(Ref<ObjectSerializer> serializer) const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__InternetInputStream__) */