/*---[Path.h]--------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : kfoundation::Path::*
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

#ifndef ORG_KNORBA_COMMON_PATH_H
#define ORG_KNORBA_COMMON_PATH_H

#include <string>

#include "ArrayDecl.h"
#include "ManagedObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {
  
  using namespace std;

  /**
   * Used to represent and manipulate file and directory pathnames.
   *
   * @note For better performance use getString() rather than toString() to 
   * get the string value of this object.
   *
   * @ingroup io
   * @headerfile Path.h <kfoundation/Path.h>
   */
  
  class Path : public ManagedObject, public SerializingStreamer {
  private:
    string _str;
    Ptr< Array<int> > _segments;
    int _extention;
    bool _isAbsolute;
    
    void refresh();
    
  public:
    static const char PATH_SEPARATOR;
    
    Path(const string& str);
    ~Path();
    bool hasExtention() const;
    bool isAbsolute() const;
    int getNSegments() const;
    string getSegement(int index) const;
    string getExtention() const;
    string getFileName() const;
    string getFileNameWithExtension() const;
    Ptr<Path> addSegement(const string& s);
    Ptr<Path> changeExtension(const string& ex);
    Ptr<Path> removeExtension();
    Ptr<Path> parent();
    void makeDir() const;
    bool exists() const;
    void remove() const;
    
    const string& getString() const;

    // From SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
    // From SerializingStreamer::Streamer
    void printToStream(ostream& os) const;
  };
  
} // namespace kfoundation

#endif
