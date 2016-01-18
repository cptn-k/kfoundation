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
#include <iostream>

// Unix
#ifdef KF_UNIX
#  include <pthread.h>
#  include <unistd.h>
#  include <errno.h>
#  include <dlfcn.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <sys/time.h>
#  include <execinfo.h>
#  include <syslog.h>
#endif

// Mac
#ifdef KF_MAC
#  include <mach-o/dyld.h>
#endif

// Application Binary Interface
#include <cxxabi.h>

// Internal
#include "Ref.h"
#include "MemoryManager.h"
#include "Logger.h"
#include "Path.h"
#include "Int.h"
#include "KFException.h"
#include "StandardInputStreamAdapter.h"
#include "StandardOutputStreamAdapter.h"
#include "BufferOutputStream.h"
#include "OutputStream.h"
#include "PrintWriter.h"
#include "UString.h"

// Self
#include "System.h"

#define __KF_BUFFER_SIZE 400


kfoundation::__SystemImpl* __k_system = NULL;
typedef void (*__k_takeoverfunction_t)(kfoundation::__SystemImpl*);


namespace kfoundation {

//\/ VoidOutputStream /\///////////////////////////////////////////////////////

  class VoidOutputStream : public OutputStream {

  // --- CONSTRUCTORS --- //

    public: VoidOutputStream();


  // --- METHODS --- //

    public: void write(const kf_octet_t* buffer, const kf_int32_t nOctets);
    public: void write(const kf_octet_t);
    public: void write(Ref<InputStream> is);
    public: void close();
    public: bool isBigEndian() const;
    public: void flush();

  };


  VoidOutputStream::VoidOutputStream() {
    // Nothing;
  }


  void VoidOutputStream::write(const kf_octet_t*, const kf_int32_t) {
    // Nothing;
  }


  void VoidOutputStream::write(const kf_octet_t) {
    // Nothing
  }


  void VoidOutputStream::write(Ref<InputStream> is) {
    // Nothing
  }


  void VoidOutputStream::close() {
    // Nothing
  }


  bool VoidOutputStream::isBigEndian() const {
    return System::isBigEndian();
  }


  void VoidOutputStream::flush() {
    // Nothing;
  }


//\/ System /\/////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  MasterMemoryManager* System::master = NULL;
  StaticRef<Logger> System::logger = NULL;

  const StaticRef<PrintWriter> System::VOID = new PrintWriter(
      StaticRef<OutputStream>(new VoidOutputStream()));

  const StaticRef<PrintWriter> System::OUT = new PrintWriter(
      StaticRef<OutputStream>(new StandardOutputStreamAdapter(std::cout)));

  const StaticRef<PrintWriter> System::ERR = new PrintWriter(
      StaticRef<OutputStream>(new StandardOutputStreamAdapter(std::cerr)));

  const StaticRef<InputStream> System::IN = new StandardInputStreamAdapter(std::cin);


// --- STATIC METHODS --- //

  /**
   * Returns executable path for the current process. Symlinks are resolved
   * internally.
   */

#ifdef KF_MAC
  Ref<Path> System::getExePath() {
    uint32_t size = 1;
    char shortBuffer[10];
    char* buffer;
    _NSGetExecutablePath(shortBuffer, &size);
    buffer = new char[size + 1];
    _NSGetExecutablePath(buffer, &size);
    Path* res = new Path(new UString((kf_octet_t*)buffer, size - 1));
    delete[] buffer;

    return res;
  }
#elif defined(KF_LINUX)
  Ref<Path> __SystemImpl::getExePath() {
    if(_exePath.isNull()) {
      string linkPath = "/proc/self/exe";
      char buffer[__KF_BUFFER_SIZE];
      ssize_t nBytes = readlink(linkPath.c_str(), buffer, __KF_BUFFER_SIZE);
      if(nBytes == -1) {
        throw KFException(K"Internal error resolving symbolik link "
                          + linkPath + ". Reason: " + System::getLastSystemError());
      }

      Path* _exePath = new Path(new UString((kf_octet_t*)buffer, nBytes));
    }

    return _exePath;
  }
#endif

  /**
   * Returns the current working directory.
   */

  Ref<Path> System::getCurrentWorkingDirectory() {
    char buffer[1024];
    if(getcwd(buffer, 1024) != NULL) {
      return new Path(new UString(buffer));
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
  
  Ref<UString> System::demangle(const char* symbol) {
    size_t len = __KF_BUFFER_SIZE;
    char* output = (char*)malloc(len);
    int status;
    output = abi::__cxa_demangle(symbol, output, &len, &status);
    
    if(output != NULL) {
      Ref<UString> outstr = new UString(output);
      free(output);
      return outstr;
    }
    
    return new UString(symbol);
  }

  
  /**
   * Given pointer to a symbol, returns its name.
   */
  
  Ref<UString> System::resolveSymbolName(void *ptr) {
  #if defined(KF_UNIX)
    Dl_info info;
    int code = dladdr(ptr, &info);
    if(code != 0) {
      return new UString(info.dli_sname);
    }
    return new UString();
  #else
  #  error "Platform not supported"
  #endif
  }

  
  /**
   * Returns the human-readable description of the last system error as
   * indicated by errno variable.
   */

  Ref<UString> System::getLastSystemError() {
    char buffer[200];
    strerror_r(errno, buffer, 200);
    return new UString(buffer);
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

  Ref<Logger> System::getLogger() {
    if(logger.isNull()) {
      logger = new Logger();
      logger->addChannel(K"default", ERR->getStream());
    }
    return logger;
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
    throw KFException(K"Not implemented");
  }

  
  /**
   * Returns reference to the master memory manager.
   */

  MasterMemoryManager& System::getMasterMemoryManager() {
    if(IS_NULL(master)) {
      master = new MasterMemoryManager();
    }
    return *master;
  }


  void System::takeover(void* libHandle) {
    throw KFException(K"Not implemented");
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
  throw "Not implemented";
}

#undef __KF_BUFFER_SIZE