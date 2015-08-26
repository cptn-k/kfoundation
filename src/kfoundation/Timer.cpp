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

#include "Timer.h"

namespace kfoundation {

  /**
   * Default constructor.
   */
    
  Timer::Timer()
  {
    Timer("");
  }
    
    
  /**
   * Constructor, creates a named timer.
   */

  Timer::Timer(string name) {
    _name = name;
    _clockBase = 0;
  }
  
  
  /**
   * Starts measuring.
   */

  void Timer::start() {
    _clockBase = clock();
    gettimeofday(&_timeBase, NULL);
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

  double Timer::get() const {
    if(!isStarted())
      return NAN;

    struct timeval now;
    gettimeofday(&now, NULL);
    double diff = now.tv_sec - _timeBase.tv_sec 
                + (now.tv_usec - _timeBase.tv_usec) / 1000000.0;
    return diff;
  }
  
  
  /**
   * Returns amount of CPU time consumed by this process since start() is called.
   * This might be bigger or smaller than get() depending of the amount of CPU
   * time and number of cores specified by OS to execute the process.
   */

  double Timer::getCpuTime() const {
    return (clock() - _clockBase) / (double)CLOCKS_PER_SEC; 
  }

  
  void Timer::printToStream(ostream& os) const {
    os << "Timer[";
    if(_name.size() != 0)
      os << "name: \"" << _name << "\", ";
    if(!isStarted())
      os << "NOT_STARTED]";
    else
      os << "elapsed: " << get() << ", cpuTime: " << getCpuTime() << "]";
  }

  
} // namespace kfoundation
