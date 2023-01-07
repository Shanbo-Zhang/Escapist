//
// Created by Escap on 1/6/2023.
//

#ifndef ESCAPIST_TYPETRAIT_H
#define ESCAPIST_TYPETRAIT_H

#include "../../General.h"
#include <memory>
#include <type_traits>

template<typename T>
class TypeTrait {
public:
    // Copy sized data from src to dest.
    static void Copy(T *dest, const T *src, SizeType size) noexcept {
        ::memcpy((void *) dest, (const void *) src, size * sizeof(T));
    }

    // Different from Copy (memcpy), it'll detect ...
    static void Move(T *dest, const T *src, SizeType size) noexcept {
        ::memmove((void *) dest, (const void *) src, size * sizeof(T));
    }

    static void Fill(T *dest, const T &value, SizeType count) noexcept {
        for (; count > 0; --count, ++dest) {
            ::memcpy((void *) dest, (const void *) &value, sizeof(T));
        }
    }

    static void Destroy(T *dest) noexcept {
        dest->~T();
    }

    static void Destroy(T *dest, SizeType count) noexcept {
        for (; count > 0; --count, ++dest) {
            dest->~T();
        }
    }
};

namespace EscapistPrivate {
    template<typename T>
    class PodTypeTrait : public TypeTrait<T> {

    public:
        static void Copy(T *dest, const T *src, SizeType size) noexcept {
            ::memcpy((void *) dest, (const void *) src, size * sizeof(T));
        }

        static void Move(T *dest, const T *src, SizeType size) noexcept {
            ::memmove((void *) dest, (const void *) src, size * sizeof(T));
        }

        static void Fill(T *dest, const T &value, size_t count) noexcept {
            for (; count > 0; --count, ++dest) {
                ::memcpy((void *) dest, (const void *) &value, sizeof(T));
            }
        }

        static void Destroy(T *dest) noexcept {}

        static void Destroy(T *dest, SizeType count) noexcept {}
    };

    template<typename T>
    class GenericTypeTrait : public TypeTrait<T> {
    public:
        static void Copy(T *dest, const T *src, SizeType size) noexcept {
            for (; size > 0; ++dest, ++src, --size)
                new(dest)T(*src);
        }

        static void Move(T *dest, const T *src, SizeType size) noexcept {
            if (dest <= src || dest >= (src + size)) {
                for (; size > 0; ++dest, ++src, --size)
                    new(dest)T(*src);
            } else {
                dest = dest + size - 1;
                src = src + size - 1;

                for (; size > 0; --dest, --src, --size)
                    new(dest)T(*src);
            }
        }

        static void Fill(T *dest, const T &val, SizeType count) noexcept {
            for (; count > 0; --count, ++dest)
                new(dest)T(val);
        }

        static void Destroy(T *dest) noexcept {}

        static void Destroy(T *dest, SizeType count) noexcept {}
    };

    enum class TypeTraitPattern : short {
        Pod,
        Generic,
        NonDefault
    };

    template<typename T>
    constexpr bool TypeTraitPatternAutoDefiner = !std::is_arithmetic<T>::value;

    template<typename T>
    struct TypeTraitPatternDefiner {
        static const TypeTraitPattern Pattern = TypeTraitPatternAutoDefiner<T> ? TypeTraitPattern::Generic
                                                                               : TypeTraitPattern::Pod;
    };

    template<typename T, typename = void>
    struct TypeTraitPatternSelector {
        using TypeTrait = EscapistPrivate::PodTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Pod)>::type> {
        using TypeTrait = EscapistPrivate::PodTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Generic)>::type> {
        using TypeTrait = EscapistPrivate::GenericTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::NonDefault)>::type> {
        using TypeTrait = TypeTrait<T>;
    };
}

#define DefineTypeTrait(T, P) \
template<>\
struct EscapistPrivate::TypeTraitPatternDefiner<T>\
{\
    static const EscapistPrivate::TypeTraitPattern Pattern = P;\
}
#define DefinePodTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::Pod)
#define DefineGenericTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::Generic)
#define DefineNonDefaultTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::NonDefault)

DefinePodTypeTrait(bool);
DefinePodTypeTrait(char);
DefinePodTypeTrait(signed char);
DefinePodTypeTrait(unsigned char);
DefinePodTypeTrait(wchar_t);
DefinePodTypeTrait(char16_t);
DefinePodTypeTrait(char32_t);
DefinePodTypeTrait(short);
DefinePodTypeTrait(unsigned short);
DefinePodTypeTrait(int);
DefinePodTypeTrait(unsigned int);
DefinePodTypeTrait(long);
DefinePodTypeTrait(unsigned long);
DefinePodTypeTrait(long long);
DefinePodTypeTrait(unsigned long long);
DefinePodTypeTrait(float);
DefinePodTypeTrait(double);
DefinePodTypeTrait(long double);

#endif //ESCAPIST_TYPETRAIT_H
