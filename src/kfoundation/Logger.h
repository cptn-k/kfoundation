/*---[Logger.h]------------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common / Logging Facility
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

#ifndef ORG_KNORBA_COMMON_LOGGER_H
#define ORG_KNORBA_COMMON_LOGGER_H

#include "definitions.h"

#include <ctime>
#include <cstring>
#ifdef KF_UNIX
#  include <sys/time.h>
#else
#  error "Only UNIX/Linux is supported"
#endif

#include <pthread.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Streamer.h"
#include "System.h"

#ifdef LOG
  #undef LOG
#endif

#define LOG_TO(LOGGER, LEVEL) LOGGER.log(LEVEL, __FILE__, __LINE__, __func__)
#define _K__LOG(LEVEL) LOG_TO(System::getLogger(), LEVEL)
#define LOG_ERR _K__LOG(::kfoundation::Logger::ERR)
#define LOG_WRN _K__LOG(::kfoundation::Logger::WRN)
#define LOG_L1  _K__LOG(::kfoundation::Logger::L1)
#define LOG_L2  _K__LOG(::kfoundation::Logger::L2)
#define LOG_L3  _K__LOG(::kfoundation::Logger::L3)
#define LOG LOG_L3

#define DLOG_ERR(X) LOG_ERR << X << ::kfoundation::EL
#define DLOG_WRN(X) LOG_WRN << X << ::kfoundation::EL
#define DLOG_L1(X) LOG_L1 << X << ::kfoundation::EL
#define DLOG_L2(X) LOG_L2 << X << ::kfoundation::EL
#define DLOG_L3(X) LOG_L3 << X << ::kfoundation::EL

#define __K_ENUMERAND(X)
#define __K_ENUMERATE_OVER_TYPES \
  __K_ENUMERAND(string&)\
  __K_ENUMERAND(const string&)\
  __K_ENUMERAND(const char*)\
  __K_ENUMERAND(const wchar_t*)\
  __K_ENUMERAND(char)\
  __K_ENUMERAND(int)\
  __K_ENUMERAND(long int)\
  __K_ENUMERAND(long long int)\
  __K_ENUMERAND(float)\
  __K_ENUMERAND(double)\
  __K_ENUMERAND(long double)\
  __K_ENUMERAND(const Streamer&)
#undef __K_ENUMERAND


namespace kfoundation {

  using namespace std;
  
  
  /**
   * Flagges that can be passed to logger via << operator.
   */
  
  typedef enum {
    EL,     ///< End log. Shoud be at the end of each logged message.
    EL_CON  ///< End log and print to console.
  } logger_flag_t;

  
  /**
   * Multi-channel, muti-level logger utility. Normally, you want to use the
   * default logger already provided via System::getLogger(). LOG and LOG_XXX
   * macros expand to the default logger. So,
   *
   *     LOG << "Hello" << EL;
   *
   * is equivalant to
   *
   *     System::getLogger().log(Logger::L3) << "Hello" << EL;
   *
   * Calling log() method starts a log stream. The stream should end with EL 
   * in order to be flushed into the designated channels. Inbetween, any
   * of the following types can be used:
   *
   * * string
   * * char*
   * * wchar_t*
   * * bool
   * * char
   * * int
   * * long int
   * * long long int
   * * float
   * * double
   * * long double
   * * Streamer and any of its subclasses.
   *
   * The default logger has one channel that outputs to standard error console.
   * However it is possible add and remove channels manually via
   * removeAllChannels() and addChannel() methods.
   *
   * Log level determines the importance of the message being logged.
   * Ordered by importance from lowest to highest, these levels are 
   *
   * * L3
   * * L2
   * * L1
   * * WRN
   * * ERR
   *
   * A channel will filter any message with a level lower than
   * its designated level. For example a channel designated with WRN level,
   * will pass WRN and ERR but filters L3, L2, and L1.
   *
   * It is possible to reduce the amount of output produced by logger
   * by setting its level or using mute() method or
   * Logger::Channel::setSilent().
   *
   * Another way that helps the performance of your released program is to use 
   * DLOG_XXX macros. Usage:
   *
   *     DLOG_L3("myCounter: " << myCounter);
   *
   * And to disable logging define DLOG_L3 as nothing in the begining of your
   * file. For example:
   *
   *     #ifndef DEBUG
   *     #undef DLOG_L3
   *     #define DLOG_L3
   *     #endif
   *
   * This will remove the log part all together from your compiled code.
   *
   * To customize the header preceeding each log, use Channel::setFormat()
   * method.
   *
   * @ingroup utils
   * @headerfile Logger.h <kfoundation/Logger.h>
   */
   

  class Logger {

    // --- NESTED DATA TYPES --- //
    
    /**
     * Log level.
     */
    
    public: typedef enum {
      ERR = 0, ///< Error severity level
      WRN = 1, ///< Warning severity level
      L1  = 2, ///< Severity lower than WRN but higher than L2
      L2  = 3, ///< Severity lower than L1 but higher than L3
      L3  = 4, ///< The lowest severity level
    } level_t;
    
    
    struct Meta {
      string  fileName;
      string  functionName;
      int     lineNumber;
      level_t level;
    };
    
    
    /**
     * Logger channel.
     * @headerfile Logger.h <kfoundation/Logger.h>
     */

    class Channel {
      
    // --- FIELDS --- //
      
      private: string   _name;
      private: ostream* _os;
      private: bool     _closeOnDeconstruct;
      private: bool     _isSilent;
      private: bool     _printHourAndMinute;
      private: bool     _printSecondAndMilisecond;
      private: bool     _printLocation;
      private: level_t  _level;
      private: pthread_mutex_t _mutex;
      
      
    // --- (DE)CONSTRUCTORS --- //

      public: Channel(const string& name, const string& fileName);
      public: Channel(const string& name, ostream* os);
      public: ~Channel();
      
      
    // --- METHODS --- //
      
      private: void init();
      public: void print(const stringstream& sstream, const Meta& meta);
      public: Channel& setLevel(level_t level);
      public: level_t getLevel() const;
      public: bool checkLevel(level_t lvl) const;
      public: void setSilent(bool isSilent);
      public: bool isSilent() const;
      public: const string& getName() const;
      public: bool checkName(const string& name) const;
      
      public: Channel& setFormat(bool printHourAndMinutes,
          bool printSecondAndMilis, bool printLocation);
      
    }; // class Channel

    
    /**
     * Log stream. Created when logger::log() method is called and distroyed
     * once EL is passed to it.
     * @headerfile Logger.h <kfoundation/Logger.h>
     */

    class Stream {
      
    // --- FIELDS --- //
      
      private: Meta              _meta;
      private: bool              _isOpen;
      private: vector<Channel*>& _channels;
      private: stringstream      _sstream;
      
      
    // --- (DE)CONSTRUCTORS --- //

      public: Stream(const Meta& meta, vector<Channel*>& channels);
      
      
    // --- METHODS --- //
      
      #define __K_ENUMERAND(X) public: Stream& operator<<(X a);
      __K_ENUMERATE_OVER_TYPES
      #undef __K_ENUMERAND
      
      public: Stream& operator<<(bool a);
      public: void operator<<(logger_flag_t f);
      
    }; // class Stream


  // --- FIELDS --- //
    
    private: vector<Channel*> _channels;
    private: pthread_mutexattr_t _mutexAttribs;
    
    
  // --- (DE)CONSTRUCTORS --- //

    public: Logger();
    public: ~Logger();
    
    
  // --- METHODS --- //
    
    public: Channel& addChannel(const string& name, ostream* os);
    public: Channel& addChannel(const string& name, const string& fileName);
    public: vector<string> getChannelNames() const;
    public: Channel& getChannelByName(const string& name) const;
    public: void removeAllChannels();

    public: Stream& log(level_t level, const char fileName[], int lineNumebr,
        const char functionName[]);

    public: Stream& log(level_t level);
    public: void setLevel(level_t level);
    public: void mute();
    public: void unmute();

  }; // class Logger

} // namespace kfoundation

#endif
