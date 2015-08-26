//
//  Character.cpp
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

#include <cstring>
#include <unistring/unicase.h>
#include <unistring/unictype.h>

#include "Ptr.h"

#include "UniChar.h"

#define  NXT 0x80
#define SEQ2 0xc0
#define SEQ3 0xe0
#define SEQ4 0xf0
#define SEQ5 0xf8
#define SEQ6 0xfc
#define  BOM 0xfeff

namespace kfoundation {
  
  using namespace std;

  
  /**
   * Constructor. Assigns the internal value to the given parameter.
   *
   * @param value The value to be assigned.
   */
  
  UniChar::UniChar(char value)
    : _value(value)
  {
    // Nothing;
  }
  
  
  /**
   * Constructor. Assigns the internal value to the given parameter.
   *
   * @param value The value to be set.
   */
  
  UniChar::UniChar(wchar_t value)
    : _value(value)
  {
    // Nothing
  }
  
   
  /**
   * Getter method. Returns the internal value.
   */
   
  wchar_t UniChar::get() const {
    return _value;
  }
  
  
  /**
   * Setter method. Sets the internal value to the given parameter.
   *
   * @param value The value to be set.
   */
  
  void UniChar::set(const wchar_t value) {
    _value = value;
  }
  
  
  /**
   * Returns the number of octets needed to represent the value of this object
   * in UTF-8.
   */
  
  unsigned short int UniChar::getNumberOfUtf8Octets() const {
    return getNumberOfUtf8Octets(_value);
  }
  
  
  /**
   * Writes the value of this object to to the given buffer as UTF-8.
   *
   * @param buffer The begining of the buffer to write on.
   * @return The number of octets written.
   */
  
  unsigned short int UniChar::writeUtf8(kf_octet_t* buffer) const {
    return writeUtf8(_value, buffer);
  }
  
  
  /**
   * Reads a unicode caracter encoded in UTF-8 from the given buffer.
   *
   * @param buffer The begining of the buffer to read the value from.
   * @return The number of octets read.
   */
  
  unsigned short int UniChar::readUtf8(kf_octet_t* buffer) {
    return readUtf8(buffer, _value);
  }
  
  
  /**
   * Reads a unicode character encoded in UTF-8 from the given stream.
   *
   * @param stream The stream to read the value from.
   * @return The number of octets read.
   */
  
  unsigned short int UniChar::readUtf8(Ptr<InputStream> stream) {
    return readUtf8(stream, _value, NULL);
  }
  
  
  /**
   * Checks if this is a lowercase character.
   */
  
  bool UniChar::isLowerCase() const {
    return uc_is_lower(_value);
  }
  
  
  /**
   * Checks if this is an uppercase character.
   */
  
  bool UniChar::isUpperCase() const {
    return uc_is_upper(_value);
  }
  
  
  /**
   * Checks if this is numeric character.
   */
  
  bool UniChar::isNumeric() const {
    return uc_is_digit(_value);
  }
  
  
  /**
   * Checks if this is an alphabetic character.
   */
  
  bool UniChar::isAlphabet() const {
    return uc_is_alpha(_value);
  }
  
  
  /**
   * Checks if this is an alphanumeric character.
   */
  
  bool UniChar::isAlphanumeric() const {
    return uc_is_alnum(_value);
  }
  
  
  /**
   * Checks if this is a white space character.
   */
  
  bool UniChar::isWhiteSpace() const {
    return uc_is_space(_value);
  }
  
  
  /**
   * Converts this character to lowercase.
   */
  
  void UniChar::toLowerCase() {
    _value = uc_tolower(_value);
  }
  
  
  /**
   * Converts this character to uppercase.
   */
  
  void UniChar::toUpperCase() {
    _value = uc_toupper(_value);
  }
  
  
  /**
   * Creates a new UniChar obejct representing the lowercase equivalant of this
   * object.
   */
  
  Ptr<UniChar> UniChar::duplicateToLowerCase() const {
    Ptr<UniChar> ch(new UniChar((wchar_t)uc_tolower(_value)));
    return ch;
  }
  
  
  /**
   * Creates a new UniChar object representing the uppercase equivalant of this
   * object.
   */
  
  Ptr<UniChar> UniChar::duplicateToUpperCase() const {
    Ptr<UniChar> ch(new UniChar((wchar_t)uc_is_upper(_value)));
    return ch;
  }
  
  
  /**
   * Creates a copy of this object.
   */
  
