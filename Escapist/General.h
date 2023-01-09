//
// Created by Escap on 1/6/2023.
//

#ifndef ESCAPIST_GENERAL_H
#define ESCAPIST_GENERAL_H

// Platform detection.

#if defined(WIN32) | defined(_WIN32) | defined(__WIN32__)
#define ESCAPIST_OS_WINDOWS
#ifdef _WIN64
#define ESCAPIST_64BITS
#else
#define ESCAPIST_32BITS
#endif
#elif defined(__linux__)
#define ESCAPIST_OS_LINUX
#elif defined(__unix__)
#define ESCAPIST_OS_UNIX
#elif
#error "Unsupported Platform"
#endif

// Start defining some basic types based on different platforms
#ifdef ESCAPIST_OS_WINDOWS

using Int8 = char;
using UInt8 = unsigned char;
using Int16 = short;
using UInt16 = unsigned short;
using Int32 = int;
using UInt32 = unsigned int;
using Int64 = long long;
using UInt64 = unsigned long long;
// TODO: does those typedef different between 32 bits and 64 bits?

using Handle = void *;
#if defined(ESCAPIST_64BITS)
using Char = wchar_t;
using SizeType = unsigned long long;

#define UNICODE
#elif defined(ESCAPIST_32BITS)
using Char = char;
using SizeType = unsigned int;
#endif

#include <winsock2.h>
#include <windows.h>
#include <cassert>

#endif

#endif //ESCAPIST_GENERAL_H
