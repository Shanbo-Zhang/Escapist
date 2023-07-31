#ifndef ESCAPIST_LIST_H
#define ESCAPIST_LIST_H

// TODO: Introduction

#include "base.h"
#include "internal/ref_count.h"
#include "internal/type_trait.h"

template<typename T>
class List : Collection<T> {
public:
    /**
     * Creates an instance with no elements in the list.
     */
    List() noexcept: data_(nullptr), first_(nullptr), last_(nullptr), end_(nullptr) {}

    /**
     * Creates an instance with \p count of \p val;
     * reserves \p front_offset before the first \p val and \p back_offset after the last \p val in the list.
     * @param val the value to be put into the list
     * @param count the amount of \p val the list needs to have
     * @param front_offset the amount of space the list needs to reserve before the first \p val
     * @param back_offset the amount of space the list needs to reserve after the last \p val
     */
    List(const T &val, SizeType count, SizeType front_offset = 0, SizeType back_offset = 0) {
        if (count) {
            // Preset all pointers in the instance to appropriate addresses;
            // Reserve enough space for future assignment or copy
            for (T *pos = List<T>::SimpleAllocate(front_offset + count + front_offset, nullptr) + front_offset;
                 count > 0;
                 --count, ++pos) {
                TypeTrait::Assign(pos, val); // Successively copy the given value to the suitable place.
            }
        } else {
            new(this)List<T>();
        }
    }

    /**
     * Creates an instance with the first \p size of some given \p data;
     * reserves \p front_offset before the first \p val and \p back_offset after the last \p val in the list.
     * @param data the given data
     * @param size the amount of \p data to be copied.
     * @param front_offset the amount of space the list needs to reserve before the first \p val
     * @param back_offset the amount of space the list needs to reserve after the last \p val
     * @warning \p size cannot exceed the memory size of \p data.
     */
    List(const T *data, SizeType size, SizeType front_offset = 0, SizeType back_offset = 0) {
        if (data && size) {
            // Preset all pointers in the instance to appropriate addresses;
            // Reserve enough space for future assignment or copy;
            // Copy the given data to the suitable place.
            TypeTrait::Copy(
                    List<T>::SimpleAllocate(front_offset + size + back_offset, nullptr) + front_offset,
                    data,
                    size
            );
        } else {
            new(this)List<T>();
        }
    }

    /**
     * Copy Constructor\n
     * TODO: finish this comment
     * @param other
     */
    List(const List<T> &other) noexcept {
        if (other.data_ && other.first_) {
            // I don't want to assign them separately -_- emmmm
            // These two instance (other and this) will associate the same memory address.
            Internal::PodTypeTrait<List<T>>::Copy(this, &other, 1);
            if (*data_) {
                (**data_).IncrementRef();
            } else {
                *data_ = new List::RefCount{2};
            }
        } else {
            new(this)List<T>();
        }
    }

    /**
     * Decreases the reference count if this instance is sharing memory with other instances;
     * Otherwise, deallocates the associated memory and reference count (if has)
     */
    ~List() {
        if (data_ && first_) { // Check if we need to do something
            if (*data_) { // This instance has associated reference count instance.
                if ((**data_).Value() > 1) { // This instance is sharing with some other instance
                    (**data_).DecrementRef();
                    return; // We don't need to further deallocate them.
                } else { // Though it has reference count instance, no one is sharing with it.
                    delete *data_;
                }
            }
            for (T *each = first_; each != last_; ++each) {
                TypeTrait::Destroy(each);
            }
            ::free(reinterpret_cast<void *>(data_));
        }
    }

protected:

private:
    // List use reference count at first, if the instance will be copied.
    using RefCount = typename Internal::ReferenceCount;

    // TODO: How this is implemented?
    using TypeTrait = typename Internal::TypeTraitPatternSelector<T>::Type;

    // If the type is very small, List use maximum capacity to prevent frequent copy-and-paste of small memory.
    static constexpr SizeType kMaxCapacity = sizeof(T *) * 8 / sizeof(T);

    // If the type is too large to reserve, this is false.
    static constexpr bool kHaveMaxCapacity = kMaxCapacity;

    static RefCount **TryAllocate(const SizeType &capacity) {
        if (capacity) {
            RefCount **data = reinterpret_cast<RefCount **>(
                    ::malloc(sizeof(RefCount *) + capacity * sizeof(T))
            );
            assert(data);
            return data;
        }
        return nullptr;
    }

    static RefCount **TryReallocate(RefCount **data, const SizeType &capacity) {
        if (data && capacity) {
            RefCount **data = reinterpret_cast<RefCount **>(
                    ::realloc(data, sizeof(RefCount *) + capacity * sizeof(T))
            );
            assert(data);
            return data;
        }
        return nullptr;
    }

    /**
     * Calculates the required capacity based on different given \p size.
     * @param size the given size.
     * @return result capacity
     */
    static constexpr SizeType Capacity(const SizeType &size) {
        if (size) {
            if (kHaveMaxCapacity && kMaxCapacity < size) {
                return kMaxCapacity;
            } else {
                return SizeType(size * 1.5);
            }
        }
        return 0;
    }

    RefCount **data_;
    T *first_;
    T *last_;
    T *end_;

    /**
     * Allocates memory and assign pointers to appropriate addresses.\n
     * 1) calculate capacity based on the given \p size;\n
     * 2) allocate the data;\n
     * 3) assign the reference count, first, last, end.\n
     * @param size the intended size
     * @param rc the given reference count pointer, can be nullptr
     * @return the allocated data
     */
    T *SimpleAllocate(SizeType size, RefCount *const &rc) {
        if (size) {
            SizeType capacity = List<T>::Capacity(size);
            data_ = List<T>::TryAllocate(capacity);
            *data_ = rc;
            first_ = (T *) (data_ + 1);
            last_ = first_ + size;
            end_ = first_ + capacity;
            return first_;
        }
        return nullptr;
    }
};

#endif //ESCAPIST_LIST_H
