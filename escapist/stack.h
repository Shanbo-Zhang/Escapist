#ifndef ESCAPIST_STACK_H
#define ESCAPIST_STACK_H

#include "base.h"

template<typename T>
class Stack {
public:
    Stack();

    Stack(const Stack<T> &other);

    Stack(Stack<T> &&other);

    ~Stack();

    bool IsEmpty() const noexcept;

    Stack<T> &Clear() noexcept;


};

#endif //ESCAPIST_STACK_H
