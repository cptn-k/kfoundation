cmake_minimum_required (VERSION 2.6)
project (GnuUnistring)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden")

add_library (unistring STATIC
  src/unistring/ctype/ctype_alnum.cpp
  src/unistring/ctype/ctype_alpha.cpp
  src/unistring/ctype/ctype_digit.cpp
  src/unistring/ctype/ctype_lower.cpp
  src/unistring/ctype/ctype_punct.cpp
  src/unistring/ctype/ctype_space.cpp
  src/unistring/ctype/ctype_upper.cpp
  src/unistring/unicase/tolower.cpp
  src/unistring/unicase/toupper.cpp)

set_target_properties (unistring
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY ${K_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${K_BIN_DIR})

install(TARGETS unistring
  ARCHIVE DESTINATION lib)

install(FILES
    src/unistring/unicase.h
    src/unistring/unictype.h
    src/unistring/unitypes.h
  DESTINATION include/unistring)

install(FILES
    src/unistring/ctype/bitmap.h
    src/unistring/ctype/ctype_alnum.h
    src/unistring/ctype/ctype_alpha.h
    src/unistring/ctype/ctype_digit.h
    src/unistring/ctype/ctype_lower.h
    src/unistring/ctype/ctype_punct.h
    src/unistring/ctype/ctype_space.h
    src/unistring/ctype/ctype_upper.h
  DESTINATION include/unistring/ctype)

install(FILES
    src/unistring/unicase/simple-mapping.h
    src/unistring/unicase/tolower.h
    src/unistring/unicase/toupper.h
  DESTINATION include/unistring/unicase)


