cmake_minimum_required (VERSION 2.8)
project (Serialization)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden -pthread -ldl")

include_directories(SYSTEM /usr/local/include)
link_directories(/usr/local/lib)

add_executable (SendPerson Apple.cpp Person.cpp Sender.cpp)
add_executable (ReceivePerson Apple.cpp Person.cpp Receiver.cpp)

target_link_libraries (SendPerson kfoundation cityhash unistring)
target_link_libraries (ReceivePerson kfoundation cityhash unistring)
