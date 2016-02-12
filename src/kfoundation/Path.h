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

#include "ArrayDecl.h"
#include "KFObject.h"
#include "SerializingStreamer.h"

namespace kfoundation {

  class UString;

  /**
   * Represents a file path.
   *
   * @ingroup io
   * @headerfile Path.h <kfoundation/Path.h>
   */
  
  class Path : public KFObject, public SerializingStreamer {

  // --- NESTED TYPES --- //

    public: class Md5 {
      public: kf_octet_t octets[16];
    };


  // --- STATIC FIELDS --- //

    public: static const char PATH_SEPARATOR;


  // --- FIELDS --- //

    private: kf_int16_t _extention;
    private: RefConst<UString> _str;
    private: Ref< Array<kf_int16_t> > _segments;


  // --- CONSTRUCTORS --- //

    public: Path(RefConst<UString> str);


  // --- METHODS --- //

    private: void setup();
    public: bool hasExtention() const;
    public: bool isAbsolute() const;
    public: kf_int32_t getNSegments() const;
    public: RefConst<UString> getSegement(int index) const;
    public: RefConst<UString> getExtention() const;
    public: RefConst<UString> getFileName() const;
    public: RefConst<UString> getFileNameWithExtension() const;
    public: Ref<Path> addSegement(RefConst<UString> s);
    public: Ref<Path> changeExtension(RefConst<UString> ex);
    public: Ref<Path> removeExtension();
    public: Ref<Path> parent();
    public: void makeDir() const;
    public: bool exists() const;
    public: void remove() const;
    public: kf_int64_t getFileSize() const;
    public: bool isDirectory() const;
    public: kf_int64_t getModificationTime() const;
    public: kf_int64_t getAccessTime() const;
    public: bool hasWritePermission() const;
    public: bool hasReadPermission() const;
    public: Md5 computeMd5() const;
    
    // From SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> builder) const;
    
    // From SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };
  
} // namespace kfoundation

#endif