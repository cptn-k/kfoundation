/*---[KFException.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: KFException
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
#include <string>

#include "definitions.h"
#include "SerializingStreamer.h"

namespace kfoundation {

using namespace std;

class KFException : public SerializingStreamer, public exception {
  
// --- NESTED TYPES --- //
  
  public: class StackTraceItem : public SerializingStreamer {
    private: int      _index;
    private: string   _exeName;
    private: long int _address;
    private: string   _symbol;
    private: int      _offset;
    
    private: static char* readWhiteSpace(char* str);
    private: static char* readText(char* str);
    private: static char* readChar(char* str, char ch);
    public: static string demangle(string str);
    public: static StackTraceItem fromStackTraceString(char* str);
    
    public: int getIndex() const {
      return _index;
    }
    
    public: string getExeName() const {
      return _exeName;
    }
    
    public: long int getAddress() const {
      return _address;
    }
    
    public: string getSymbol() const {
      return _symbol;
    }
    
    public: int getOffset() const {
      return _offset;
    }
    
    public: void serialize(PPtr<ObjectSerializer> os) const;
  };
  
// --- STATIC FIELDS --- //
  
  private: static const int MAX_FRAMES = 40;
  
  
// --- FIELDS --- //
  
  private: string _name;
  private: string _message;
  private: int    _nHiddenFrames;
  private: StackTraceItem* _stackTrace;
  private: int _nFrames;
  
  
// --- (DE)CONSTRUCTORS --- //
  
  public: KFException(string message);
  public: KFException(const KFException& other);
  public: ~KFException() throw();
  
  
// --- METHODS --- //
  
  private: void createStackTrace();
  protected: void setName(string name);
  protected: void incrementNumberOfHiddenFrames();
  public: const string& getMessage() const;
  public: int getNStackItems();
  public: const StackTraceItem& getStackItem() const;
  
  // Inherited from SerializingStreamer //
  public: void serialize(PPtr<ObjectSerializer> os) const;
  
  // Inherited from exception //
  public: virtual const char* what() const throw();
  
};
  
} // namespace kfoundation

#endif
