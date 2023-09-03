cmake_minimum_required(VERSION 3.22)
project(Escapist)

set(CMAKE_CXX_STANDARD 14)

add_executable(Escapist main.cpp escapist/base.h escapist/string.h escapist/list.h escapist/internal/ref_count.h escapist/internal/type_trait.h
        escapist/time.h
        escapist/stack.h
)
