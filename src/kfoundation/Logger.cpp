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
  
  /**
   * Constructor, do not use directly. Instead, use Logger::addChannel().
   */
  
  Logger::Channel::Channel(const string& name, const string& fileName)
  : _name(name),
    _closeOnDeconstruct(true)
  {
    init();
    ofstream* ofs = new ofstream();
    ofs->open(fileName.c_str(), ofstream::out | ofstream::app);
    _os = ofs;
  }

  
  /**
   * Constructor, do not use directly. Instread, use Logger::addChannel().
   */
  
  Logger::Channel::Channel(const string& name, ostream* os)
  : _name(name),
    _closeOnDeconstruct(false)
  {
    init();
    _os = os;
  }

  
  /**
   * Deconstructor.
   */
  
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

  
  /**
   * Sets the filtering level. All messages with equal or higher level than
   * the given level will be passed and the rest will be filtered.
   *
   * @param level Filtering level
   */

  Logger::Channel& Logger::Channel::setLevel(level_t level) {
    _level = level;
    return *this;
  }
  
  
  /**
   * Returns the current filtering level.
   */
  
  Logger::level_t Logger::Channel::getLevel() const {
    return _level;
  }
  
  
  /**
   * Checks if the current filtering level equals the given parameter.
   *
   * @param lvl The level to check against.
   * @return The result of comparison.
   */
  
  bool Logger::Channel::checkLevel(level_t lvl) const {
    if(_isSilent) {
      return false;
    }
    
    return lvl <= _level;
  }
  
  
  /**
   * If the given parameter is `true`, this channel will no longer produce any
   * output. Otherwise, output will be produced with the given filtering level
   * applied.
   *
   * @param isSilent If set `true` this channel will be silenced, otherwise
   *        it will resume producing output.
   */
  
  void Logger::Channel::setSilent(bool isSilent) {
    _isSilent = isSilent;
  }
  
  
  /**
   * Checks if this channel is set to silent.
   *
   * @see setSilent()
   */
  
  bool Logger::Channel::isSilent() const {
    return _isSilent;
  }
  
  
  /**
   * Returns the name of this channel.
   */
  
  const string& Logger::Channel::getName() const {
    return _name;
  }
  
  
  /**
   * Checks if the name of this channel is the same as the given parameter.
   *
   * @param name The name to check against.
   * @return The result of comparison.
   */
  
  bool Logger::Channel::checkName(const string& name) const {
    return name == _name;
  }
  
  
  /**
   * The heading of each log entry can be customized using this function.
   *
   * @param printHourAndMinutes If set `true` the hour and minute will be 
   *        printed in HH:MM format.
   * @param printSecondAndMilisecond If set `true` second and milisecond
   *        will be printed in SS.m format. If printHourAndMinutes is already set to `true`
   *        the output will be in HH:MM:SS.m format.
   * @param printLocation If set `true` the location of the code is printed
   *        in [function_name\@file_name:line_number] format.
   */
  
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
  
  
  /**
   * Constructor, do not use directly. Use Logger::log() instread.
   */
  
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
  
  
  /**
   * Default constructor.
   */
  
  Logger::Logger() {
    // Nothing
  }

  
  /**
   * Deconstructor.
   */
  
  Logger::~Logger() {
    removeAllChannels();
  }


// --- METHODS --- //
  
  /**
   * Adds a channel that outputs to the given ostream object. Since
   * ostream is not a ManagedObejct it needs to be deleted by user if necessary.
   *
   * @param name A name for the new channel.
   * @param os The output stream to print to.
   */
  
  Logger::Channel& Logger::addChannel(const string& name, ostream* os) {
    Channel* ch = new Channel(name, os);
    _channels.push_back(ch);
    return *ch;
  }
  
  
  /**
   * Adds a channel that outputs to the given file. The new logs will be
   * appended to the existing contents of the file.
   *
   * @param name A name for the new channel.
   * @param fileName Path to the file to write to.
   */

  Logger::Channel& Logger::addChannel(const string& name, const string& fileName)
  {
    Channel* ch = new Channel(name, fileName);
    _channels.push_back(ch);
    return *ch;
  }
  
  
  /**
   * Returns refernce to the channel with the given name.
   *
   * @throw An exception if there is no channel with the given name.
   */
  
  Logger::Channel& Logger::getChannelByName(const string &name) const {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      if(_channels[i]->checkName(name)) {
        return *_channels[i];
      }
    }
    
    throw "Logger " + name + " does not have a channel with name \"" + name
        + "\"";
  }
  
  
  /**
   * Removes all channels.
   */
  
  void Logger::removeAllChannels() {
    for(vector<Channel*>::iterator it = _channels.begin();
        it != _channels.end(); it++)
    {
      Channel* ch = *it;
      delete ch;
    }
    
    _channels.clear();
  }

  
  /**
   * Not for direct use. Used by `LOG`, `LOG_XXX` and `DLOG_XXX` macros to 
   * create a new Logger::Stream.
   */
  
  Logger::Stream& Logger::log(level_t level, const char fileName[],
                      int lineNumber, const char functionName[])
  {
    Meta meta;
    meta.fileName = fileName;
    meta.lineNumber = lineNumber;
    meta.functionName = functionName;
    meta.level = level;
    
    Stream* stream = new Stream(meta, _channels);
    return *stream;
  }
  
  
  /**
   * Creates and initiates a new Logger:Stream with the given level.
   *
   * @param level Level of the log item.
   * @return Reference to a new Logger::Stream.
   */

  Logger::Stream& Logger::log(level_t level) {
    Meta meta;
    meta.level = level;
    
    // usleep(rand()%1000 + 200);
    Stream* stream = new Stream(meta, _channels);
    return *stream;
  }
  
  
  /**
   * Set the filtering level of all channels to the given value.
   *
   * @param The level to set.
   */
    
  void Logger::setLevel(level_t level) {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setLevel(level);
    }
  }

  
  /**
   * Sets all channels to silent.
   */

  void Logger::mute() {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setSilent(true);
    }
  }

  
  /**
   * Removes silence flag from all channels.
   */

  void Logger::unmute() {
    for(int i = (int)_channels.size() - 1; i >= 0; i--) {
      _channels.at(i)->setSilent(false);
    }
  }

#undef __K_ENUMERATE_OVER_TYEPS

  
} // namespace kfoundation
