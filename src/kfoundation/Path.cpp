/*---[Path.cpp]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : -
 |  Implements: kfoundation::Path::*
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

// Std
#include <cstdlib>
#include <cstdio>

// Unix
#include <unistd.h>
#include <sys/stat.h>

// Internal
#include "definitions.h"
#include "Path.h"
#include "Ref.h"
#include "Array.h"
#include "IOException.h"
#include "StringPrintWriter.h"
#include "UString.h"


namespace kfoundation {

//\/ GLOBAL /\//////////////////////////////////////////////////////////////////

#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))
#define I(x, y, z)			((y) ^ ((x) | ~(z)))
#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
  #define SET(n) \
    (*(MD5_u32plus *)&ptr[(n) * 4])
  #define GET(n) \
    SET(n)
#else
  #define SET(n) \
    (ctx->block[(n)] = \
    (MD5_u32plus)ptr[(n) * 4] | \
    ((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
    ((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
    ((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
  #define GET(n) \
    (ctx->block[(n)])
#endif

  typedef unsigned int MD5_u32plus;

  typedef struct {
    MD5_u32plus lo, hi;
    MD5_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD5_u32plus block[16];
  } MD5_CTX;

  void MD5_Init(MD5_CTX *ctx);
  void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
  void MD5_Final(unsigned char *result, MD5_CTX *ctx);


  static const void *body(MD5_CTX *ctx, const void *data, unsigned long size)
  {
    const unsigned char *ptr;
    MD5_u32plus a, b, c, d;
    MD5_u32plus saved_a, saved_b, saved_c, saved_d;

    ptr = (const unsigned char *)data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
      saved_a = a;
      saved_b = b;
      saved_c = c;
      saved_d = d;

      /* Round 1 */
      STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
      STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
      STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
      STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
      STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
      STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
      STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
      STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
      STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
      STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
      STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
      STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
      STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
      STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
      STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
      STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

      /* Round 2 */
      STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
      STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
      STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
      STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
      STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
      STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
      STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
      STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
      STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
      STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
      STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
      STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
      STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
      STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
      STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
      STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

      /* Round 3 */
      STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
      STEP(H2, d, a, b, c, GET(8), 0x8771f681, 11)
      STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
      STEP(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
      STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
      STEP(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
      STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
      STEP(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
      STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
      STEP(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
      STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
      STEP(H2, b, c, d, a, GET(6), 0x04881d05, 23)
      STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
      STEP(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
      STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
      STEP(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

      /* Round 4 */
      STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
      STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
      STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
      STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
      STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
      STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
      STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
      STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
      STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
      STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
      STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
      STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
      STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
      STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
      STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
      STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

      a += saved_a;
      b += saved_b;
      c += saved_c;
      d += saved_d;

      ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return ptr;
  }

  void MD5_Init(MD5_CTX *ctx)
  {
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
  }

  void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size)
  {
    MD5_u32plus saved_lo;
    unsigned long used, available;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
      ctx->hi++;
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
      available = 64 - used;

      if (size < available) {
        memcpy(&ctx->buffer[used], data, size);
        return;
      }

      memcpy(&ctx->buffer[used], data, available);
      data = (const unsigned char *)data + available;
      size -= available;
      body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
      data = body(ctx, data, size & ~(unsigned long)0x3f);
      size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
  }

  void MD5_Final(unsigned char *result, MD5_CTX *ctx)
  {
    unsigned long used, available;

    used = ctx->lo & 0x3f;

    ctx->buffer[used++] = 0x80;

    available = 64 - used;

    if (available < 8) {
      memset(&ctx->buffer[used], 0, available);
      body(ctx, ctx->buffer, 64);
      used = 0;
      available = 64;
    }

    memset(&ctx->buffer[used], 0, available - 8);

    ctx->lo <<= 3;
    ctx->buffer[56] = ctx->lo;
    ctx->buffer[57] = ctx->lo >> 8;
    ctx->buffer[58] = ctx->lo >> 16;
    ctx->buffer[59] = ctx->lo >> 24;
    ctx->buffer[60] = ctx->hi;
    ctx->buffer[61] = ctx->hi >> 8;
    ctx->buffer[62] = ctx->hi >> 16;
    ctx->buffer[63] = ctx->hi >> 24;

    body(ctx, ctx->buffer, 64);

    result[0] = ctx->a;
    result[1] = ctx->a >> 8;
    result[2] = ctx->a >> 16;
    result[3] = ctx->a >> 24;
    result[4] = ctx->b;
    result[5] = ctx->b >> 8;
    result[6] = ctx->b >> 16;
    result[7] = ctx->b >> 24;
    result[8] = ctx->c;
    result[9] = ctx->c >> 8;
    result[10] = ctx->c >> 16;
    result[11] = ctx->c >> 24;
    result[12] = ctx->d;
    result[13] = ctx->d >> 8;
    result[14] = ctx->d >> 16;
    result[15] = ctx->d >> 24;

    memset(ctx, 0, sizeof(*ctx));
  }



//\/ Path /\////////////////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  /**
   * Path separator symbol on the current platform.
   */
  
#if defined(KF_UNIX) || defined(__doxygen__)
  const char Path::PATH_SEPARATOR = '/';
#else
#  error "Unsupported platform"
#endif
  

// --- CONSTRUCTORS --- //

  /**
   * Constructor.
   *
   * @param str String containing the desired path. It can be empty.
   */
  
  Path::Path(RefConst<UString> str) {
    _str = str;
    _segments = new Array<kf_int16_t>();
    setup();
  }


// --- METHODS --- //
  
  void Path::setup() {
    _segments->clear();
    _extention = -1;

    OctetIterator it = _str->getOctetIterator();
    for(kf_octet_t ch = it.first(); it.hasMore(); ch = it.next()) {
      if(ch == PATH_SEPARATOR) {
        _segments->push(it.getIndex());
        _extention = -1;
      } else if(ch == '.') {
        _extention = it.getIndex();
      }
    }

    _segments->push(_str->getOctetCount());
    _segments->consolidate();
  }
  
  
  /**
   * Checks if the path has extention. That is, if the last segment of the path
   * contatins a dot '.'.
   */
  
  bool Path::hasExtention() const {
    return _extention != -1;
  }
  
  
  /**
   * Checks if the path is absolute. That is, if it begins with a separator
   * character.
   */
  
  bool Path::isAbsolute() const {
    return _str->at(0) == PATH_SEPARATOR;
  }
  
  
  /**
   * Returns the number of segments in the path.
   */
  
  int Path::getNSegments() const {
    return (int)_segments->getSize();
  }
  
  
  /**
   * Returns the path segment at the given index.
   */
  
  RefConst<UString> Path::getSegement(int index) const {
    int begin = (index == 0)?0:(_segments->at(index - 1) + 1);
    int end = _segments->at(index);
    
    if(end == -1) {
      return K"/";
    }
    
    return new UStringWindow(_str, begin, end);
  }
  
  
  /**
   * Returns the last segment of the path excluding the extension if exists.
   */
  
  RefConst<UString> Path::getFileName() const {
    int s = _segments->getSize() - 1;
    int begin = (s == 0)?0:(_segments->at(s - 1) + 1);
    int end = _segments->at(s);
    if(_extention > -1) {
      end = _extention;
    }
    
    return new UStringWindow(_str, begin, end);
  }
  
  
  /**
   * Returns the extension if exists, otherwise, an empty string.
   */
  
  RefConst<UString> Path::getExtention() const {
    if(!hasExtention()) {
      return new UString();
    }
    
    return new UStringWindow(_str, _extention + 1, _str->getOctetCount());
  }
  
  
  /**
   * Returns the last segment of the path including extention.
   */
  
  RefConst<UString> Path::getFileNameWithExtension() const {
    return getSegement(getNSegments() - 1);
  }
  
  
  /**
   * Add a segment to the path.
   */
  
  Ref<Path> Path::addSegement(RefConst<UString> s) {
    return new Path(_str + PATH_SEPARATOR + s);
  }
  
  
  /**
   * Changes the file extensions. Adds one if it does not exist.
   */
  
  Ref<Path> Path::changeExtension(RefConst<UString> ex) {
    if(hasExtention()) {
      Ref<UString> str = new UStringWindow(_str, 0, _extention + 1);
      return new Path(str + ex);
    }
    
    return new Path(_str + '.' + ex);
  }
  
  
  /**
   * Removes the extension.
   */
  
  Ref<Path> Path::removeExtension() {
    if(hasExtention()) {
      return new Path(_str->subString(0, _extention));
    }
    
    return this;
  }
  
  
  /**
   * Returns the parent path of the current path by removing its last segment.
   * If the path has only one segment or is empty, returns an empty path.
   */
  
  Ref<Path> Path::parent() {
    int s = _segments->getSize();
    
    if(s <= 1) {
      return new Path(UString::EMPTY);
    }
    
    return new Path(new UStringWindow(_str, 0, _segments->at(s - 2)));
  }
  
  
  /**
   * Makes a new directory at the path encoded by this object.
   */
  
  void Path::makeDir() const {
#ifdef KF_UNIX
    
    if(mkdir((const char*)_str->getOctets(), S_IRWXU) == -1) {
      throw IOException(K"Error making directory " + _str + ". Reason: "
          + System::getLastSystemError());
    };
    
#else
#  error "Platform not supported."
#endif
  }
  
  
  /**
   * Checks if a file or directory pointed by this path object exists.
   */
  
  bool Path::exists() const {
#ifdef KF_UNIX
    return access(_str->getCString(), F_OK) != -1;
#else
#  error "Platform not supported."
#endif
  }
  
  
  /**
   * Deletes the file or directory pointed by this path object.
   */
  
  void Path::remove() const {
#ifdef KF_UNIX
    if(::remove(_str->getCString()) == -1) {
      throw IOException(K"Error removing directory " + _str + ". Reason: "
         + System::getLastSystemError());
    }
#else
#  error "Platform not supported."
#endif
  }


  kf_int64_t Path::getFileSize() const {
    struct stat stats;
    ::stat(_str->getCString(), &stats);
    return stats.st_size;
  }


  bool Path::isDirectory() const {
    struct stat stats;
    ::stat(_str->getCString(), &stats);
    return S_ISDIR(stats.st_mode);
  }


  kf_int64_t Path::getModificationTime() const {
    struct stat stats;
    ::stat(_str->getCString(), &stats);
    return stats.st_mtimespec.tv_sec * 1000
        + stats.st_mtimespec.tv_nsec / 1000000;
  }


  kf_int64_t Path::getAccessTime() const {
    struct stat stats;
    ::stat(_str->getCString(), &stats);
    return stats.st_atimespec.tv_sec * 1000
      + stats.st_atimespec.tv_nsec / 1000000;
  }


  bool Path::hasWritePermission() const {
    return access(_str->getCString(), W_OK) == 0;
  }


  bool Path::hasReadPermission() const {
    return access(_str->getCString(), R_OK) == 0;
  }


  Path::Md5 Path::computeMd5() const {
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    FILE *inFile = fopen(_str->getCString(), "rb");

    if (inFile == NULL) {
      throw KFException(K"File " + *_str + " cant be opened.");
    }

    MD5_Init (&mdContext);

    while ((bytes = (int)fread(data, 1, 1024, inFile)) != 0) {
      MD5_Update(&mdContext, data, bytes);
    }

    fclose (inFile);

    Md5 result;
    MD5_Final(result.octets, &mdContext);

    return result;
  }

  
  void Path::serialize(Ref<ObjectSerializer> builder) const {
    StringPrintWriter segments;

    builder->object(K"Path")
      ->attribute(K"name", getFileName())
      ->attribute(K"extension", getExtention())
      ->attribute(K"isAbsolute", isAbsolute())
      ->member(K"segments")->collection();

    int s = getNSegments();
    for(int i = 0; i < s; i++) {
      builder->object(*getSegement(i));
    }

    builder->endCollection()
      ->endObject();
  }


  void Path::printToStream(Ref<OutputStream> os) const {
    _str->printToStream(os);
  }
  
  
  RefConst<UString> Path::toString() const {
    return _str;
  }

} // namespace kfoundation