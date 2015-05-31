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

/** 
 *
 * @file
 * This file implements `kfoundation::Timer` class.
 *
 */

#include "Timer.h"

namespace kfoundation {
  
Timer::Timer()
{
  Timer("");
}

Timer::Timer(string name) {
  _name = name;
  _clockBase = 0;
}

void Timer::start() {
  _clockBase = clock();
  gettimeofday(&_timeBase, NULL);
}

bool Timer::isStarted() const {
  return _clockBase != 0;
}

double Timer::get() const {
  if(!isStarted())
    return NAN;

  struct timeval now;
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - _timeBase.tv_sec 
              + (now.tv_usec - _timeBase.tv_usec) / 1000000.0;
  return diff;
}

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
