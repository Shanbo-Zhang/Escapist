#ifndef ESCAPIST_LIST_H
#define ESCAPIST_LIST_H

// TODO: Introduction

#include "base.h"
#include "internal/ref_count.h"
#include "internal/type_trait.h"

template<typename T>
class List {
public:
    /**
     * Creates an empty \c List<T> instance
     */
    List() noexcept
            : data_(nullptr), first_(nullptr), last_(nullptr), end_(nullptr) {}


    /**
     * Creates an List<T> with the given \p value repeating \p count of times.
     * @param count the count of \p value intended to be plugged in
     * @param value the given \p value
     * @param front_offset the amount of spaces intended to reserve before the first \p value
     * @param back_offset the amount of spaces intended to reserve after the last \p value
     */
    List(SizeType count, const T &value, SizeType front_offset = 0, SizeType back_offset = 0) {
        if (count) {
            SizeType size = front_offset + count + back_offset;
            for (T *pos = List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr) + front_offset;
                 count > 0;
                 --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        } else {
            new(this)List<T>();
        }
    }

    /**
     *
     * @param source
     * @param count
     * @param front_offset
     * @param back_offset
     */
    List(const T *source, SizeType count, SizeType front_offset = 0, SizeType back_offset = 0) {
        if (source && count) {
            SizeType size = front_offset + count + back_offset;
            TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr) + front_offset, source, count);
        } else {
            new(this)List<T>();
        }
    }

    List(const List<T> &other) noexcept {
        if (&other == this) {
            return;
        } else {
            if (other.data_) {
                ::memcpy(this, &other, sizeof(List<T>));
                if (*data_) {
                    (**data_).IncrementRef();
                } else {
                    *data_ = new RefCount(2);
                }
            } else {
                new(this)List<T>();
            }
        }
    }

    List(List<T> &&other) noexcept {
        if (&other == this) {
            return;
        } else {
            ::memcpy(this, &other, sizeof(List<T>));
            ::memset(&other, 0, sizeof(List<T>));
        }
    }

    List(const List<T> &other, SizeType count, SizeType offset,
         SizeType front_offset = 0, SizeType back_offset = 0) {
        if (&other == this) {
            assert(false);
        }
        if (count) {
            new(this)List<T>(other.first_ + offset, count, front_offset, back_offset);
        } else {
            new(this)List<T>();
        }
    }

    /**
     * Destructs the current instance.
     * If this instance is sharing with some other instances, then this do nothing,
     * except decreasing the reference count.
     * Otherwise, destructor will free the reference count (if has) and the entire
     * memory associated with this instance.
     */
    ~List() {
        if (data_) { // check if it needs to do something
            if (*data_) { // check if it has RC
                if ((**data_).Value() > 1) { // sharing
                    (**data_).DecrementRef(); // decrease and quit
                    return;
                }
                delete *data_; // = 1, thus the RC is no longer needed.
            }
            // We need to free the memory.
            // To avoid memory leak, we need to run the destructor of every existing element.
            for (T *pos = first_; pos != end_; ++pos) {
                TypeTrait::Destroy(pos);
            }
            ::free(static_cast<void *>(data_)); // NO MEMORY LEAK AT ALL!!!!!!
        }
    }

    T *Data() {
        if (data_) {
            if ((*data_) && (**data_).Value() > 1) {
                T *old = first_;
                SizeType size = last_ - first_;
                (**data_).DecrementRef();
                TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr), old, size);
            }
            return first_;
        }
        return nullptr;
    }

    const T *ConstData() const noexcept {
        return first_;
    }

    T &At(SizeType index) {
        assert(data_);
        SizeType size = last_ - first_;
        assert(index < size);
        if (*data_) {
            if ((*data_) && (**data_).Value() > 1) {
                T *old = first_;
                (**data_).DecrementRef();
                TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr), old, size);
            }
        }
        return *(first_ + index);
    }

    const T &ConstAt(SizeType index) const {
        assert(index < last_ - first_);
        return *(first_ + index);
    }

    SizeType Count() const noexcept {
        return data_ ? last_ - first_ : 0;
    }

    SizeType Capacity() const noexcept {
        return data_ ? end_ - first_ : 0;
    }

    bool IsEmpty() const noexcept {
        return !(data_ && (last_ == first_));
    }

    bool IsNull() const noexcept {
        return !data_;
    }

    List<T> &Clear() {
        if (data_) {
            if (*data_ && (**data_).Value() > 1) {
                (**data_).DecrementRef();
                new(this)List<T>();
            } else {
                for (; last_ != first_; --last_) {
                    TypeTrait::Destroy(last_);
                }
            }
        }
        return *this;
    }

    List<T> &EnsureCapacity(SizeType capacity) {
        if (data_) {
            if (capacity > end_ - first_) {
                SizeType size = last_ - first_;
                if (*data_ && (**data_).Value() > 1) {
                    T *old = first_;
                    (**data_).DecrementRef();
                    TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr), old, size);
                } else {
                    List<T>::SimpleReallocate(size, capacity);
                }
            }
        } else {
            List<T>::SimpleAllocate(0, capacity, nullptr);
        }
        return *this;
    }

    List<T> &Append(const T &value, SizeType count = 1,
                    SizeType front_offset = 0, SizeType back_offset = 0) noexcept {
        if (T *pos = List<T>::GrowthAppend(front_offset + count + back_offset) + front_offset) {
            for (; count > 0; --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        }
        return *this;
    }

    List<T> &Append() {

    }

public:
    using TypeTrait = typename Internal::TypeTraitPatternSelector<T>::Type;
    using RefCount = typename Internal::ReferenceCount;

    /**
     * Traditionally, the array-like list stores a collection of elements in
     * contiguous memory. However, as the size grows, it approaches the capacity.
     * When there does not have any available place to put additional elements,
     * the list usually find a larger place, copy existing elements, and free the
     * old memory.
     * \n
     * However, if we use the growing factor, when the intended capacity is small,
     * the size reaches maximum and triggers reallocation frequently, which causes
     * inefficiency. By using this, we avoid this frequent copy-and-paste when the
     * memory is small. If the intended capacity is lower than this value, the
     * capacity will use this instead.
     */
    static constexpr SizeType kMinCap = (sizeof(T *) * 8 / sizeof(T));


    /**
     * Calculates the capacity based on the given \p size.
     * It usually has three cases.
     *  - 1. the given \p size is 0.
     *  - 2. the given size is smaller than \p kMinCap.
     *  - 3. the given size is larger than \p kMinCap
     * @param size
     * @return
     */
    static constexpr SizeType Cap(SizeType size) {
        if (size) {
            if (kMinCap && kMinCap * 0.75 >= size) {
                return kMinCap;
            }
            return size * 1.5;
        }
        return 0;
    }

    /**
     * Calculates the total amount of bytes using in the instance.
     * The instances contains sizeof(RefCount*) + \p capacity * sizeof(T)
     * @param capacity the given capacity
     * @return the total amount of bytes using in the instance.
     */
    static constexpr SizeType TotCap(SizeType capacity) {
        return sizeof(RefCount *) + capacity * sizeof(T);
    }

    inline List(RefCount **&data, const T *&first, const T *&last, const T *&end)
            : data_(data), first_(first), last_(last), end_(end) {}

    inline List(RefCount **data, const SizeType &size, const SizeType &capacity)
            : data_(data), first_((T *) (data + 1)), last_(first_ + size), end_(first_ + capacity) {}

    /**
     * Allocates memory for the instance and assigns member variables.
     * Simple means it does not consider the validity of given value, and existing data.
     * @return \c first_
     */
    T *SimpleAllocate(const SizeType &size, const SizeType &capacity, RefCount *const &rc) {
        data_ = static_cast<RefCount **>(::malloc(TotCap(capacity)));
        assert(data_);
        *data_ = rc;
        first_ = (T *) (data_ + 1);
        last_ = first_ + size;
        end_ = first_ + capacity;
        return first_;
    }

    void SimpleReallocate(const SizeType &size, const SizeType &capacity) {
        RefCount **old = data_;
        RefCount *old_rc = *data_;
        data_ = static_cast<RefCount **>(::realloc(data_, TotCap(capacity)));
        assert(data_);
        if (old != data_) {
            *data_ = old_rc;
            first_ = (T *) (data_ + 1);
            last_ = first_ + size;
        }
        end_ = first_ + capacity;
    }

    /**
     * Internal implementation of all Append methods.
     * Reserves enough space in memory at the end of the existing elements
     * for further Append operation.
     * @param count count of elements to be added
     * @return the address to add new elements, or nullptr if nothing to add
     */
    T *GrowthAppend(SizeType count) {
        if (count) { // Check if we need to do something.
            if (data_) { // There exists elements, thus detect if we need to detach or expand.
                SizeType size = last_ - first_, new_size = size + count;
                if ((*data_) && (**data_).Value()) { // detach?
                    T *old = first_;
                    (**data_).DecrementRef();
                    TypeTrait::Copy(List<T>::SimpleAllocate(new_size, List<T>::Cap(new_size), nullptr), old, count);
                } else if (new_size > end_ - first_) { // expand?
                    List<T>::SimpleReallocate(new_size, List<T>::Cap(new_size));
                } else { // just reassign size, because we have ENOUGH SPACE!!!
                    last_ = first_ + new_size;
                }
                return first_ + size;
            } else {
                return List<T>::SimpleAllocate(count, List<T>::Cap(count), nullptr);
            }
        }
        return nullptr;
    }

    RefCount **data_; // The first bytes of the memory associated to this instance.
    T *first_; // The address of the first element.
    T *last_; // The address of the last element.
    T *end_; // The address of the end of the memory.
};

#endif //ESCAPIST_LIST_H
