/*---[definitions.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
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

#ifndef ORG_KNORBA_COMMON_COMMON_H
#define ORG_KNORBA_COMMON_COMMON_H

#define KF_FAST_PTR

#ifdef sun
  #undef sun
  #ifndef __sun
    #define __sun
  #endif
#endif

#ifdef __linux__
#  define KF_LINUX
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#if defined(__APPLE__)
#  define KF_MAC
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#if defined(__MACH__) && !defined(__APPLE__)
#  define KF_MACH
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#ifdef __sun
#  define KF_SOLARIS
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#ifdef __FreeBSD__
#  define KF_FREE_BSD
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#ifndef KF_SUPPORTED
#  error "The current platform is not supported"
#endif

#if defined(KF_LINUX) || defined(KF_MAC) || defined(KF_MACH) || defined(KF_SOLARIS)\
      || defined(KF_FREE_BSD)
#  define KF_UNIX
#endif

#ifndef NULL
  #define NULL 0
#endif

#define NOT_NULL(X) (X)
#define IS_NULL(X) !(X)

#define INSTANCEOF(A, B) (dynamic_cast<B*>(A) != NULL)

#define KF_EXPORT __attribute__((visibility("default")))

#define KF_NOP while(false){}

namespace kfoundation {
  
  typedef enum {
    SMALLER = -1,
    EQUAL = 0,
    GREATER = 1
  } kf_comparison_result_t;
  
  typedef unsigned char kf_octet_t;
  typedef char          kf_int8_t;
  typedef short int     kf_int16_t;
  typedef int           kf_int32_t;
  typedef long int      kf_int64_t;
  
} // namespace kfoundation

#endif