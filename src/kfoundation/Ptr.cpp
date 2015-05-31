/*---[Ptr.cpp]-------------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: PtrBase
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
#include "PtrDecl.h"

namespace kfoundation {
  
//\/ PtrBase /\////////////////////////////////////////////////////////////////
  
  MasterMemoryManager* PtrBase::master = NULL;
  ObjectRecord** PtrBase::objectTable  = NULL;
  long int PtrBase::serial = 0;
  
}

