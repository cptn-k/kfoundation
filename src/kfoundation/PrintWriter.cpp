//
//  PrintWriter.cpp
//  KFoundation
//
//  Created by Kay Khandan on 12/10/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

// Std
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cstdio>

// Internal
#include "Bool.h"
#include "Int32.h"
#include "Int64.h"
#include "Double.h"
#include "Streamer.h"
#include "UString.h"
#include "UChar.h"
#include "InputStream.h"

// Self
#include "PrintWriter.h"

namespace kfoundation {

// --- STATIC FIELDS --- //

  const kf_octet_t PrintWriter::HEX_DIGITS[]
    = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
       'A', 'B', 'C', 'D', 'E', 'F'};


// --- (DE)CONSTRUCTORS --- //

  /**
   * Creates a new PrintWriter to write on the given stream.
   */

  PrintWriter::PrintWriter(Ref<OutputStream> stream)
  : _stream(stream),
    _hex(false)
  {
    // Nothing;
  }


// --- STATIC METHODS --- //

  void PrintWriter::printHex(kf_octet_t *buffer, const kf_octet_t *values,
      kf_int32_t count, bool reverse)
  {
    for(register kf_int32_t i = 0; i < count; i++) {
      kf_octet_t value = values[i];
      int n = (count - i - 1) * 2;
      buffer[n] = HEX_DIGITS[value >> 4];
      buffer[n + 1] = HEX_DIGITS[value & 0xF];
    }
  }


