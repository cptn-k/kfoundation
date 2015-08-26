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

#include <ctime>
#include <cmath>

#ifdef KF_UNIX
#  include <sys/time.h>
#else
#  error "Only UNIX/Linux is supported"
#endif

#include "Streamer.h"
#include "ManagedObject.h"

using namespace std;

namespace kfoundation {

  /**
   * Utility class to measure execution time of a code fragment. It keeps elapsed
   * time and consumed CPU time. Usage:
   *
   *     Timer t;
   *     t.start();
   *     ... do whatever you want to measure here ...
   *     LOG << t << EL;
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

  class Timer : public ManagedObject, public Streamer {
  private:
    clock_t        _clockBase;
    struct timeval _timeBase;
    string         _name;

  public:
    Timer();
    Timer(string name);

    void start();
    bool isStarted() const;
    double get() const;
    double getCpuTime() const;
    virtual void printToStream(ostream& os) const;
  };
  
} // namespace kfoundation

#endif
