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
  
  typedef enum {
    EL,
    EL_CON
  } logger_flag_t;


  class Logger {

    // --- NESTED DATA TYPES --- //
    
    public: typedef enum {
      ERR = 0, /**< Error severity level    */
      WRN = 1, /**< Warning severity level  */
      L1  = 2, /**< Severity lower than WRN but higher than L2 */
      L2  = 3, /**< Severity lower than L1 but higher than L3  */
      L3  = 4, /**< The lowest severity level */
    } level_t;
    
    
    struct Meta {
      string  fileName;
      string  functionName;
      int     lineNumber;
      level_t level;
    };
    

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
