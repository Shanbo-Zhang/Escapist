#ifndef ESCAPIST_REF_COUNT_H
#define ESCAPIST_REF_COUNT_H

#include "../base.h"
#include <atomic>

namespace Internal {
    class ReferenceCount final {
    public:
        ReferenceCount() = delete;

        explicit ReferenceCount(const int &value) noexcept: atom(value) {}

        ReferenceCount(const ReferenceCount &other) = delete;

        int Value() const {
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

    private:
        std::atomic<int> atom;
    };
}

#endif //ESCAPIST_REF_COUNT_H
