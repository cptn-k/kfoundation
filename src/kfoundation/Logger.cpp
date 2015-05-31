/*---[Logger.cpp]---------------------------------------------m(._.)m--------*\
|
|  Project: KFoundation
|  Class: Logger - a logging utility
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
 * Implementation of KnoRBA Logging Facility classes
 *   - `kfoundation::Logger`
 *   - `kfoundation::Logger::Channel`
 *   - `kfoundation::Logger::Stream`
 *
 */

#include <unistd.h>
#include <cstdlib>

#include "Logger.h"
#include "definitions.h"
#include "Path.h"
#include "Bool.h"

namespace kfoundation {

  using namespace std;

//\/ Logger::Channel /\////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //
  
  Logger::Channel::Channel(const string& name, const string& fileName)
  : _name(name),
    _closeOnDeconstruct(true)
  {
    init();
    ofstream* ofs = new ofstream();
    ofs->open(fileName.c_str(), ofstream::out | ofstream::app);
    _os = ofs;
  }

  
  Logger::Channel::Channel(const string& name, ostream* os)
  : _name(name),
    _closeOnDeconstruct(false)
  {
    init();
    _os = os;
  }

  
  Logger::Channel::~Channel() {
    pthread_mutex_destroy(&_mutex);
    
    if(_closeOnDeconstruct) {
      ((ofstream*)_os)->close();
      delete _os;
    }
  }
  
  
// --- METHODS --- //
  
  void Logger::Channel::init() {
    pthread_mutex_init(&_mutex, NULL);
    _isSilent = false;
    _level = L3;
    _printHourAndMinute = true;
    _printSecondAndMilisecond = true;
    _printLocation = false;
  }
  
  
  void Logger::Channel::print(const stringstream &sstream,
      const kfoundation::Logger::Meta &meta)
  {
    if(!checkLevel(meta.level)) {
      return;
    }
   
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* timeInfo = localtime(&tv.tv_sec);
    
    
    const int BUFFER_SIZE = 20;
    char timeStr[BUFFER_SIZE];
    char* p = timeStr;
    *p = 0;
    
    if(_printHourAndMinute && _printSecondAndMilisecond) {
      p += strftime(p, BUFFER_SIZE, "%H:%M:%S.", timeInfo);
    } else if(_printHourAndMinute && !_printSecondAndMilisecond) {
      p += strftime(p, BUFFER_SIZE, "%H:%M", timeInfo);
    } else if(!_printHourAndMinute && _printSecondAndMilisecond) {
      p += strftime(p, BUFFER_SIZE, "%S.", timeInfo);
    }
    
    if(_printSecondAndMilisecond) {
      p += sprintf(p, "%d", tv.tv_usec);
    }
    
    // synchronized {
    pthread_mutex_lock(&_mutex);
    
    if(_printHourAndMinute || _printSecondAndMilisecond) {
      *_os << timeStr << ' ';
    }
    
    if(_printLocation && meta.fileName.length() > 0) {
      *_os << '[' << meta.functionName << '@' << meta.fileName << ':'
           << meta.lineNumber << "] ";
    }
    
    if(meta.level == ERR) {
      *_os << "!!! ";
    } else if(meta.level == WRN) {
      *_os << "! ";
    }
    
    *_os << sstream.str();
    _os->flush();
    
    pthread_mutex_unlock(&_mutex);
    // } synchronized
  }


  Logger::Channel& Logger::Channel::setLevel(level_t level) {
    _level = level;
    return *this;
  }
  
  
  Logger::level_t Logger::Channel::getLevel() const {
    return _level;
  }
  
  
  bool Logger::Channel::checkLevel(level_t lvl) const {
    if(_isSilent) {
      return false;
    }
    
    return lvl <= _level;
  }
  
  
  void Logger::Channel::setSilent(bool isSilent) {
    _isSilent = isSilent;
  }
  
  
  bool Logger::Channel::isSilent() const {
    return _isSilent;
  }
  
  
  const string& Logger::Channel::getName() const {
    return _name;
  }
  
  
  bool Logger::Channel::checkName(const string& name) const {
    return name == _name;
  }
  
  
  Logger::Channel& Logger::Channel::setFormat(bool printHourAndMinutes,
      bool printSecondAndMilisecond, bool printLocation)
  {
    _printHourAndMinute = printHourAndMinutes;
    _printSecondAndMilisecond = printSecondAndMilisecond;
    _printLocation = printLocation;
    return *this;
  }

  
//\/ Logger::Stream /\/////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  Logger::Stream::Stream(const Logger::Meta& meta,
      vector<Logger::Channel*>& channels)
  : _meta(meta),
    _channels(channels),
    _isOpen(false)
  {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _isOpen = _isOpen || _channels[i]->checkLevel(meta.level);
    }
  }
  
  
// --- METHODS --- //
  
  #define __K_ENUMERAND(X) Logger::Stream& Logger::Stream::operator<<(X a) {\
    if(_isOpen) {\
      _sstream << a;\
    }\
    return *this;\
  }
  __K_ENUMERATE_OVER_TYPES
  #undef __K_ENUMERAND

  
  Logger::Stream& Logger::Stream::operator<<(bool a) {
    if(_isOpen) {
      _sstream << Bool::toString(a);
    }
    return *this;
  }

  
  void Logger::Stream::operator<<(logger_flag_t f) {
    _sstream << endl;
    
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels[i]->print(_sstream, _meta);
    }
    
    if(f == EL_CON) {
      cout << _sstream.str();
      cout.flush();
    }
    
    delete this;
  }
  

//\/ Logger /\/////////////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //
  
  Logger::Logger() {
    // Nothing
  }

  
  Logger::~Logger() {
    removeAllChannels();
  }


// --- METHODS --- //
  
  Logger::Channel& Logger::addChannel(const string& name, ostream* os) {
    Channel* ch = new Channel(name, os);
    _channels.push_back(ch);
    return *ch;
  }
  

  Logger::Channel& Logger::addChannel(const string& name, const string& fileName)
  {
    Channel* ch = new Channel(name, fileName);
    _channels.push_back(ch);
    return *ch;
  }
  
  
  Logger::Channel& Logger::getChannelByName(const string &name) const {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      if(_channels[i]->checkName(name)) {
        return *_channels[i];
      }
    }
    
    throw "Logger " + name + " does not have a channel with name \"" + name
        + "\"";
  }

  
  void Logger::removeAllChannels() {
    for(vector<Channel*>::iterator it = _channels.begin();
        it != _channels.end(); it++)
    {
      Channel* ch = *it;
      delete ch;
    }
    
    _channels.clear();
  }

  
  Logger::Stream& Logger::log(level_t level, const char fileName[],
                      int lineNumber, const char functionName[])
  {
    Meta meta;
    meta.fileName = fileName;
    meta.lineNumber = lineNumber;
    meta.functionName = functionName;
    meta.level = level;
    
    // usleep(rand()%1000 + 200);
    Stream* stream = new Stream(meta, _channels);
    return *stream;
  }
  

  Logger::Stream& Logger::log(level_t level) {
    Meta meta;
    meta.level = level;
    
    // usleep(rand()%1000 + 200);
    Stream* stream = new Stream(meta, _channels);
    return *stream;
  }
    
    
  void Logger::setLevel(level_t level) {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setLevel(level);
    }
  }


  void Logger::mute() {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setSilent(true);
    }
  }


  void Logger::unmute() {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setSilent(false);
    }
  }

#undef __K_ENUMERATE_OVER_TYEPS

  
} // namespace kfoundation
