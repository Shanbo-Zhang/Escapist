cmake_minimum_required(VERSION 3.24)
project(Escapist)

set(CMAKE_CXX_STANDARD 11)

add_executable(Escapist main.cpp General.h Common/Internal/ReferenceCount.h Common/Internal/TypeTrait.h Common/ArrayList.h)
