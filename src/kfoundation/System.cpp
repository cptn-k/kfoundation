/*---[System.cpp]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::__SystemImpl::*
 |  Implements: kfoundation::System::*
 |              kfoundation::__SystemImpl::*
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

#include "definitions.h"

// Std
#include <cstdlib>

// Unix
#ifdef KF_UNIX
#  include <pthread.h>
#  include <unistd.h>
#  include <errno.h>
#  include <dlfcn.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <execinfo.h>
#endif

// Mac
#ifdef KF_MAC
#  include <mach-o/dyld.h>
#endif

// Application Binary Interface
#include <cxxabi.h>

// Internal
#include "Ptr.h"
#include "MemoryManager.h"
#include "Logger.h"
#include "Path.h"
#include "Int.h"
#include "KFException.h"
#include "StandardInputStreamAdapter.h"
#include "StandardOutputStreamAdapter.h"

// Self
#include "System.h"

#define __KF_BUFFER_SIZE 400

using namespace std;

kfoundation::__SystemImpl* __k_system = NULL;
typedef void (*__k_takeoverfunction_t)(kfoundation::__SystemImpl*);


namespace kfoundation {
  
//\/ __SystemImpl /\///////////////////////////////////////////////////////////
  
  class __SystemImpl {
    
  // --- FIELDS --- //
    
    private: Logger _logger;
    private: Ptr<Path> _exePath;
    private: MasterMemoryManager _memoryManager;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: __SystemImpl();
    

  // --- STATIC METHODS --- //
    
    public: static string resolveSymlink(const string&);
    
    
  // --- METHODS --- //
    
    public: PPtr<Path> getExePath();
    public: inline Logger& getLogger();
    public: inline MasterMemoryManager& getMasterMemoryManager();
    
  };
  
  __SystemImpl::__SystemImpl()
  : _logger()
  {
    _logger.addChannel("default", &cout);
  }
  
  
#ifdef KF_MAC
  PPtr<Path> __SystemImpl::getExePath() {
    if(_exePath.isNull()) {
      uint32_t size = 1;
      char shortBuffer[10];
      char* buffer;
      _NSGetExecutablePath(shortBuffer, &size);
      buffer = new char[size + 1];
      _NSGetExecutablePath(buffer, &size);
      _exePath = new Path(string(buffer, size - 1));
      delete[] buffer;
    }
    
    return _exePath;
  }
#elif defined(KF_LINUX)
  PPtr<Path> __SystemImpl::getExePath() {
    if(_exePath.isNull()) {
      string linkPath = "/proc/self/exe";
      char buffer[__KF_BUFFER_SIZE];
      ssize_t nBytes = readlink(linkPath.c_str(), buffer, __KF_BUFFER_SIZE);
      if(nBytes == -1) {
        throw KFException("Internal error resolving symbolik link "
            + linkPath + ". Reason: " + System::getLastSystemError());
      }
      _exePath = new Path(string(buffer, nBytes));
    }
    
    return _exePath;
  }
#endif
  

  inline Logger& __SystemImpl::getLogger() {
    return _logger;
  }
  
  
  inline MasterMemoryManager& __SystemImpl::getMasterMemoryManager() {
    return _memoryManager;
  }
  
} // namespace kfoundation

inline kfoundation::__SystemImpl* __k_getSystem() {
  if(__k_system == NULL) {
    timeval tv;
    gettimeofday(&tv, NULL);
    srand((unsigned int)tv.tv_usec);
    __k_system = new kfoundation::__SystemImpl();
  }
  return __k_system;
}

namespace kfoundation {

//\/ System /\/////////////////////////////////////////////////////////////////
  
  
  //const SPtr<InputStream> System::IN = new StandardInputStreamAdapter(cin);
  //const SPtr<OutputStream> System::OUT = new StandardOutputStreamAdapter(cout);
  //const SPtr<OutputStream> System::ERR = new StandardOutputStreamAdapter(cerr);
  
  void* System::getSystem() {
    return __k_getSystem();
  }

  
  /**
   * Returns executable path for the current process. Symlinks are resolved
   * internally.
   */

  PPtr<Path> System::getExePath() {
    return __k_getSystem()->getExePath();
  }

  
  /**
   * Returns the current working directory.
   */

  Ptr<Path> System::getCurrentWorkingDirectory() {
    char buffer[1024];
    if(getcwd(buffer, 1024) != NULL) {
      return new Path(string(buffer));
    }
    return NULL;
  }

  
  /**
   * Used to check if the current platform is big-endian.
   */
  
  bool System::isBigEndian() {
    return true;
  }
  
  
  /**
   * Demangles a C++ ABI symbol into a human readable one.
   */
  
  string System::demangle(string str) {
    size_t len = __KF_BUFFER_SIZE;
    char* output = (char*)malloc(len);
    int status;
    output = abi::__cxa_demangle(str.c_str(), output, &len, &status);
    
    if(output != NULL) {
      string outstr(output);
      free(output);
      return outstr;
    }
    
    return str;
  }

  
  /**
   * Given pointer to a symbol, returns its name.
   */
  
  string System::resolveSymbolName(void *ptr) {
  #if defined(KF_UNIX)
    Dl_info info;
    int code = dladdr(ptr, &info);
    if(code != 0) {
      return string(info.dli_sname);
    }
    return "";
  #else
  #  error "Platform not supported"
  #endif
  }

  
  /**
   * Returns the human-readable description of the last system error as
   * indicated by errno variable.
   */

  string System::getLastSystemError() {
    char buffer[200];
    strerror_r(errno, buffer, 200);
    return string(buffer);
  }


  /**
   * Return type of the undelying operating system.
   */
  
  System::operating_system_t System::getOperatingSystem() {
  #if defined(KF_MAC)
    return APPLE_OS_X;
  #elif defined(KF_MACH)
    return System::MACH;
  #elif defined(KF_SOLARIS)
    return System::SOLARIS;
  #elif defined(KF_FREE_BSD)
    return System::FREE_BSD;
  #elif defined(KF_LINUX)
    return System::LINUX;
  #else
    return System::UNSUPPORTED;
  #endif
  }

  
  /**
   * Returns the default system logger.
   */

  Logger& System::getLogger() {
    return __k_getSystem()->getLogger();
  }

  
  /**
   * Causes the current threed to sleep for the given number of miliseconds.
   *
   * Most platforms provide sleep() which works with 1-second resolution, and
   *   usleep() which works with 1-microsecond resolution. System::sleep()
   *   provides milisecond resolution which makes sense in most devices, and
   *   has the same interface as Java.System.sleep(). Can be used in
   *   conjunction with System::getCurrentTimeInMiliseconds() without the fuss
   *   of timeval struct.
   *
   * @param msecs The number of miliseconds to sleep.
   */
  
  void System::sleep(const int msecs) {
    ::usleep(msecs*1000);
  }


  /**
   * Executes the given command with the given set of arguments.
   * @param command The command to be executed.
   * @param args An array of C strings, each containing a signle argument.
   * @param argc The number of elements in args.
   */
  
  int System::exec(const char* command, char **args, int argc) {
    throw KFException("Not implemented");
  }

  
  /**
   * Returns reference to the master memory manager.
   */

  MasterMemoryManager& System::getMasterMemoryManager() {
    return __k_getSystem()->getMasterMemoryManager();
  }


  void System::takeover(void* libHandle) {
    void* ptr = dlsym(libHandle, "__k_systemTakeOver");
    __k_takeoverfunction_t fn = (__k_takeoverfunction_t)ptr;
    fn(__k_getSystem());
  }
  
  
  /**
   * Returns the current time in miliseconds from the standard origin time.
   * The origin time is usually midnigh January 1, 1970 UTC.
   */
  
  kf_int64_t System::getCurrentTimeInMiliseconds() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return (kf_int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
  }
  
  
  /**
   * Returns the process id for this process.
   */
  
  int System::getPid() {
    return ::getpid();
  }
  
  
  int System::backtrace(void** addrList, int countLimit) {
    return ::backtrace(addrList, countLimit);
  }
  
  
  char** System::backtraceSymbols(void** addrList, int count) {
    return ::backtrace_symbols(addrList, count);
  }
    
} // namespace kfoundation


KF_EXPORT
void __k_systemTakeOver(kfoundation::__SystemImpl* impl) {
  using namespace kfoundation;
  if(__k_system != NULL) {
    __k_system->getMasterMemoryManager().migrate(impl->getMasterMemoryManager());
    delete __k_system;
  }
  __k_system = impl;
}


#undef __KF_BUFFER_SIZE
