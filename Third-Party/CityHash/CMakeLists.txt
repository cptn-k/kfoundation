cmake_minimum_required(VERSION 2.6)
project (CityHash)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden")

add_library(cityhash STATIC
  src/cityhash/city.cc)

set_target_properties (cityhash
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY ${K_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${K_BIN_DIR})

install(TARGETS cityhash
  ARCHIVE DESTINATION lib)

install(FILES src/cityhash/city.h
  DESTINATION include/cityhash)
