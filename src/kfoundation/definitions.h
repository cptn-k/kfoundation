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

#ifdef sun
#  undef sun
#  ifndef __sun
#    define __sun
#  endif
#endif


/**
 * @def KF_LINUX
 * @brief is defined if the target platform is linux.
 * @ingroup defs
 */

/**
 * @def KF_SUPPORTED
 * @brief is defined when the target platform is supported.
 * @ingroup defs
 */

#if defined(__linux__) || defined(__doxygen__)
#  define KF_LINUX
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif



/**
 * @def KF_MAC
 * @brief is defined when the target platform is Mac OS.
 * @ingroup defs
 */

#if defined(__APPLE__) || defined(__doxygen__)
#  define KF_MAC
#  ifndef KF_SUPPORTED
#define KF_SUPPORTED
#  endif
#endif


/**
 * @def KF_MACH
 * @brief is defined when the target platform is based on Mach kernel.
 * @ingroup defs
 */

#if (defined(__MACH__) && !defined(__APPLE__)) || defined(__doxygen__)
#  define KF_MACH
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif


/**
 * @def KF_SOLARIS
 * @brief is defined when the target platform is Solaris.
 * @ingroup defs
 */

#if defined(__sun) || defined(__doxygen__)
#  define KF_SOLARIS
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif


/**
 * @def KF_FREE_BSD
 * @brief is defined when the target platform is FreeBSD.
 * @ingroup defs
 */

#if defined(__FreeBSD__) || defined(__doxygen__)
#  define KF_FREE_BSD
#  ifndef KF_SUPPORTED
#    define KF_SUPPORTED
#  endif
#endif

#ifndef KF_SUPPORTED
#  error "The current platform is not supported"
#endif


/**
 * @def KF_UNIX
 * @brief is defined when the target platform is UNIX-based.
 */

#if defined(KF_LINUX) || defined(KF_MAC) || defined(KF_MACH) || defined(KF_SOLARIS)\
      || defined(KF_FREE_BSD)
#  define KF_UNIX
#endif


/**
 * @def NULL
 * @brief is assured to be defined as zero.
 */

#ifndef NULL
  #define NULL 0
#endif


/**
 * @breif Tests if the argument is not null.
 * @ingroup defs
 * Recommended to be used to improve code readability.
 */

#define NOT_NULL(X) (X)


/**
 * @breif Tests if the argument is null.
 * @ingroup defs
 * Recommended to be used to improve code readability.
 */

#define IS_NULL(X) !(X)


/**
 * Checks object A is an instance of class B.
 */

#define INSTANCEOF(A, B) (dynamic_cast<B*>(A) != NULL)


/**
 * @brief Used to mark exported symbols. 
 * @ingroup defs
 * Recommended to be used in place of compiler-specific method.
 */

#define KF_EXPORT __attribute__((visibility("default")))


/**
 * @brief Used for consuming semicolon in other macros. 
 * @ingroup defs
 * Performs no operation.
 */

#define KF_NOP while(false){}


namespace kfoundation {
  
  /**
   * @brief Encodes the comparison result between two values
   * @ingroup defs
   */
  
  typedef enum {
    SMALLER = -1, ///< Smaller
    EQUAL = 0,    ///< Eqla
    GREATER = 1   ///< Greater
  } kf_comparison_t;

  
  /**
   * @brief 8-bit unsigned numeric type.
   * @ingroup defs
   */
  
  typedef unsigned char kf_octet_t;
  
  
  /**
   * @brief 8-bit signed numeric type.
   * @ingroup defs
   */
  
  typedef char kf_int8_t;
  
  
  /**
   * @brief 16-bit signed numeric type.
   * @ingroup defs
   */
  
  typedef short int kf_int16_t;
  
  
  /**
   * @brief 32-bit signed numeric type.
   * @ingroup defs
   */
  
  typedef int kf_int32_t;
  
  
  /**
   * @brief 64-bit signed numeric type.
   * @ingroup defs
   */
  
  typedef long int kf_int64_t;
  
} // namespace kfoundation

#endif