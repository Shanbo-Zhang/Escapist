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

        }
        return String('0');
    }

public:
    static String ToString(int)
};

#endif //ESCAPIST_CONVERT_H
