//
//  UChar.cpp
//  KFoundation-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/13/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

/*
 * Copyright (c) 2007 Alexey Vatchenko <av@bsdua.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// Std
#include <cstring>

// Unistring
#include <unistring/unicase.h>
#include <unistring/unictype.h>

// Internal
#include "Ref.h"
#include "OutputStream.h"
#include "InputStream.h"
#include "UString.h"

// Self
#include "UChar.h"

#define  NXT 0x80
#define SEQ2 0xc0
#define SEQ3 0xe0
#define SEQ4 0xf0
#define SEQ5 0xf8
#define SEQ6 0xfc
#define  BOM 0xfeff

namespace kfoundation {
  

// --- CONSTURCTORS --- //

  UChar::UChar() {
    _octets[0] = 0;
    _octets[5] = 1;
  }
  

  UChar::UChar(const kf_octet_t* utf) {
    memcpy(_octets, utf, getNumberOfUtf8Octets(*utf));
  }


  /**
   * Constructor. Assigns the internal value to the given parameter.
   *
   * @param value The value to be assigned.
   */
  
  UChar::UChar(char ascii) {
    _octets[5] = (kf_octet_t)writeUtf8(ascii, _octets);
  }
  
  
  /**
   * Constructor. Assigns the internal value to the given parameter.
   *
   * @param value The value to be set.
   */
  
  UChar::UChar(wchar_t unicode) {
    _octets[5] = (kf_octet_t)writeUtf8(unicode, _octets);
  }


