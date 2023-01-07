//
// Created by Escap on 1/6/2023.
//

#ifndef ESCAPIST_REFERENCECOUNT_H
#define ESCAPIST_REFERENCECOUNT_H

#include "../../General.h"
#include <atomic>

namespace EscapistPrivate {
    class ReferenceCount final {
    private:
        std::atomic<int> atom;

    public:
        ReferenceCount() = delete;

        explicit ReferenceCount(const int &value) noexcept: atom(value) {}

        ReferenceCount(const ReferenceCount &other) = delete;

        int GetValue() const {
            return atom.load(std::memory_order::memory_order_acquire);
        }

        ReferenceCount &SetValue(const int &value) {
            atom.store(value, std::memory_order::memory_order_release);
            return *this;
        }

        ReferenceCount &IncrementRef() {
            atom.fetch_add(1, std::memory_order::memory_order_acq_rel);
            return *this;
        }

        ReferenceCount &DecrementRef() {
            atom.fetch_sub(1, std::memory_order::memory_order_acq_rel);
            return *this;
        }
    };
}

#endif //ESCAPIST_REFERENCECOUNT_H
