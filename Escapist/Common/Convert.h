//
// Created by Escap on 1/9/2023.
//

#ifndef ESCAPIST_CONVERT_H
#define ESCAPIST_CONVERT_H

#include "String.h"

class Convert {
private:
    template<typename Type>
    static String IntegerToString(Type value) {
        static_assert(std::is_integral<Type>::value, "The type must be integer!");
        if (value) {
            String result;
            bool isMinus = value < 0;
            if (isMinus) {
                value = -value;
            }
            while (value) {
                result.Append(48 + (value % 10));
                value /= 10;
            }
            return (String &&) (isMinus ? result.Append('-').Reverse() : result.Reverse());
        }
        return String('0');
    }

    template<typename Type>
    static Type StringToInteger(const String &str) {
        if (str.GetConstAt(0) == '0' && str.GetConstAt(1) == 'x') {
            const Char *curr = str.GetConstData() + 2;
            Type result = 0;
            while (*curr) {
                result = result * 16;
                if (*curr >= 0 && *curr <= 9) {
                    result += (*curr - '0');
                } else if (*curr >= 'A' && *curr <= 'F') {
                    result += (*curr - 'A' + 10);
                } else if (*curr >= 'a' && *curr <= 'f') {
                    result += (*curr - 'a' + 10);
                } else {
                    assert(false);
                }
                ++curr;
            }
            return result;
        } else {
            bool isMinus = str.GetConstAt(0) == '-';
            const Char *curr = isMinus ? str.GetConstData() + 1 : str.GetConstData();
            Type result = 0;
            while (*curr && ::isdigit(*curr)) {
                result *= 10;
                result += *curr;
                ++curr;
            }
            return isMinus ? -result : result;
        }
    }

    template<typename Type>
    static String UnsignedIntegerToString(Type value) {
        static_assert(std::is_integral<Type>::value, "The type must be integer!");
        static_assert(std::is_unsigned<Type>::value, "The type must be unsigned!");
        if (value) {
            String result;
            while (value) {
                result.Append(48 + (value % 10));
                value /= 10;
            }
            return (String &&) result.Reverse();
        }
        return String('0');
    }

    template<typename Type>
    static Type StringToUnsignedInteger(const String &str) {
        if (str.GetConstAt(0) == '0' && str.GetConstAt(1) == 'x') {
            const Char *curr = str.GetConstData() + 2;
            Type result = 0;
            while (*curr) {
                result = result * 16;
                if (*curr >= 0 && *curr <= 9) {
                    result += (*curr - '0');
                } else if (*curr >= 'A' && *curr <= 'F') {
                    result += (*curr - 'A' + 10);
                } else if (*curr >= 'a' && *curr <= 'f') {
                    result += (*curr - 'a' + 10);
                } else {
                    assert(false);
                }
                ++curr;
            }
            return result;
        } else {
            const Char *curr = str.GetConstData();
            Type result = 0;
            while (*curr && ::isdigit(*curr)) {
                result *= 10;
                result += *curr;
                ++curr;
            }
            return result;
        }
    }

public:
    static String ToString(short value) {
        return (String &&) Convert::IntegerToString<short>(value);
    }

    static String ToString(unsigned short value) {
        return (String &&) Convert::UnsignedIntegerToString<unsigned short>(value);
    }

    static String ToString(int value) {
        return (String &&) Convert::IntegerToString<int>(value);
    }

    static String ToString(unsigned int value) {
        return (String &&) Convert::UnsignedIntegerToString<unsigned int>(value);
    }

    static String ToString(long value) {
        return (String &&) Convert::IntegerToString<long>(value);
    }

    static String ToString(unsigned long value) {
        return (String &&) Convert::UnsignedIntegerToString<unsigned long>(value);
    }

    static String ToString(long long value) {
        return (String &&) Convert::IntegerToString<long long>(value);
    }

    static String ToString(unsigned long long value) {
        return (String &&) Convert::UnsignedIntegerToString<unsigned long long>(value);
    }

    static char ParseChar(const String &string) {
        return Convert::StringToInteger<char>(string);
    }

    static unsigned char ParseByte(const String &string) {
        return Convert::StringToUnsignedInteger<unsigned char>(string);
    }

    static short ParseShort(const String &string) {
        return Convert::StringToInteger<short>(string);
    }

    static unsigned short ParseUShort(const String &string) {
        return Convert::StringToUnsignedInteger<unsigned short>(string);
    }

    static short ParseInt(const String &string) {
        return Convert::StringToInteger<int>(string);
    }

    static unsigned short ParseUInt(const String &string) {
        return Convert::StringToUnsignedInteger<unsigned int>(string);
    }

    static short ParseLong(const String &string) {
        return Convert::StringToInteger<long>(string);
    }

    static unsigned short ParseULong(const String &string) {
        return Convert::StringToUnsignedInteger<unsigned long>(string);
    }

    static short ParseLongLong(const String &string) {
        return Convert::StringToInteger<long long>(string);
    }

    static unsigned short ParseULongLong(const String &string) {
        return Convert::StringToUnsignedInteger<unsigned long long>(string);
    }
};

#endif //ESCAPIST_CONVERT_H
