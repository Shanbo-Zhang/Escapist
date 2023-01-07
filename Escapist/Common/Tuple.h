//
// Created by Escap on 1/7/2023.
//

#ifndef ESCAPIST_TUPLE_H
#define ESCAPIST_TUPLE_H

#include "../General.h"

template<typename ...Types>
class Tuple;

template<>
class Tuple<> {
};

template<typename This, typename ...Next>
class Tuple<This, Next...> : private Tuple<Next...> {
private:
    using Value = This;
    using Base = Tuple<Next...>;
    using Self = Tuple<This, Next...>;

protected:
    Value value;

public:
    Tuple() = delete;

    Tuple(This theValue, Next...nextValue) : value(theValue), Base(nextValue...) {}

    Value GetValue() noexcept {
        return value;
    }

    Value &GetValueRef() noexcept {
        return value;
    }

    const Value &GetConstValue() noexcept {
        return value;
    }
};

namespace EscapistPrivate {
    template<SizeType index, typename...Types>
    struct TupleElementFinder;

    template<typename This, typename ...Types>
    struct TupleElementFinder<0, Tuple<This, Types...>> {
        using Value = This;
        using Tuple = Tuple<This, Types...>;
    };

    template<SizeType index, typename This, typename ...Next>
    struct TupleElementFinder<index, Tuple<This, Next...>> {
        using Value = typename TupleElementFinder<index - 1, Tuple<Next...>>::Value;
        using Tuple = typename TupleElementFinder<index - 1, Tuple<Next...>>::Tuple;
    };

    template<>
    struct TupleElementFinder<0, Tuple<>> {
        using Value = Tuple<>;
        using Tuple = Tuple<>;
    };
}

template<SizeType index, typename ...Types>
typename EscapistPrivate::TupleElementFinder<index, Tuple<Types...>>::Value&
GetTupleValue(const Tuple<Types...> &tuple) {
    using Tuple = typename EscapistPrivate::TupleElementFinder<index, Tuple<Types...>>::Tuple;
    return ((Tuple *) &tuple)->GetValueRef();
}

#endif //ESCAPIST_TUPLE_H
