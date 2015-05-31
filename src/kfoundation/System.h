/*---[System.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: System
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
#include "PtrDecl.h"

using namespace std;

namespace kfoundation {

  class __SystemImpl;
  class Logger;
  class Path;
  class Runnable;
  class InputStream;
  class OutputStream;
    
  class System {
    
  // --- NESTED TYPES --- //
    
    public: typedef enum {
      LINUX,
      FREE_BSD,
      SOLARIS,
      APPLE_OS_X,
      MACH,
      UNSUPPORTED
    } operating_system_t;
    
    typedef pthread_t thread_t;
    
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<InputStream> IN;
    public: static const SPtr<OutputStream> OUT;
    public: static const SPtr<OutputStream> ERR;
    
    
  // --- STATIC METHODS --- //

    private: static void* getSystem();
    public: static PPtr<Path> getExePath();
    public: static Ptr<Path> getCurrentWorkingDirectory();
    public: static bool isBigEndian();
    public: static string demangle(string str);
    public: static string resolveSymbolName(void* ptr);
    public: static string getLastSystemError();
    public: static operating_system_t getOperatingSystem();
    public: static Logger& getLogger();
    public: static void sleep(const int msecs);
    public: static int exec(const char* command, char** args, int argc);
    public: static MasterMemoryManager& getMasterMemoryManager();
    public: static void takeover(void* libHandle);
    public: static kf_int64_t getCurrentTimeInMiliseconds();
    public: static int getPid();
    public: static int backtrace(void** addrList, int countLimit);
    public: static char** backtraceSymbols(void** addrList, int count);
    
  };

  
} // namespace kfoundation

extern "C" void __k_systemTakeOver(kfoundation::__SystemImpl*);


#endif