// --- METHODS --- //

  /**
   * Returns the stream used as output of this PrintWriter.
   */

  Ref<OutputStream> PrintWriter::getStream() const {
    return _stream;
  }


  /**
   * Prints a boolean vlaue.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const bool value) {
    Bool(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a 32-bit integer vlaue.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const kf_int32_t value) {
    Int32(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a 64-bit vlaue.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const kf_int64_t value) {
    Int64(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a float vlaue.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const float value) {
    Double(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a double vlaue.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const double value) {
    Double(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a 8-bit character.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const char value) {
    UChar(value).printToStream(_stream);
    return this;
  }


  /**
   * Prints a wide character.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const wchar_t value) {
    UChar(value).printToStream(_stream);
    return this;
  }

    
  /**
   * Prints a C-style string.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const char* value) {
    _stream->write((const kf_octet_t*)value, (kf_int32_t)strlen(value));
    return this;
  }
  
  
//  Ref<PrintWriter> PrintWriter::print(const std::string& value) {
//    _stream->write((const kf_octet_t*)value.c_str(), (kf_int32_t)value.length());
//    return this;
//  }

  
//  Ref<PrintWriter> PrintWriter::print(const std::wstring &value) {
//    UString(value).printToStream(_stream);
//    return this;
//  }

  
  /**
   * Prints a UString.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(RefConst<kfoundation::UString> value) {
    value->printToStream(_stream);
    return this;
  }


  /**
   * Prints any Streamer object.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const Streamer& value) {
    value.printToStream(_stream);
    return this;
  }


  /**
   * Prints any streamer object in given number of times.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(const Streamer& value, kf_int32_t count) {
    for(int i = count - 1; i >= 0; i--) {
      value.printToStream(_stream);
    }
    return this;
  }


  /**
   * Reads and prints everything from the given InputStream.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::print(Ref<InputStream> stream) {
    _stream->write(stream);
    return this;
  }


  /**
   * Prints an octet in hexadecimal.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printHex(kf_octet_t value) {
    kf_octet_t digits[2];
    printHex(digits, &value, 1, false);
    _stream->write(digits, 2);
    return this;
  }


  /**
   * Prints a 32-bit integer value in hexadecimal.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printHex(const kf_int32_t value) {
    kf_octet_t digits[8];
    printHex(digits, (const kf_octet_t*)&value, 4, System::isBigEndian());
    _stream->write(digits, 8);
    return this;
  }


  /**
   * Prints a 64-bit integer value in hexadecimal.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printHex(const kf_int64_t value) {
    kf_octet_t digits[16];
    printHex(digits, (const kf_octet_t*)&value, 8, System::isBigEndian());
    _stream->write(digits, 16);
    return this;
  }


  /**
   * Prints the hex value corresponding to each one of the given number of 
   * octets starting from the given memory address.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printHex(const kf_octet_t* octets,
      kf_int32_t count)
  {
    kf_octet_t* buffer = new kf_octet_t[count * 2];
    printHex(buffer, octets, count, true);
    _stream->write(buffer, count*2);
    return this;
  }


  /**
   * Corresponds to printf() function.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printFormat(RefConst<UString> format, ...) {
    char* buffer = new char[256];
    va_list args;
    va_start(args, format);
    kf_int32_t n = vsprintf(buffer, (const char*)format->getOctets(), args);
    va_end(args);
    if(n > 0) {
      _stream->write((kf_octet_t*)buffer, n);
    }
    return this;
  }


  /**
   * Prints a time value. The input is expressed in milliseconds from epoch.
   * Format string corresponds to strftime() function.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::printTime(RefConst<UString> format,
      kf_int64_t time)
  {
    time_t secs = time / 1000;
    struct tm* printable = localtime(&secs);

    char* buffer = new char[256];
    size_t s = strftime(buffer, 256, (const char*)format->getOctets(),
        printable);

    if(s > 0) {
      _stream->write((kf_octet_t*)buffer, (kf_int32_t)s);
    }

    return this;
  }


  /**
   * Prints the new line character.
   * @return refernece to self
   */

  Ref<PrintWriter> PrintWriter::newLine() {
    _stream->write('\n');
    return this;
  }


  /**
   * Flushes the stream. May or may not also print a newline character,
   * depending on implementation.
   */

  void PrintWriter::over() {
    newLine();
  }


  /**
   * Processes a special end of chain flag.
   */

  void PrintWriter::operator<<(const printwriter_end_stream_flag_t flag) {
    over();
  }


  /**
   * Processes a special flag.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const printwriter_flag flag) {
    switch (flag) {
      case NEW_LINE:
        _stream->write('\n');
        break;


      case HEX:
        _hex = true;
    }

    return *this;
  }


  /**
   * Prints a boolean value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const bool value) {
    Bool(value).printToStream(_stream);
    return *this;
  }


  /**
   * Prints a 32-bit integer value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const kf_int32_t value) {
    if(_hex) {
      printHex(value);
      _hex = false;
    } else {
      Int32(value).printToStream(_stream);
    }
    return *this;
  }


  /**
   * Prints a 64-bit integer value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const kf_int64_t value) {
    if(_hex) {
      printHex(value);
    } else {
      Int64(value).printToStream(_stream);
    }
    return *this;
  }


  /**
   * Prints a float value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const float value) {
    Double(value).printToStream(_stream);
    return *this;
  }


  /**
   * Prints a double value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const double value) {
    Double(value).printToStream(_stream);
    return *this;
  }


  /**
   * Prints a 8-bit character.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const char value) {
    UChar(value).printToStream(_stream);
    return *this;
  }


  /**
   * Prints a wide character.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const wchar_t value) {
    UChar(value).printToStream(_stream);
    return *this;
  }

  
  /**
   * Prints a boolean value.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const char *cstr) {
    _stream->write((const kf_octet_t*)cstr, (kf_int32_t)strlen(cstr));
    return *this;
  }
  

//  PrintWriter& PrintWriter::operator<<(const std::string& value) {
//    UString(value).printToStream(_stream);
//    return *this;
//  }


//  PrintWriter& PrintWriter::operator<<(const std::wstring& value) {
//    UString(value).printToStream(_stream);
//    return *this;
//  }

  
  /**
   * Prints a UString.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(RefConst<UString> value) {
    value->printToStream(_stream);
    return *this;
  }


  /**
   * Prints any Streamer object.
   * @return self
   */

  PrintWriter& PrintWriter::operator<<(const Streamer& value) {
    value.printToStream(_stream);
    return *this;
  }


  /**
   * Processes special end of chain flag.
   */

  void operator<<(Ref<PrintWriter> pw, const printwriter_end_stream_flag_t flag) {
    pw->over();
  }


  /**
   * Processes a special flag.
   *
   * @return reference to self.
   */
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const printwriter_flag flag) {
    *pw << flag;
    return pw;
  }
  
  
  /**
   * Prints a boolean value.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const bool value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints a 32-bit integer value.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int32_t value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints a 64-bit integer value.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int64_t value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints a float value.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const float value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints a double value.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const double value) {
    *pw << value;
    return pw;
  }


  /**
   * Prints a 8-bit character.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const char value) {
    *pw << value;
    return pw;
  }

  
  /**
   * Prints a wide character character.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const wchar_t value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints a C-style string.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const char *cstr) {
    *pw << cstr;
    return pw;
  }
  
  
//  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const std::string& value) {
//    *pw << value;
//    return pw;
//  }

  
//  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const std::wstring& value) {
//    *pw << value;
//    return pw;
//  }

  
  /**
   * Prints a UString.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const RefConst<UString> value) {
    *pw << value;
    return pw;
  }
  
  
  /**
   * Prints any stream object.
   *
   * @return reference to self.
   */

  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const Streamer& value) {
    *pw << value;
    return pw;
  }
  
  
} // namespace kfoundation