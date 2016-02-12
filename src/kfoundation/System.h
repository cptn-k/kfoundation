/*---[System.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::System::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef ORG_KNORBA_COMMON_SYSTEM_H
#define ORG_KNORBA_COMMON_SYSTEM_H

// Internal
#include "RefDecl.h"

namespace kfoundation {

  class __SystemImpl;
  class Logger;
  class Path;
  class Runnable;
  class InputStream;
  class OutputStream;
  class PrintWriter;
  class UString;
  
  /**
   * @brief Provides a cross-platform way to access sytsem features. It cannot
   *        be instantiated.
   * @ingroup utils
   * @headerfile System.h <kfoundation/System.h>
   */
    
  class System {
    
  // --- NESTED TYPES --- //
    
    /**
     * Operating system type
     */
    
    public: typedef enum {
      LINUX,        ///< Linux
      FREE_BSD,     ///< Free BSD
      SOLARIS,      ///< Solaris
      APPLE_OS_X,   ///< Apple Mac OS X
      MACH,         ///< Mach
      UNSUPPORTED   ///< Other / Unsupported
    } operating_system_t;
    
    
  // --- STATIC FIELDS --- //

    private: static MasterMemoryManager* master;
    private: static StaticRef<Logger> logger;
    public: static const StaticRef<PrintWriter> VOID;
    public: static const StaticRef<PrintWriter> OUT;
    public: static const StaticRef<PrintWriter> ERR;
    public: static const StaticRef<InputStream> IN;
    
    
  // --- STATIC METHODS --- //

    public: static Ref<Path>    getExePath();
    public: static Ref<Path>    getCurrentWorkingDirectory();
    public: static bool         isBigEndian();
    public: static RefConst<UString> demangle(RefConst<UString> symbol);
    public: static Ref<UString> resolveSymbolName(void* ptr);
    public: static Ref<UString> getLastSystemError();
    public: static operating_system_t getOperatingSystem();
    public: static Ref<Logger>  getLogger();
    public: static void         sleep(const kf_int32_t msecs);
    public: static int          exec(const char* command, char** args, kf_int32_t argc);
    public: static MasterMemoryManager& getMasterMemoryManager();
    public: static void         takeover(void* libHandle);
    public: static kf_int64_t   getCurrentTimeInMiliseconds();
    public: static kf_int32_t   getPid();
    public: static kf_int16_t   getNumberOfCpuCores();
    public: static kf_int32_t   backtrace(void** addrList, kf_int32_t countLimit);
    public: static char**       backtraceSymbols(void** addrList, kf_int32_t count);
    public: static void         exit(kf_int32_t code = 0);
    public: static void         exitAfterAllThreads();
    
  };
  
} // namespace kfoundation


extern "C" void __k_systemTakeOver(kfoundation::__SystemImpl*);


#endif