// --- STATIC METHODS --- //

  /**
   * Returns the number of octets for the codepoint starting with the given
   * octet.
   */

  kf_int8_t UChar::getNumberOfUtf8Octets(const kf_octet_t firstOctet) {
    if ((firstOctet & 0x80) == 0) {
      return 1;
    } else if ((firstOctet & 0xe0) == SEQ2) {
      return 2;
    } else if ((firstOctet & 0xf0) == SEQ3) {
      return 3;
    } else if ((firstOctet & 0xf8) == SEQ4) {
      return 4;
    } else if ((firstOctet & 0xfc) == SEQ5) {
      return 5;
    } else if ((firstOctet & 0xfe) == SEQ6) {
      return 6;
    }
    return 0;
  }


  /**
   * Returns the number of octets necessary to represent the given character
   * in UTF-8 format.
   */

  kf_int8_t UChar::getNumberOfUtf8Octets(const wchar_t ch) {
    if (ch <= 0x0000007f) {
      return 1;
    } else if (ch <= 0x000007ff) {
      return 2;
    } else if (ch <= 0x0000ffff) {
      return 3;
    } else if (ch <= 0x001fffff) {
      return 4;
    } else if (ch <= 0x03ffffff) {
      return 5;
    } else { /* if (*w <= 0x7fffffff) */
      return 6;
    }
  }


  /**
   * Writes the given character onto the given buffer with UTF-8 encoding.
   *
   * @param ch The character to write.
   * @param buffer The begining of the buffer to write on.
   * @return The number of octets written.
   */

  kf_int8_t UChar::writeUtf8(const wchar_t ch, kf_octet_t* buffer) {
    unsigned short int n = getNumberOfUtf8Octets(ch);
    kf_octet_t* oc = (unsigned char*)&ch;

    switch (n) {
      case 1:
        *buffer = oc[0];
        break;

      case 2:
        buffer[1] = NXT | (oc[0] & 0x3f);
        buffer[0] = SEQ2 | (oc[0] >> 6) | ((oc[1] & 0x07) << 2);
        break;

      case 3:
        buffer[2] = NXT | (oc[0] & 0x3f);
        buffer[1] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[0] = SEQ3 | ((oc[1] & 0xf0) >> 4);
        break;

      case 4:
        buffer[3] = NXT | (oc[0] & 0x3f);
        buffer[2] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[1] = NXT | ((oc[1] & 0xf0) >> 4) |
        ((oc[2] & 0x03) << 4);
        buffer[0] = SEQ4 | ((oc[2] & 0x1f) >> 2);
        break;

      case 5:
        buffer[4] = NXT | (oc[0] & 0x3f);
        buffer[3] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[2] = NXT | ((oc[1] & 0xf0) >> 4) |
        ((oc[2] & 0x03) << 4);
        buffer[1] = NXT | (oc[2] >> 2);
        buffer[0] = SEQ5 | (oc[3] & 0x03);
        break;

      case 6:
        buffer[5] = NXT | (oc[0] & 0x3f);
        buffer[4] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[3] = NXT | (oc[1] >> 4) | ((oc[2] & 0x03) << 4);
        buffer[2] = NXT | (oc[2] >> 2);
        buffer[1] = NXT | (oc[3] & 0x3f);
        buffer[0] = SEQ6 | ((oc[3] & 0x40) >> 6);
        break;
    }

    return n;
  }


  /**
   * Reads a UTF-8 encoded character from the given input and sets the given
   * output accordingly.
   *
   * @param input The begining of the buffer to read from.
   * @param output The output to write to.
   * @return The number of octets read.
   */

  kf_int8_t UChar::readUtf8(const kf_octet_t* input, wchar_t& output) {
    const kf_octet_t* p = input;
    unsigned char high = 0;
    unsigned short int n = 1;
    short int nBits = 0;

    /*
     * Get number of bytes for one wide character.
     */
    if ((*p & 0x80) == 0) {
      high = (wchar_t)*p;
    } else if ((*p & 0xe0) == SEQ2) {
      n = 2;
      high = (wchar_t)(*p & 0x1f);
    } else if ((*p & 0xf0) == SEQ3) {
      n = 3;
      high = (wchar_t)(*p & 0x0f);
    } else if ((*p & 0xf8) == SEQ4) {
      n = 4;
      high = (wchar_t)(*p & 0x07);
    } else if ((*p & 0xfc) == SEQ5) {
      n = 5;
      high = (wchar_t)(*p & 0x03);
    } else if ((*p & 0xfe) == SEQ6) {
      n = 6;
      high = (wchar_t)(*p & 0x01);
    }

    output = 0;
    for(int i = 1; i < n; i++) {
      output |= (wchar_t)(p[n - i] & 0x3f) << nBits;
      nBits += 6;
    }
    output |= high << nBits;

    return n;
  }


  /**
   * Reads one UTF-8 encoded character from the given stream and assigns the
   * given output accordingly.
   *
   * @param stream The stream to read from.
   * @param output The output to write to.
   * @param readBytes The read octets are written to this buffer, and I don't
   *                  remember why.
   * @return The number of read octets.
   */

  kf_int8_t UChar::readUtf8(const Ref<InputStream> &stream, wchar_t &output,
      kf_octet_t* readBytes)
  {
    kf_octet_t p[6];
    kf_octet_t high = 0;
    unsigned short int n = 1;
    short int nBits = 0;

    p[0] = stream->read();

    /*
     * Get number of bytes for one wide character.
     */
    if ((*p & 0x80) == 0) {
      high = (wchar_t)*p;
    } else if ((*p & 0xe0) == SEQ2) {
      n = 2;
      high = (wchar_t)(*p & 0x1f);
    } else if ((*p & 0xf0) == SEQ3) {
      n = 3;
      high = (wchar_t)(*p & 0x0f);
    } else if ((*p & 0xf8) == SEQ4) {
      n = 4;
      high = (wchar_t)(*p & 0x07);
    } else if ((*p & 0xfc) == SEQ5) {
      n = 5;
      high = (wchar_t)(*p & 0x03);
    } else if ((*p & 0xfe) == SEQ6) {
      n = 6;
      high = (wchar_t)(*p & 0x01);
    }

    if(n > 1) {
      stream->read(p + 1, n - 1);
    }

    output = 0;
    for(int i = 1; i < n; i++) {
      output |= (wchar_t)(p[n - i] & 0x3f) << nBits;
      nBits += 6;
    }
    output |= high << nBits;

    if(readBytes != NULL) {
      memcpy(readBytes, p, n);
    }

    return n;
  }


  /**
   * Checks if the given parameter is a lowercase character.
   */

  bool UChar::isLowerCase(const wchar_t ch) {
    return uc_tolower(ch);
  }


  /**
   * Checks if the given parameter is an uppercase character.
   */

  bool UChar::isUpperCase(const wchar_t ch) {
    return uc_toupper(ch);
  }


  /**
   * Checks if the given parameter is a numeric character.
   */

  bool UChar::isNumeric(const wchar_t ch) {
    return uc_is_upper(ch);
  }


  /**
   * Checks if the given argument is an alphabetic character.
   */

  bool UChar::isAlpabet(const wchar_t ch) {
    return uc_is_alpha(ch);
  }


  /**
   * Checks if the given parameter is an alphanumeric character.
   */

  bool UChar::isAlphanumeric(const wchar_t ch) {
    return uc_is_alnum(ch);
  }


  /**
   * Checks if the given paramter is a white space.
   */

  bool UChar::isWhiteSpace(const wchar_t ch) {
    return uc_is_space(ch);
  }


  /**
   * Converts the given parameter to lowercase.
   *
   * @param ch The character to be converted.
   * @return The result of conversion.
   */

  wchar_t UChar::toLowercase(const wchar_t ch) {
    return uc_tolower(ch);
  }


  /**
   * Converts the given parameter to uppercase.
   *
   * @param ch The character to be converted.
   * @return The result of conversion.
   */
  
  wchar_t UChar::toUppercase(const wchar_t ch) {
    return uc_toupper(ch);
  }


