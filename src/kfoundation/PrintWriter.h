//
//  PrintWriter.hpp
//  KFoundation
//
//  Created by Kay Khandan on 12/10/15.
//  Copyright © 2015 Kay Khandan. All rights reserved.
//

#ifndef PrintWriter_hpp
#define PrintWriter_hpp

#include "definitions.h"
#include "KFObject.h"
#include "Ref.h"
#include "OutputStream.h"
#include "Streamer.h"

namespace kfoundation {

  typedef enum {
    OVER
  } printwriter_end_stream_flag_t;


  typedef enum {
    NEW_LINE, ///< Line feed
    HEX       ///< Hex
  } printwriter_flag;


  class PrintWriter : public KFObject {

  // --- STATIC FIELDS --- //

    private: static const kf_octet_t HEX_DIGITS[];


  // --- FEIDLS --- //

    private: Ref<OutputStream> _stream;
    private: bool _hex;


  // --- (DE)CONSTRUCTORS --- //

    public: PrintWriter(Ref<OutputStream> stream);


  // --- STATIC METHODS --- //

    private: void printHex(kf_octet_t* buffer, const kf_octet_t* values, kf_int32_t count, bool reverse);


  // --- METHODS --- //

    public: Ref<OutputStream> getStream() const;
    public: Ref<PrintWriter> print(const bool value);
    public: Ref<PrintWriter> print(const kf_int32_t value);
    public: Ref<PrintWriter> print(const kf_int64_t value);
    public: Ref<PrintWriter> print(const float value);
    public: Ref<PrintWriter> print(const double value);
    public: Ref<PrintWriter> print(const char value);
    public: Ref<PrintWriter> print(const wchar_t value);
    public: Ref<PrintWriter> print(const char* cstr);
//  public: Ref<PrintWriter> print(const std::string& value);
//  public: Ref<PrintWriter> print(const std::wstring& value);
    public: Ref<PrintWriter> print(RefConst<UString> value);
    public: Ref<PrintWriter> print(const Streamer& value);
    public: Ref<PrintWriter> print(const Streamer& value, kf_int32_t count);
    public: Ref<PrintWriter> printHex(kf_octet_t value);
    public: Ref<PrintWriter> printHex(const kf_int32_t value);
    public: Ref<PrintWriter> printHex(const kf_int64_t value);
    public: Ref<PrintWriter> printHex(const kf_octet_t* octets, kf_int32_t count);
    public: Ref<PrintWriter> printFormat(RefConst<UString> format, ...);
    public: Ref<PrintWriter> printTime(RefConst<UString> format, kf_int64_t time);
    public: Ref<PrintWriter> newLine();
    public: virtual void over();


  // --- OPERATORS --- //

    public: void operator<<(const printwriter_end_stream_flag_t flag);
    public: PrintWriter& operator<<(const printwriter_flag flag);
    public: PrintWriter& operator<<(const bool value);
    public: PrintWriter& operator<<(const kf_int32_t value);
    public: PrintWriter& operator<<(const kf_int64_t value);
    public: PrintWriter& operator<<(const float value);
    public: PrintWriter& operator<<(const double value);
    public: PrintWriter& operator<<(const char value);
    public: PrintWriter& operator<<(const wchar_t value);
    public: PrintWriter& operator<<(const char* cstr);
//  public: PrintWriter& operator<<(const std::string& value);
//  public: PrintWriter& operator<<(const std::wstring& value);
    public: PrintWriter& operator<<(RefConst<UString> value);
    public: PrintWriter& operator<<(const Streamer& value);

  };
  
  
  void operator<<(Ref<PrintWriter> pw, const printwriter_end_stream_flag_t flag);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const printwriter_flag flag);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const bool value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int32_t value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const kf_int64_t value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const float value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const double value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const char value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const wchar_t value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const char* cstr);
//Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const std::string& value);
//Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const std::wstring& value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, RefConst<UString> value);
  Ref<PrintWriter> operator<<(Ref<PrintWriter> pw, const Streamer& value);

  
} // namespace kfoundation

#endif /* PrintWriter_hpp */