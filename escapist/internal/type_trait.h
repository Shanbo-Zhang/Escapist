#ifndef ESCAPIST_TYPE_TRAIT_H
#define ESCAPIST_TYPE_TRAIT_H

#include "../base.h"
#include <cstring>
#include <cstdlib>
#include <type_traits>

template<typename T>
struct TypeTrait {
    static void Copy(T *dest, const T *src, SizeType count);

    static void Move(T *dest, const T *src, SizeType count);

    static void Assign(T *dest, const T &v);

    static bool Equals(const T &left, const T &right);

    static void Destroy(T *dest);
};

namespace Internal {
    template<typename T>
    struct PodTypeTrait : public TypeTrait<T> {
        static inline void Copy(T *dest, const T *src, SizeType count) {
            ::memcpy(dest, src, count * sizeof(T));
        }

        static void Move(T *dest, const T *src, SizeType count) {
            ::memmove(dest, src, count * sizeof(T));
        }

        static void Assign(T *dest, const T &v) {
            new(dest)T(v);
        }

        static bool Equals(const T &left, const T &right) {
            return (left == right);
        }

        static void Destroy(T *dest) {}
    };

    template<typename T>
    struct GenericTypeTrait : public TypeTrait<T> {
        static inline void Copy(T *dest, const T *src, SizeType count) {
            for (; count > 0; ++dest, ++src, --count) {
                new(dest)T(*src);
            }
        }

        static void Move(T *dest, const T *src, SizeType count) {
            if (dest <= src || dest >= (src + count)) {
                for (; count > 0; ++dest, ++src, --count)
                    new(dest)T(*src);
            } else {
                dest = dest + count - 1;
                src = src + count - 1;

                for (; count > 0; --dest, --src, --count)
                    new(dest)T(*src);
            }
        }

        static void Assign(T *dest, const T &v) {
            new(dest)T(v);
        }

        static bool Equals(const T &left, const T &right) {
            return left == right;
        }

        static void Destroy(T *dest) {
            dest->~T();
        }
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
        using Type = Internal::PodTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Pod)>::type> {
        using Type = Internal::PodTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Generic)>::type> {
        using Type = Internal::GenericTypeTrait<T>;
    };

    template<typename T>
    struct TypeTraitPatternSelector<T,
            typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::NonDefault)>::type> {
        using Type = TypeTrait<T>;
    };
}


#define DefineTypeTrait(T, P) \
template<>\
struct Internal::TypeTraitPatternDefiner<T>\
{\
    static const Internal::TypeTraitPattern Pattern = P;\
}
#define DefinePodTypeTrait(T) DefineTypeTrait(T,Internal::TypeTraitPattern::Pod)
#define DefineGenericTypeTrait(T) DefineTypeTrait(T,Internal::TypeTraitPattern::Generic)
#define DefineNonDefaultTypeTrait(T) DefineTypeTrait(T,Internal::TypeTraitPattern::NonDefault)

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

#endif //ESCAPIST_TYPE_TRAIT_H