  Ptr<UniChar> UniChar::duplicateUniChar() const {
    Ptr<UniChar> ch(new UniChar((wchar_t)_value));
    return ch;
  }
  
  
  /**
   * Returns the number of octets necessary to represent the given character
   * in UTF-8 format.
   */
  
  unsigned short int UniChar::getNumberOfUtf8Octets(const wchar_t ch) {
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
  
  unsigned short int UniChar::writeUtf8(const wchar_t ch, kf_octet_t* buffer) {
    unsigned short int n = getNumberOfUtf8Octets(ch);
		kf_octet_t* oc = (unsigned char*)&ch;
    
		switch (n) {
      case 1:
        *buffer = oc[0];
        break;
        
      case 2:
        buffer[0] = NXT | (oc[0] & 0x3f);
        buffer[1] = SEQ2 | (oc[0] >> 6) | ((oc[1] & 0x07) << 2);
        break;
        
      case 3:
        buffer[0] = NXT | (oc[0] & 0x3f);
        buffer[1] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[2] = SEQ3 | ((oc[1] & 0xf0) >> 4);
        break;
        
      case 4:
        buffer[0] = NXT | (oc[0] & 0x3f);
        buffer[1] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[2] = NXT | ((oc[1] & 0xf0) >> 4) |
                    ((oc[2] & 0x03) << 4);
        buffer[3] = SEQ4 | ((oc[2] & 0x1f) >> 2);
        break;
        
      case 5:
        buffer[0] = NXT | (oc[0] & 0x3f);
        buffer[1] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[2] = NXT | ((oc[1] & 0xf0) >> 4) |
                    ((oc[2] & 0x03) << 4);
        buffer[3] = NXT | (oc[2] >> 2);
        buffer[4] = SEQ5 | (oc[3] & 0x03);
        break;
        
      case 6:
        buffer[0] = NXT | (oc[0] & 0x3f);
        buffer[1] = NXT | (oc[0] >> 6) | ((oc[1] & 0x0f) << 2);
        buffer[2] = NXT | (oc[1] >> 4) | ((oc[2] & 0x03) << 4);
        buffer[3] = NXT | (oc[2] >> 2);
        buffer[4] = NXT | (oc[3] & 0x3f);
        buffer[5] = SEQ6 | ((oc[3] & 0x40) >> 6);
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
  
  unsigned short int UniChar::readUtf8(const kf_octet_t* input, wchar_t& output) {
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
  
  unsigned short int UniChar::readUtf8(const Ptr<InputStream> &stream, wchar_t &output, kf_octet_t* readBytes) {
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
  
  bool UniChar::isLowerCase(const wchar_t ch) {
    return uc_tolower(ch);
  }
  
  
  /**
   * Checks if the given parameter is an uppercase character.
   */
  
  bool UniChar::isUpperCase(const wchar_t ch) {
    return uc_toupper(ch);
  }
  
  
  /**
   * Checks if the given parameter is a numeric character.
   */
  
  bool UniChar::isNumeric(const wchar_t ch) {
    return uc_is_upper(ch);
  }
  
  
  /**
   * Checks if the given argument is an alphabetic character.
   */
  
  bool UniChar::isAlpabet(const wchar_t ch) {
    return uc_is_alpha(ch);
  }
  
  
  /**
   * Checks if the given parameter is an alphanumeric character.
   */
  
  bool UniChar::isAlphanumeric(const wchar_t ch) {
    return uc_is_alnum(ch);
  }
  
  
  /**
   * Checks if the given paramter is a white space.
   */
  
  bool UniChar::isWhiteSpace(const wchar_t ch) {
    return uc_is_space(ch);
  }
  
  
  /**
   * Converts the given parameter to lowercase.
   *
   * @param ch The character to be converted.
   * @return The result of conversion.
   */
  
  wchar_t UniChar::toLowerCase(const wchar_t ch) {
    return uc_tolower(ch);
  }
  
  
  /**
   * Converts the given parameter to uppercase.
   *
   * @param ch The character to be converted.
   * @return The result of conversion.
   */
  
  wchar_t UniChar::toUpperCase(const wchar_t ch) {
    return uc_toupper(ch);
  }
  
  void UniChar::printToStream(ostream& os) const {
    kf_octet_t buffer[6];
    unsigned short int n = writeUtf8(buffer);
    os.write((char*)buffer, n);
  }
  
} // namespace kfoundation
