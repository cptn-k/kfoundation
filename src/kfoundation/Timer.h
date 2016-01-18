/*---[Logger.h]------------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

#ifndef ORG_KNORBA_COMMON_TIMER_H
#define ORG_KNORBA_COMMON_TIMER_H

#include "definitions.h"

// Super
#include "KFObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {

  class UString;

  /**
   * Utility class to measure execution time of a code fragment. It keeps elapsed
   * time and consumed CPU time. Usage:
   *
   *     Timer t;
   *     t.start();
   *     ... do whatever you want to measure here ...
   *     LOG << t << ENDS;
   *
   * If you want to store the elapsed time to use it later, use get() method.
   *
   *     double duration = t.get();
   *
   * To reset an already started timer, call start() method again.
   *
   * @ingroup utils
   * @headerfile Timer.h <kfoundation/Timer.h>
   */

  class Timer : public KFObject, public SerializingStreamer {

  // --- FIELDS --- //

    private: kf_int64_t _clockBase;
    private: double     _timeBase;
    private: RefConst<UString> _name;


  // --- CONSTRUCTORS --- //

    public: Timer();
    public: Timer(RefConst<UString> name);


  // --- METHODS --- //

    public: void start();
    public: bool isStarted() const;
    public: double getRealTime() const;
    public: double get() const;

    // From SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> stream) const;

  };
  
} // namespace kfoundation

#endif