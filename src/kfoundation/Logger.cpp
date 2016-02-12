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

// Unix
#include <syslog.h>
#include <cstdlib>

namespace {
  const int SYS_LOG_ERR = LOG_ERR;
}

// Internal
#include "definitions.h"
#include "BufferOutputStream.h"
#include "Ref.h"
#include "System.h"
#include "RefDictionary.h"
#include "Path.h"

// Self
#include "Logger.h"

namespace kfoundation {

//\/ Logger::Writer /\/////////////////////////////////////////////////////////

// --- CONSTRUCTOR --- //

  Logger::Writer::Writer(Ref<Dic> channels, Mutex& mutex, level_t sysLogLevel)
  : PrintWriter(new BufferOutputStream()),
    _channels(channels),
    _mutex(mutex),
    _sysLogLevel(sysLogLevel)
  {
    _buffer = getStream().AS(BufferOutputStream);
  }


// --- METHODS --- //

  void Logger::Writer::over() {
    newLine();
    _buffer->write(0);

    const kf_octet_t* buffer = _buffer->getData();
    const kf_int32_t size = _buffer->getSize();

    System::sleep(rand()%10);
    Dic::Iterator it = _channels->getIterator();
    _mutex.lock();
    for(it.first(); it.hasMore(); it.next()) {
      it.getValue()->write(buffer, size);
    }
    _mutex.unlock();

    switch (_sysLogLevel) {
      case ERR:
        syslog(SYS_LOG_ERR, "%s", (char*)buffer);
        break;

      case WRN:
        syslog(SYS_LOG_ERR, "%s", (char*)buffer);
        break;


      case L1:
        syslog(SYS_LOG_ERR, "%s", (char*)buffer);
        break;

      case L2:
        syslog(SYS_LOG_ERR, "%s", (char*)buffer);
        break;

      case L3:
        syslog(SYS_LOG_ERR, "%s\n", (char*)buffer);

      default:
        break;
    }
  }


//\/ Logger /\/////////////////////////////////////////////////////////////////

// --- (DE)CONSTRUCTORS --- //

  /**
   * Default constructor.
   */
  
  Logger::Logger()
  : _channels(new Dic()),
    _mutex(),
    _shortTime(ON),
    _printLocation(ON),
    _systemLog(ON),
    _level(L3)
  {
    // Nothing
  }


// --- METHODS --- //

  PrintWriter& Logger::printTime(PrintWriter& writer) {
    kf_int64_t time = System::getCurrentTimeInMiliseconds();
    kf_int32_t millis = time%1000;

    if(IS_ON(_shortTime)) {
      writer.printTime(K"%S.", time);
    } else {
      writer.printTime(K"%H:%M:%S.", time);
    }

    writer << millis;

    return writer;
  }


  /**
   * Adds a channel that outputs to the given ostream object. Since
   * ostream is not a ManagedObejct it needs to be deleted by user if necessary.
   *
   * @param name A name for the new channel.
   * @param os The output stream to print to.
   */

  void Logger::addChannel(RefConst<UString> name, Ref<OutputStream> stream) {
    _channels->set(name, stream);
  }


  /**
   * Returns refernce to the channel with the given name.
   *
   * @throw An exception if there is no channel with the given name.
   */
  
  Ref<OutputStream> Logger::getChannelByName(RefConst<UString> name) const {
    return _channels->get(name);
  }
  
  
  /**
   * Removes all channels.
   */
  
  void Logger::removeAllChannels() {
    _channels->clear();
  }

  
  /**
   * Not for direct use. Used by `LOG`, `LOG_XXX` and `DLOG_XXX` macros to 
   * create a new Logger::Stream.
   */
  
  Ref<PrintWriter> Logger::log(level_t level, const char* fileName,
      kf_int32_t lineNumber, const char* functionName)
  {
    if(level > _level || _level == MUTE) {
      return System::VOID;
    }

    Ref<PrintWriter> writer
        = new Writer(_channels, _mutex, IS_ON(_systemLog)?_level:MUTE);

    printTime(*writer) << " [" << UString(functionName) << L'@'
        << Path(K fileName).getFileNameWithExtension()
        << L':' << lineNumber << "] ";

    switch(level) {
      case ERR:
        writer->print("!!! ");
        break;

      case WRN:
        writer->print("! ");

      default:
        break;
    }

    return writer;
  }
  
  
  /**
   * Creates and initiates a new Logger:Stream with the given level.
   *
   * @param level Level of the log item.
   * @return Reference to a new Logger::Stream.
   */

  Ref<PrintWriter> Logger::log(level_t level) {
    if(level > _level || _level == MUTE) {
      return System::VOID;
    }

    Ref<PrintWriter> writer
        = new Writer(_channels, _mutex, IS_ON(_systemLog)?_level:MUTE);

    printTime(*writer);

    switch(level) {
      case ERR:
        writer->print("!!! ");
        break;

      case WRN:
        writer->print("! ");

      default:
        break;
    }
    
    return writer;
  }
  
  
  /**
   * Set the filtering level of all channels to the given value.
   *
   * @param The level to set.
   */
    
  void Logger::setLevel(level_t level) {
    _level = level;
  }


  void Logger::setPrintLocation(kf_onoff_t state) {
    _printLocation = state;
  }


  void Logger::setPrintShortTime(kf_onoff_t state) {
    _shortTime = state;
  }


  void Logger::setSystemLoggerOutput(kf_onoff_t state) {
    _systemLog = state;
  }

#undef __K_ENUMERATE_OVER_TYEPS
  
} // namespace kfoundation