// --- METHODS --- //
   
  wchar_t UChar::toWChar() const {
    register wchar_t ch;
    readUtf8(_octets, ch);
    return ch;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given parameter.
   *
   * @param value The value to be set.
   */
  
  void UChar::set(const wchar_t value) {
    _octets[5] = writeUtf8(value, _octets);
  }


  kf_int8_t UChar::read(const kf_octet_t* value) {
    register kf_int8_t s = getNumberOfUtf8Octets(*value);
    memcpy(_octets, value, s);
    _octets[5] = s;
    return s;
  }


  kf_int8_t UChar::read(Ref<InputStream> stream) {
    kf_int32_t first = stream->read();
    if(first == KF_NOT_FOUND) {
      return KF_NOT_FOUND;
    }

    kf_int8_t n = getNumberOfUtf8Octets((kf_octet_t)first);
    *_octets = (kf_octet_t)first;
    if(n > 1) {
      stream->read(_octets + 1, n - 1);
    }
    _octets[5] = n;
    return n;
  }


  kf_int8_t UChar::write(kf_octet_t* buffer) const{
    memcpy(buffer, _octets, _octets[5]);
    return _octets[5];
  }
  

  /**
   * Checks if this is a lowercase character.
   */
  
  bool UChar::isLowerCase() const {
    return uc_is_lower(toWChar());
  }
  
  
  /**
   * Checks if this is an uppercase character.
   */
  
  bool UChar::isUpperCase() const {
    return uc_is_upper(toWChar());
  }
  
  
  /**
   * Checks if this is numeric character.
   */
  
  bool UChar::isNumeric() const {
    return uc_is_digit(toWChar());
  }
  
  
  /**
   * Checks if this is an alphabetic character.
   */
  
  bool UChar::isAlphabet() const {
    return uc_is_alpha(toWChar());
  }
  
  
  /**
   * Checks if this is an alphanumeric character.
   */
  
  bool UChar::isAlphanumeric() const {
    return uc_is_alnum(toWChar());
  }
  
  
  /**
   * Checks if this is a white space character.
   */
  
  bool UChar::isWhiteSpace() const {
    return uc_is_space(toWChar());
  }

  
  /**
   * Creates a new UChar obejct representing the lowercase equivalant of this
   * object.
   */
  
  UChar UChar::toLowercase() const {
    return UChar(toLowercase(toWChar()));
  }
  
  
  /**
   * Creates a new UChar object representing the uppercase equivalant of this
   * object.
   */
  
  UChar UChar::toUppercase() const {
    return UChar(toUppercase(toWChar()));
  }


  bool UChar::equals(const UChar ch) const {
    return memcmp(_octets, ch._octets, _octets[5]) == 0;
  }

  // Inherited from Streamer

  void UChar::printToStream(Ref<OutputStream> os) const {
    os->write(_octets, getNumberOfUtf8Octets(*_octets));
  }


  RefConst<UString> UChar::toString() const {
    return new UString(_octets, getNumberOfUtf8Octets(*_octets));
  }
  
} // namespace kfoundation
