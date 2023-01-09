cmake_minimum_required(VERSION 3.24)
project(Escapist)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    link_libraries(ws2_32)
endif ()

add_executable(Escapist main.cpp Escapist/General.h Escapist/Common/Internal/ReferenceCount.h Escapist/Common/Internal/TypeTrait.h Escapist/Common/ArrayList.h Escapist/Common/ByteArray.h Escapist/Common/Tuple.h Escapist/Common/String.h Escapist/Common/Thread.h Escapist/Common/Flag.h Escapist/Common/Json.h Escapist/Common/Socket.h Escapist/Common/Convert.h)
