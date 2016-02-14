cmake_minimum_required (VERSION 2.8)
project (PassingApple)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden -pthread -ldl")

include_directories(SYSTEM /usr/local/include)
link_directories(/usr/local/lib)

add_executable (passApple Apple.cpp Person.cpp)

target_link_libraries (passApple kfoundation cityhash unistring)
