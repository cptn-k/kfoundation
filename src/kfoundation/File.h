/*---[File.h]--------------------------------------------------m(._.)m--------*\
 |
 |  Project: KFoundation
 |  Class: File
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

#ifndef __KFoundation__File__
#define __KFoundation__File__

#include "definitions.h"
#include "ManagedObject.h"
#include "PtrDecl.h"

namespace kfoundation {

  class Path;
  
  class File : public ManagedObject {
    
  // --- STATIC METHODS --- //
    public: static void getMd5(PPtr<Path> filePath, kf_octet_t md5[16]);
    
  };
  
} // namespace kfoundation

#endif /* defined(__KFoundation__File__) */
