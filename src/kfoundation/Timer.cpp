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

#include "definitions.h"

// Std
#include <ctime>
#include <cmath>

// POSIX
#ifdef KF_UNIX
#  include <sys/time.h>
#else
#  error "Platform is not supported"
#endif

// Internal
#include "Ref.h"
#include "OutputStream.h"
#include "ObjectSerializer.h"
#include "UString.h"

// Self
#include "Timer.h"

#define MICRO 1000000.0

namespace kfoundation {

  /**
   * Default constructor.
   */
    
  Timer::Timer() {
    _clockBase = 0;
    _timeBase = 0;
  }
    
    
  /**
   * Constructor, creates a named timer.
   */

  Timer::Timer(RefConst<UString> name) {
    _name = name;
    _clockBase = 0;
    _timeBase = 0;
  }
  
  
  /**
   * Starts measuring.
   */

  void Timer::start() {
    _clockBase = clock();
    struct timeval tv;
    gettimeofday(&tv, NULL);
    _timeBase = tv.tv_sec + tv.tv_usec / MICRO;
  }
  
  
  /**
   * Checks if the timer is started.
   */

  bool Timer::isStarted() const {
    return _clockBase != 0;
  }
  
  
  /**
   * Returns the elapsed time since start() is called.
   */

  double Timer::getRealTime() const {
    if(_clockBase == 0) {
      return 0;
    }

    struct timeval now;
    gettimeofday(&now, NULL);
    return _timeBase;
  }
  
  
  /**
   * Returns amount of CPU time consumed by this process since start() is called.
   * This might be bigger or smaller than get() depending of the amount of CPU
   * time and number of cores specified by OS to execute the process.
   */

  double Timer::get() const {
    return (_clockBase == 0)?0:(clock() - _clockBase) / (double)CLOCKS_PER_SEC;
  }

  
  void Timer::serialize(Ref<ObjectSerializer> stream) const {
    stream->object(K"Timer")
      ->attribute(K"name", _name)
      ->attribute(K"elapsedCpuTime", get())
      ->attribute(K"elapsedRealTime", getRealTime())
      ->endObject();
  }

} // namespace kfoundation