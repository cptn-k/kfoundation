/*---[KFException.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::KFException::*
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

#ifndef ORG_KNORBA_COMMON_EXCEPTION_H
#define ORG_KNORBA_COMMON_EXCEPTION_H

#include <exception>

#include "RefDecl.h"
#include "definitions.h"
#include "SerializingStreamer.h"

namespace kfoundation {

  class UString;
  template<typename T> class Array;

/**
 * Superclass for all exceptions in KFoundation.
 *
 * It maintain a record of stack trace at the time it is thrown. Since this
 * class is a SerializingStreamer, it can be fed directly to logger.
 *
 *     try {
 *       ... something ...
 *     } catch(KFException& e) {
 *       LOG << e << ENDS;
 *     }
 *.
 * @ingroup exceptions
 * @headerfile KFException.h <kfoundation/KFException.h>
 */
  
class KFException : public SerializingStreamer, public std::exception {

// --- NESTED TYPES --- //

  /**
   * Represents an element in stack trace.
   */

  public: class StackTraceItem : public SerializingStreamer {

  // --- FIELDS --- //

    private: kf_int32_t _index;
    private: kf_int32_t _offset;
    private: RefConst<UString> _exeName;
    private: RefConst<UString> _address;
    private: RefConst<UString> _symbol;


  // --- METHODS --- //

    public: void set(char* str);

    /** Returns the frame number for element. */

    public: kf_int32_t getIndex() const {
      return _index;
    }


    /** Returns the name of the executable file for this element. */
    
    public: RefConst<UString> getExeName() const {
      return _exeName;
    }


    /** Returns the function return address. */
    
    public: RefConst<UString> getAddress() const {
      return _address;
    }


    /** Returns the function name. */
    
    public: RefConst<UString> getSymbol() const {
      return _symbol;
    }


    /** Returns the instruction offset. */
    
    public: kf_int32_t getOffset() const {
      return _offset;
    }

    // From SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> os) const;

  };
  
  
// --- STATIC FIELDS --- //
  
  private: static const int MAX_FRAMES = 40;
  
  
// --- FIELDS --- //
  
  private: int _nHiddenFrames;
  private: int _nFrames;
  private: RefConst<UString> _name;
  private: RefConst<UString> _message;
  private: mutable RefConst<UString> _what;
  private: Ref< Array<StackTraceItem> > _stackTrace;


// --- (DE)CONSTRUCTORS --- //

  public: KFException(RefConst<UString> message);
  public: KFException(const KFException& other);
  public: ~KFException() throw();
  
  
// --- METHODS --- //
  
  private: void createStackTrace();
  protected: void setName(RefConst<UString> name);
  protected: void incrementNumberOfHiddenFrames();
  public: RefConst<UString> getMessage() const;
  public: int getNStackItems();
  public: const StackTraceItem& getStackItem() const;
  
  // Inherited from SerializingStreamer //
  public: void serialize(Ref<ObjectSerializer> os) const;
  
  // Inherited from exception //
  public: virtual const char* what() const throw();
  
};
  
} // namespace kfoundation

#endif