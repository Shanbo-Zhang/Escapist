//
// Created by Escap on 1/8/2023.
//

#ifndef ESCAPIST_FLAG_H
#define ESCAPIST_FLAG_H

#include "../General.h"
#include <type_traits>

template<typename Enum>
class Flag {
private:
    static_assert(std::is_enum<Enum>::value, "The type must be an enumeration type.");
    using Self = Flag<Enum>;
    using Type = typename std::underlying_type<Enum>::type;
    static_assert(std::is_arithmetic<Type>::value);
    Type i;

public:
    Flag() :i(0) {}
    explicit Flag(Type value) :i(value) {}
    explicit Flag(Enum value) :i(Type(value)) {}
    Flag(SizeType count, ...) :i(0) {
        va_list argList;
        va_start(argList, count);
        for (; count > 0; --count) {
            i |= va_arg(argList, Type);
        }
    }

    template<typename Return = Enum>
    Return GetValue()const { return i; }
    Type GetValue()const { return i; }

    Self& SetValue(Type value) { i = value; }
    Self& SetValue(Enum value) { i = (Type)value; }

    Self& AddFlag(const Type& value) { i |= value; return *this; }
    Self& AddFlag(const Enum& value) { i |= (Type)value; return *this; }
    Self& AddFlag(const Self& value) { i |= value.i; return *this; }
    Self& DeleteFlag(const Type& value) { i ^= value; return *this; }
    Self& DeleteFlag(const Enum& value) { i ^= value; return *this; }
    Self& DeleteFlag(const Self& value) { i ^= value.i; return *this; }

    bool HasFlag(const Type& value)const { return ((i & value) == value); }
    bool HasFlag(const Enum& value)const { return ((i & (Type)value) == (Type)value); }
    bool HasFlag(const Self& value)const { return ((i & value.i) == value.i); }
    bool HasAnyFlags(const Type& value)const { return (i & value) != Type(0); }
    bool HasAnyFlags(const Self& value)const { return (i & value.i) != Type(0); }
};

#endif //ESCAPIST_FLAG_H
