cmake_minimum_required (VERSION 3.0)
project (Containers)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden")

include_directories(SYSTEM /usr/local/include)
link_directories(/usr/local/lib)

add_executable (container Person.cpp)

target_link_libraries (container kfoundation cityhash unistring)