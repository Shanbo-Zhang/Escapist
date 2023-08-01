/**
 * This file is the foundation of this long-term updating library.
 * It contains several parts.
 *
 * <1> System Specialization:
 * This library is for me to record my journey of learning. Currently,
 * I am interested in lots of fields, including networks, Cryptography,
 * computer systems, computer hardware, etc. These codes will probably
 * be executed in other systems supporting C++. Thus I intended to make
 * this cross-platform, and I can learn how to write cross-platform
 * libraries.
 *
 * <2> General Types/Templates Definition:
 * Since this library will be cross-platform, to eliminate difference
 * between different libraries, codes in this library will use united types,
 * some new types might come from different primitive types in different
 * machines.
 *
 * <3> Basic Interface/Enumeration Declaration:
 * I intend to find an equilibrium between OOP and C, but for now I don't
 * prefer using only one of them. Thus the third part will be some general
 * interfaces and enumerations. probably using in everywhere in this library,
 * such as Iterator, etc.
 */

#ifndef ESCAPIST_BASE_H
#define ESCAPIST_BASE_H

/**
 * System Detection:
 * Firstly, different systems define different macros in C.
 * To detect systems, detect these macros.
 */
#if defined(_WIN32) // _WIN32: Microsoft Windows
// However, windows has 32bits and 64bits.
// Thus, we need to do something more to store them.
#define ESCAPIST_OS_WINDOWS
#ifdef _WIN64 // This is to detect if the system is 64bits.
#define ESCAPIST_64BITS
#else
#define ESCAPIST_32BITS
#endif
#elif defined(__linux__) // __linux__: Linux
#define ESCAPIST_OS_LINUX
#ifdef __x86_64__
#define ESCAPIST_64BITS
#else
#define ESCAPIST_32BITS
#endif
#elif defined(__APPLE__)
#define ESCAPIST_OS_MACOS
#elif defined(__unix__)
#define ESCAPIST_OS_UNIX
#elif
#error "Unsupported Platform"
#endif

#include <cassert>

// Type Unification:
//  - Char : the type storing a character.
//  - SizeType: the type storing size.
#ifdef ESCAPIST_OS_WINDOWS
#ifdef ESCAPIST_64BITS
using Char = wchar_t;
using SizeType = unsigned long long;
#else
using Char = char;
using SizeType = unsigned int;
#endif
#endif

#ifdef ESCAPIST_OS_LINUX
#ifdef ESCAPIST_OS_LINUX
using Char = wchar_t;
using SizeType = long unsigned int;
#elif
using Char = char;
using SizeType = unsigned int;
#endif
#endif

template<typename T>
class Collection {
};

template<typename T>
class IIterator {
public:
    IIterator() = delete;

    explicit IIterator(T *const &pos) : pos_(pos) {}

    IIterator(const IIterator<T> &it) : pos_(it.pos_) {}

    virtual ~IIterator() = default;

    virtual T &Instance() = 0;

    virtual IIterator<T> &Next() const noexcept = 0;

protected:
    T *pos_;
};

template<typename T>
class IConstIterator {
public:
    IConstIterator() = delete;

    explicit IConstIterator(T *const &pos) : pos_(pos) {}

    IConstIterator(const IConstIterator<T> &it) : pos_(it.pos_) {}

    virtual ~IConstIterator() = default;

    virtual T &Instance() = 0;

private:
    T *const &pos_;
};

#endif //ESCAPIST_BASE_H