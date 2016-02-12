/*---[InternetAddress.h]---------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::InternetAddress::*
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

#ifndef __KFoundation__InternetAddress__
#define __KFoundation__InternetAddress__

#include "definitions.h"
#include "RefDecl.h"

// Super
#include "KFObject.h"
#include "Streamer.h"

namespace kfoundation {

  class OutputStream;
  class UString;
  
  /**
   * Encodes an IP address and port number. This is an immutable object.
   *
   * @ingroup io
   * @headerfile InternetAddress.h <kfoundation/InternetAddress.h>
   */
  
  class InternetAddress
  : public KFObject, public Streamer, public Comparable<InternetAddress>
  {
    
  // --- NESTED TYPES --- //
    
    /**
     * Represents IPv4 address classes.
     */
    
    public: enum class_t {
      A, ///< Class A
      B, ///< Class B
      C, ///< Class C
      D, ///< Class D, reserved for multicast
      E, ///< Class E, experimental, used for research
    };


  // --- STATIC FIELDS --- //
    
    private: static const kf_octet_t CLASS_A_MASK[4];
    private: static const kf_octet_t CLASS_B_MASK[4];
    private: static const kf_octet_t CLASS_C_MASK[4];
    private: static const kf_octet_t DEFAULT_MASK[4];
 

  // --- FIELDS --- //
    
    private: kf_octet_t _ip[4];
    private: kf_int32_t _port;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: InternetAddress();
    public: InternetAddress(const kf_octet_t* ip, kf_int32_t port);
    public: InternetAddress(RefConst<UString> str);
    public: InternetAddress(RefConst<UString> str, kf_int32_t port);
    public: InternetAddress(RefConst<InternetAddress> other);
    
  
  // --- METHODS --- //
    
    private: void parseString(RefConst<UString> str);
    public: const kf_octet_t* getIp() const;
    public: kf_int32_t getPort() const;
    public: Ref<InternetAddress> copyWithPort(kf_int32_t port) const;
    public: class_t getClass() const;
    public: Ref<InternetAddress> getBroadcastAddress() const;
    public: Ref<InternetAddress> getSubnetMask() const;

    // From Comparable<InternetAddress> //
    public: bool equals(RefConst<InternetAddress> other) const;
    
    // Inherited from Streamer //
    public: void printToStream(Ref<OutputStream> stream) const;
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__InternetAddress__) */