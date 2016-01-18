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
#include "Int.h"
#include "LongInt.h"
#include "Double.h"
#include "Streamer.h"
#include "UString.h"
#include "UChar.h"

// Self
#include "PrintWriter.h"

namespace kfoundation {

// --- STATIC FIELDS --- //

  const kf_octet_t PrintWriter::HEX_DIGITS[]
    = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
       'A', 'B', 'C', 'D', 'E', 'F'};


// --- (DE)CONSTRUCTORS --- //

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
    for(register kf_int32_t i = count - 1; i >= 0; i--) {
      kf_octet_t value = values[i];
      buffer[i*2] = HEX_DIGITS[value >> 4];
      buffer[i*2 + 1] = HEX_DIGITS[value & 0xF];
    }
  }


// --- METHODS --- //

  Ref<OutputStream> PrintWriter::getStream() const {
    return _stream;
  }


  Ref<PrintWriter> PrintWriter::print(const bool value) {
    Bool(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const kf_int32_t value) {
    Int(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const kf_int64_t value) {
    LongInt(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const float value) {
    Double(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const double value) {
    Double(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const char value) {
    UChar(value).printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const wchar_t value) {
    UChar(value).printToStream(_stream);
    return this;
  }

    
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

  
  Ref<PrintWriter> PrintWriter::print(RefConst<kfoundation::UString> value) {
    value->printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const Streamer& value) {
    value.printToStream(_stream);
    return this;
  }


  Ref<PrintWriter> PrintWriter::print(const Streamer& value, kf_int32_t count) {
    for(int i = count - 1; i >= 0; i--) {
      value.printToStream(_stream);
    }
    return this;
  }


  Ref<PrintWriter> PrintWriter::printHex(kf_octet_t value) {
    kf_octet_t digits[2];
    printHex(digits, &value, 1, false);
    _stream->write(digits, 2);
    return this;
  }


  Ref<PrintWriter> PrintWriter::printHex(const kf_int32_t value) {
    kf_octet_t digits[8];
    printHex(digits, (const kf_octet_t*)&value, 4, System::isBigEndian());
    _stream->write(digits, 8);
    return this;
  }


  Ref<PrintWriter> PrintWriter::printHex(const kf_int64_t value) {
    kf_octet_t digits[16];
    printHex(digits, (const kf_octet_t*)&value, 8, System::isBigEndian());
    _stream->write(digits, 16);
    return this;
  }


  Ref<PrintWriter> PrintWriter::printHex(const kf_octet_t* octets,
      kf_int32_t count)
  {
    kf_octet_t* buffer = new kf_octet_t[count * 2];
    printHex(buffer, octets, count, true);
    _stream->write(buffer, count*2);
    return this;
  }


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


  Ref<PrintWriter> PrintWriter::newLine() {
    _stream->write('\n');
    return this;
  }


  void PrintWriter::over() {
    newLine();
  }


  void PrintWriter::operator<<(const printwriter_end_stream_flag_t flag) {
    over();
  }


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


  PrintWriter& PrintWriter::operator<<(const bool value) {
    Bool(value).printToStream(_stream);
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const kf_int32_t value) {
    if(_hex) {
      printHex(value);
      _hex = false;
    } else {
      Int(value).printToStream(_stream);
    }
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const kf_int64_t value) {
    if(_hex) {
      printHex(value);
    } else {
      LongInt(value).printToStream(_stream);
    }
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const float value) {
    Double(value).printToStream(_stream);
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const double value) {
    Double(value).printToStream(_stream);
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const char value) {
    UChar(value).printToStream(_stream);
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const wchar_t value) {
    UChar(value).printToStream(_stream);
    return *this;
  }

  
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

  
  PrintWriter& PrintWriter::operator<<(RefConst<UString> value) {
    value->printToStream(_stream);
    return *this;
  }


  PrintWriter& PrintWriter::operator<<(const Streamer& value) {
    value.printToStream(_stream);
    return *this;
  }

  
  void operator<<(Ref<PrintWriter> pw, const printwriter_end_stream_flag_t flag) {
    pw->over();
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const printwriter_flag flag) {
    *pw << flag;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const bool value) {
    *pw << value;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int32_t value) {
    *pw << value;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int64_t value) {
    *pw << value;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const float value) {
    *pw << value;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const double value) {
    *pw << value;
    return pw;
  }


  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const char value) {
    *pw << value;
    return pw;
  }

  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const wchar_t value) {
    *pw << value;
    return pw;
  }
  
  
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

  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const RefConst<UString> value) {
    *pw << value;
    return pw;
  }
  
  
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const Streamer& value) {
    *pw << value;
    return pw;
  }
  
  
} // namespace kfoundation