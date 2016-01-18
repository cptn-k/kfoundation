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

// Internal
#include "definitions.h"
#include "RefDictionaryDecl.h"
#include "PrintWriter.h"
#include "Mutex.h"
#include "RefDecl.h"

// Super
#include "KFObject.h"

#ifdef LOG
  #undef LOG
#endif

#ifdef LOG_ERR
  #undef LOG_ERR
#endif

#define LOG_TO(LOGGER, LEVEL) *LOGGER->log(LEVEL, __FILE__, __LINE__, __func__)
#define _K__LOG(LEVEL) LOG_TO(System::getLogger(), LEVEL)
#define LOG_ERR _K__LOG(::kfoundation::Logger::ERR)
#define LOG_WRN _K__LOG(::kfoundation::Logger::WRN)
#define LOG_L1  _K__LOG(::kfoundation::Logger::L1)
#define LOG_L2  _K__LOG(::kfoundation::Logger::L2)
#define LOG_L3  _K__LOG(::kfoundation::Logger::L3)
#define LOG LOG_L3

#define DLOG_ERR(X) LOG_ERR << X << ::kfoundation::OVER
#define DLOG_WRN(X) LOG_WRN << X << ::kfoundation::OVER
#define DLOG_L1(X) LOG_L1 << X << ::kfoundation::OVER
#define DLOG_L2(X) LOG_L2 << X << ::kfoundation::OVER
#define DLOG_L3(X) LOG_L3 << X << ::kfoundation::OVER
#define DLOG(X) DLOG_L3(X)


namespace kfoundation {

  class BufferOutputStream;

  /**
   * Multi-channel, muti-level logger utility. Normally, you want to use the
   * default logger already provided via System::getLogger(). LOG and LOG_XXX
   * macros expand to the default logger. So,
   *
   *     LOG << "Hello" << ENDS;
   *
   * is equivalant to
   *
   *     System::getLogger().log(Logger::L3) << "Hello" << ENDS;
   *
   * Calling log() method starts a log stream. The stream should end with ENDS 
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
   

  class Logger : public KFObject {

  // --- NESTED DATA TYPES --- //

    /**
     * Log level.
     */
    
    public: typedef enum {
      MUTE = 0,
      ERR  = 1, ///< Error severity level
      WRN  = 2, ///< Warning severity level
      L1   = 3, ///< Severity lower than WRN but higher than L2
      L2   = 4, ///< Severity lower than L1 but higher than L3
      L3   = 5, ///< The lowest severity level
    } level_t;
    
    
    public: class Dic : public RefDictionary<UString, OutputStream> {
      // Nothing;
    };


    public: class Writer : public PrintWriter {

    // --- FIELDS --- //

      private: Ref<BufferOutputStream> _buffer;
      private: Ref<Dic> _channels;
      private: Mutex& _mutex;
      private: level_t _sysLogLevel;


    // --- CONSTRUCTOR --- //

      public : Writer(Ref<Dic> channels, Mutex& mutex, level_t sysLogLevel);


    // --- METHODS --- //

      // From PrintWriter
      public : void over();

    };

    
  // --- FIELDS --- //
    
    private: Ref<Dic>   _channels;
    private: Mutex      _mutex;
    private: kf_onoff_t _shortTime;
    private: kf_onoff_t _printLocation;
    private: kf_onoff_t _systemLog;
    private: level_t    _level;
    
    
  // --- (DE)CONSTRUCTORS --- //

    public: Logger();

    
  // --- METHODS --- //

   private: PrintWriter& printTime(PrintWriter& writer);
    public: void addChannel(RefConst<UString> name, Ref<OutputStream> stream);
    public: RefArray<UString> getChannelNames() const;
    public: Ref<OutputStream> getChannelByName(RefConst<UString> name) const;
    public: void removeAllChannels();

    public: Ref<PrintWriter> log(level_t level, const char* fileName,
        kf_int32_t lineNumebr, const char* functionName);

    public: Ref<PrintWriter> log(level_t level);
    public: void setLevel(level_t level);
    public: void setPrintShortTime(kf_onoff_t state);
    public: void setPrintLocation(kf_onoff_t state);
    public: void setSystemLoggerOutput(kf_onoff_t state);

  }; // class Logger

} // namespace kfoundation

#endif