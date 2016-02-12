cmake_minimum_required (VERSION 3.0)
project (Containers)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden")

include_directories(SYSTEM /usr/local/include)
link_directories(/usr/local/lib)

add_executable (threadTest Person.cpp)

target_link_libraries (threadTest kfoundation cityhash unistring)
