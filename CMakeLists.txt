cmake_minimum_required (VERSION 2.6)

project (KFoundation)
set(VERSION 1.0)

set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_FLAGS "-fvisibility=hidden -rdynamic")

add_subdirectory (Third-Party/CityHash     build-tmp/CityHash)
add_subdirectory (Third-Party/GnuUnistring build-tmp/GnuUnistring)

include_directories(SYSTEM Third-Party/CityHash/src)
include_directories(SYSTEM Third-Party/GnuUnistring/src)

add_library (kfoundation STATIC
# --- KFoundation --- #
  src/kfoundation/KFException.cpp
  src/kfoundation/System.cpp
  src/kfoundation/Thread.cpp
  src/kfoundation/Mutex.cpp
  src/kfoundation/Condition.cpp
# --- Memory Management --- #
  src/kfoundation/Ptr.cpp
  src/kfoundation/ManagedObject.cpp
  src/kfoundation/MemoryManager.cpp
  src/kfoundation/MasterMemoryManager.cpp
  src/kfoundation/RefCountMemoryManager.cpp
  src/kfoundation/MemoryException.cpp
  src/kfoundation/NullPointerException.cpp
  src/kfoundation/InvalidPointerException.cpp
  src/kfoundation/OutOfMemoryException.cpp
# --- IO --- #
  src/kfoundation/File.cpp
  src/kfoundation/Path.cpp
  src/kfoundation/InternetAddress.cpp
  src/kfoundation/FileInputStream.cpp
  src/kfoundation/BufferInputStream.cpp
  src/kfoundation/StringInputStream.cpp
  src/kfoundation/BufferOutputStream.cpp
  src/kfoundation/FileOutputStream.cpp
  src/kfoundation/InternetInputStream.cpp
  src/kfoundation/InternetOutputStream.cpp
  src/kfoundation/StandardInputStreamAdapter.cpp
  src/kfoundation/StandardOutputStreamAdapter.cpp
  src/kfoundation/IOException.cpp
# --- Containers --- #
  src/kfoundation/IndexOutOfBoundException.cpp
# --- Type Wrappers --- #
  src/kfoundation/Bool.cpp
  src/kfoundation/Double.cpp
  src/kfoundation/Int.cpp
  src/kfoundation/LongInt.cpp
  src/kfoundation/UniChar.cpp
  src/kfoundation/UniString.cpp
# --- Utils --- #
  src/kfoundation/PredictiveParserBase.cpp
  src/kfoundation/CodeLocation.cpp
  src/kfoundation/CodeRange.cpp
  src/kfoundation/Timer.cpp
  src/kfoundation/Logger.cpp
  src/kfoundation/ParseException.cpp
  src/kfoundation/InvalidFormatException.cpp
# --- Serialization --- #
  src/kfoundation/Streamer.cpp
  src/kfoundation/ObjectSerializer.cpp
  src/kfoundation/SerializingStreamer.cpp
  src/kfoundation/ObjectStreamReader.cpp
  src/kfoundation/StreamDeserializer.cpp
  src/kfoundation/XmlObjectStreamReader.cpp
# --- Region Arithmatics --- #
  src/kfoundation/Tuple.cpp
  src/kfoundation/Direction.cpp
  src/kfoundation/Range.cpp
  src/kfoundation/DirectionIterator.cpp
  src/kfoundation/RangeIterator.cpp
  src/kfoundation/ProximityIterator.cpp
  src/kfoundation/RangeMap.cpp)

target_link_libraries (kfoundation
  LINK_PUBLIC cityhash unistring)

set_target_properties(
  kfoundation
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY lib)

install(TARGETS kfoundation
  ARCHIVE DESTINATION lib)

install(FILES
    # --- KFoundation --- #
    src/kfoundation/definitions.h
    src/kfoundation/KFException.h
    src/kfoundation/System.h
    src/kfoundation/Thread.h
    src/kfoundation/Mutex.h
    src/kfoundation/Condition.h
    # --- Memory Management --- #
    src/kfoundation/ManagedObject.h
    src/kfoundation/PtrDecl.h
    src/kfoundation/Ptr.h
    src/kfoundation/MemoryManager.h
    src/kfoundation/MasterMemoryManager.h
    src/kfoundation/RefCountMemoryManager.h
    src/kfoundation/ObjectPoolMemoryManagerDecl.h
    src/kfoundation/ObjectPoolMemoryManager.h
    src/kfoundation/MemoryException.h
    src/kfoundation/NullPointerException.h
    src/kfoundation/InvalidPointerException.h
    src/kfoundation/OutOfMemoryException.h
    # --- IO --- #
    src/kfoundation/File.h
    src/kfoundation/Path.h
    src/kfoundation/InternetAddress.h
    src/kfoundation/InputStream.h
    src/kfoundation/FileInputStream.h
    src/kfoundation/BufferInputStream.h
    src/kfoundation/StringInputStream.h
    src/kfoundation/OutputStream.h
    src/kfoundation/BufferOutputStream.h
    src/kfoundation/FileOutputStream.h
    src/kfoundation/InternetInputStream.h
    src/kfoundation/InternetOutputStream.h
    src/kfoundation/StandardInputStreamAdapter.h
    src/kfoundation/StandardOutputStreamAdapter.h
    src/kfoundation/IOException.h
    # --- Containers ---- #
    src/kfoundation/ArrayDecl.h
    src/kfoundation/Array.h
    src/kfoundation/NumericVectorDecl.h
    src/kfoundation/NumericVector.h
    src/kfoundation/ManagedArrayDecl.h
    src/kfoundation/ManagedArray.h
    src/kfoundation/IndexOutOfBoundException.h
    # --- Type Wrappers --- #
    src/kfoundation/Bool.h
    src/kfoundation/Double.h
    src/kfoundation/Int.h
    src/kfoundation/LongInt.h
    src/kfoundation/UniChar.h
    src/kfoundation/UniString.h
    # --- Utils --- #
    src/kfoundation/PredictiveParserBase.h
    src/kfoundation/CodeLocation.h
    src/kfoundation/CodeRange.h
    src/kfoundation/Timer.h
    src/kfoundation/Logger.h
    src/kfoundation/ParseException.h
    # --- Serialization --- #
    src/kfoundation/InvalidFormatException.h
    src/kfoundation/ObjectSerializer.h
    src/kfoundation/ObjectStreamReader.h
    src/kfoundation/SerializingStreamer.h
    src/kfoundation/StreamDeserializer.h
    src/kfoundation/Streamer.h
    src/kfoundation/XmlObjectStreamReader.h
    # --- Region Arithmatics --- #
    src/kfoundation/Tuple.h
    src/kfoundation/Direction.h
    src/kfoundation/Range.h
    src/kfoundation/RangeIterator.h
    src/kfoundation/DirectionIterator.h
    src/kfoundation/ProximityIterator.h
    src/kfoundation/RangeMap.h
  DESTINATION include/kfoundation)
