#ifndef ESCAPIST_LIST_H
#define ESCAPIST_LIST_H

// TODO: Introduction

#include <initializer_list>
#include "base.h"
#include "internal/ref_count.h"
#include "internal/type_trait.h"

template<typename T>
class List {
public:
    /**
     *
     */
    class Iterator {
    public:
        Iterator() = delete;

        Iterator(const Iterator &other) noexcept
                : pos_(other.pos_), index_(other.index_), from_(other.from_) {}

        Iterator(Iterator &&other) noexcept
                : pos_(other.pos_), index_(other.index_), from_(other.from_) {
            other.pos_ = other.index_ = other.from_ = 0;
        }

        List<T>::Iterator &operator=(const List<T>::Iterator &other) {
            if (this == &other) {
                return *this;
            }
            new(this)List<T>::Iterator(other.pos_, other.index_, other.from_);
        }

        bool operator==(const List<T>::Iterator &other) const noexcept {
            return from_->data_ == other.from_->data_ && pos_ == other.pos_;
        }

        bool operator!=(const List<T>::Iterator &other) const noexcept {
            return !(from_->data_ == other.from_->data_ && pos_ == other.pos_);
        }

        T &Instance() noexcept {
            return *pos_;
        }

        bool HaveNext() const noexcept {
            return index_ < (from_->end_ - from_->first_);
        }

        bool HavePrev() const noexcept {
            return index_ != 0;
        }

        List<T>::Iterator Next() const {
            // assert(List<T>::Iterator::HaveNext());
            return List<T>::Iterator(pos_ + 1, index_ + 1, from_);
        }


        List<T>::Iterator CheckedNext() const {
            assert(List<T>::Iterator::HaveNext());
            return List<T>::Iterator(pos_ + 1, index_ + 1, from_);
        }

        List<T>::Iterator Prev() const {
            // assert(List<T>::Iterator::HavePrev());
            return List<T>::Iterator(pos_ - 1, index_ - 1, from_);
        }

        List<T>::Iterator CheckedPrev() const {
            assert(List<T>::Iterator::HavePrev());
            return List<T>::Iterator(pos_ - 1, index_ - 1, from_);
        }

    private:
        T *const pos_;
        const SizeType index_;
        const List<T> *from_;

        Iterator(T *pos, SizeType index, const List<T> *from)
                : pos_(pos), index_(index), from_(from) {}

        friend class List<T>;
    };

    /**
     *
     */
    class ConstIterator {
    public:
        ConstIterator() = delete;

        ConstIterator(const List<T>::ConstIterator &other) noexcept
                : pos_(other.pos_), index_(other.index_), from_(other.from_) {}

        List<T>::ConstIterator &operator=(const List<T>::ConstIterator &other) {
            if (this == &other) {
                return *this;
            }
            new(this)List<T>::ConstIterator(other.pos_, other.index_, other.from_);
        }

        bool operator==(const List<T>::ConstIterator &other) const noexcept {
            return from_->data_ == other.from_->data_ && pos_ == other.pos_;
        }

        bool operator!=(const List<T>::ConstIterator &other) const noexcept {
            return !(from_->data_ == other.from_->data_ && pos_ == other.pos_);
        }

        const T &Instance() noexcept {
            return *pos_;
        }

        bool HaveNext() const noexcept {
            return index_ < (from_->last_ - from_->first_);
        }

        bool HavePrev() const noexcept {
            return index_ != 0;
        }

        List<T>::ConstIterator Next() const {
            assert(List<T>::ConstIterator::HaveNext());
            return List<T>::ConstIterator(pos_ + 1, index_ + 1, from_);
        }

        List<T>::ConstIterator Prev() const {
            assert(List<T>::ConstIterator::HavePrev());
            return List<T>::ConstIterator(pos_ - 1, index_ - 1, from_);
        }

    private:
        const T *const pos_;
        const SizeType index_;
        const List<T> *const from_;

        ConstIterator(const T *pos, const SizeType index, const List<T> *from)
                : pos_(pos), index_(index), from_(from) {}

        friend class List<T>;
    };

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

    /**
     *
     * @param other
     */
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

    /**
     *
     * @param other
     */
    List(List<T> &&other) noexcept {
        if (&other == this) {
            return;
        } else {
            ::memcpy(this, &other, sizeof(List<T>));
            ::memset(&other, 0, sizeof(List<T>));
        }
    }

    /**
     *
     * @param other
     * @param count
     * @param offset
     * @param front_offset
     * @param back_offset
     */
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
     *
     * @param i
     */
    List(const std::initializer_list<T> i) {
        if (SizeType s = i.size()) {
            T *pos = List<T>::SimpleAllocate(i.size(), Cap(i.size()), nullptr);
            for (auto it = i.begin(); it != i.end(); ++it, ++pos) {
                TypeTrait::Assign(pos, *it);
            }
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

    /**
     *
     * @return
     */
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

    /**
     *
     * @return
     */
    const T *ConstData() const noexcept {
        return first_;
    }

    /**
     *
     * @param index
     * @return
     */
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

    /**
     *
     * @param index
     * @return
     */
    List<T>::Iterator IteratorAt(SizeType index) {
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
        return List<T>::Iterator(data_ + index, index, this);
    }

    /**
     *
     * @param index
     * @return
     */
    const T &ConstAt(SizeType index) const {
        assert(index < last_ - first_);
        return *(first_ + index);
    }

    List<T>::ConstIterator ConstIteratorAt(SizeType index) const {
        assert(index < last_ - first_);
        return List<T>::ConstIterator(data_ + index, index, this);
    }

    List<T> &SetAt(SizeType index, const T &value) {
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
        TypeTrait::Assign(first_ + index, value);
        return *this;
    }

    /**
     *
     * @return
     */
    SizeType Count() const noexcept {
        return data_ ? last_ - first_ : 0;
    }

    /**
     *
     * @return
     */
    SizeType Capacity() const noexcept {
        return data_ ? end_ - first_ : 0;
    }

    /**
     *
     * @return
     */
    bool IsEmpty() const noexcept {
        return !(data_ && (last_ == first_));
    }

    bool IsNull() const noexcept {
        return !data_;
    }

    /**
     *
     * @return
     */
    List<T>::Iterator First() noexcept {
        if (data_ && (*data_) && (**data_).Value() > 1) {
            T *old = first_;
            SizeType size = last_ - first_;
            (**data_).DecrementRef();
            TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr), old, size);
        }
        return List<T>::Iterator(first_, 0, this);
    }

    /**
     *
     * @return
     */
    List<T>::ConstIterator ConstFirst() const noexcept {
        return List<T>::ConstIterator(first_, 0, this);
    }

    /**
     *
     * @return
     */
    List<T>::Iterator Last() noexcept {
        SizeType size = last_ - first_;
        if (data_ && (*data_) && (**data_).Value() > 1) {
            T *old = first_;
            (**data_).DecrementRef();
            TypeTrait::Copy(List<T>::SimpleAllocate(size, List<T>::Cap(size), nullptr), old, size);
        }
        return List<T>::Iterator(last_, size, this);
    }

    /**
     *
     * @return
     */
    List<T>::ConstIterator ConstLast() const noexcept {
        return List<T>::ConstIterator(last_, List<T>::Count(), this);
    }

    /**
     *
     * @return
     */
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

    /**
     *
     * @param capacity
     * @return
     */
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

    /**
     *
     * @param other
     * @param Equals
     * @return
     */
    bool Equals(const List<T> &other, bool(*Equals)(const T &, const T &)) {
        if (&other == this) {
            return true;
        } else if (other.Count() != Count()) {
            return false;
        } else {
            for (T *left_pos = first_, *right_pos = other.first_;
                 left_pos != last_;
                 ++left_pos, ++right_pos) {
                if (!Equals(*left_pos, *right_pos)) {
                    return false;
                }
            }
            return true;
        }
    }

    /**
     *
     * @param other
     * @param Compare
     * @return
     */
    bool CompareTo(const List<T> &other, int(Compare)(const T &, const T &)) {
        if (&other == this) {
            return true;
        } else if (other.Count() != Count()) {
            return false;
        } else {
            int rtn;
            for (T *left_pos = first_, *right_pos = other.first_;
                 left_pos != last_;
                 ++left_pos, ++right_pos) {
                rtn = Compare(*left_pos, *right_pos);
                if (rtn) {
                    return rtn;
                }
            }
            return 0;
        }
    }

    /**
     *
     * @param value
     * @param count
     * @param front_offset
     * @param back_offset
     * @return
     */
    List<T> &Reassign(const T &value, SizeType count = 1,
                      SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::AssignImpl(front_offset + count + back_offset) + front_offset) {
            for (; count > 0; --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        }
        return *this;
    }

    /**
     *
     * @param data
     * @param count
     * @param front_offset
     * @param back_offset
     * @return
     */
    List<T> &Reassign(const T *data, SizeType count,
                      SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::AssignImpl(front_offset + count + back_offset) + front_offset) {
            TypeTrait::Copy(pos, data, count);
        }
        return *this;
    }

    List<T> &Reassign(const List<T> &other, SizeType count = 1,
                      SizeType front_offset = 0, SizeType back_offset = 0) {
        if (data_) {
            return List<T>::Append(other.data_, other.size_, front_offset, back_offset);
        }
    }

    List<T> &Reassign(const List<T> &other, SizeType from, SizeType to,
                      SizeType front_offset = 0, SizeType back_offset = 0) {
        if (other.data_ && other.size_ && from < to && to < other.size_) {
            return List<T>::Append(other.data_ + from, to, front_offset, front_offset);
        }
        return *this;
    }

    List<T> &Append(const T &value, SizeType count = 1,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthAppend(front_offset + count + back_offset) + front_offset) {
            for (; count > 0; --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        }
        return *this;
    }

    List<T> &Append(const T *data, SizeType count,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthAppend(front_offset + count + back_offset) + front_offset) {
            TypeTrait::Copy(pos, data, count);
        }
        return *this;
    }

    List<T> &Append(const List<T> &other, SizeType count = 1,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (data_) {
            return List<T>::Append(other.data_, other.size_, front_offset, back_offset);
        }
    }

    List<T> &Append(const List<T> &other, SizeType from, SizeType to,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (other.data_ && other.size_ && from < to && to < other.size_) {
            return List<T>::Append(other.data_ + from, to, front_offset, front_offset);
        }
        return *this;
    }

    List<T> &Prepend(const T &value, SizeType count = 1,
                     SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthPrepend(front_offset + count + back_offset) + front_offset) {
            for (; count > 0; --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        }
        return *this;
    }

    List<T> &Prepend(const T *data, SizeType count,
                     SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthPrepend(front_offset + count + back_offset) + front_offset) {
            TypeTrait::Copy(pos, data, count);
        }
        return *this;
    }

    List<T> &Prepend(const List<T> &other, SizeType count = 1,
                     SizeType front_offset = 0, SizeType back_offset = 0) {
        if (data_) {
            return List<T>::Prepend(other.data_, other.size_, front_offset, back_offset);
        }
    }

    List<T> &Prepend(const List<T> &other, SizeType from, SizeType to,
                     SizeType front_offset = 0, SizeType back_offset = 0) {
        if (other.data_ && other.size_ && from < to && to < other.size_) {
            return List<T>::Prepend(other.data_ + from, to, front_offset, front_offset);
        }
        return *this;
    }

    List<T> &Insert(SizeType index, const T &value, SizeType count = 1,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthInsert(index, front_offset + count + back_offset) + front_offset) {
            for (; count > 0; --count, ++pos) {
                TypeTrait::Assign(pos, value);
            }
        }
        return *this;
    }

    List<T> &Insert(SizeType index, const T *data, SizeType count,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (T *pos = List<T>::GrowthInsert(index, front_offset + count + back_offset) + front_offset) {
            TypeTrait::Copy(pos, data, count);
        }
        return *this;
    }

    List<T> &Insert(SizeType index, const List<T> &other, SizeType count = 1,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (data_) {
            return List<T>::Insert(index, other.data_, other.size_, front_offset, back_offset);
        }
    }

    List<T> &Insert(SizeType index, const List<T> &other, SizeType from, SizeType to,
                    SizeType front_offset = 0, SizeType back_offset = 0) {
        if (other.data_ && other.size_ && from < to && to < other.size_) {
            return List<T>::Insert(index, other.data_ + from, to, front_offset, front_offset);
        }
        return *this;
    }

    /**
     * Removes the existing elements starting from \p index and continuing \p count times.
     * @param index the first element intended to be removed
     * @param count the amount of elements intended to be removed
     * @return the reference to the current instance
     */
    List<T> &Remove(SizeType index, SizeType count = 1) {
        if (data_) {
            SizeType old_size = last_ - first_;
            assert(index + count < old_size);
            SizeType new_size = old_size - count;
            if (*data_ && (**data_).Value() > 1) {
                (**data_).DecrementRef();
                T *old = first_;
                TypeTrait::Copy(
                        List<T>::SimpleAllocate(new_size, Cap(new_size), nullptr),
                        old,
                        index
                );
                TypeTrait::Copy(first_ + index, old + index + count, old_size - index);
            } else {
                SizeType remain = count;
                for (T *pos = first_ + index; remain > 0; --remain, ++pos) {
                    TypeTrait::Destroy(pos);
                }
                TypeTrait::Move(first_ + index, first_ + index + count, old_size - index - count);
                last_ -= count;
            }
        }
        return *this;
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
    T *SimpleAllocate(const SizeType &size, const SizeType &capacity, RefCount *const &rc);

    /**
     *
     * @param size
     * @param capacity
     * @return
     */
    T *SimpleReallocate(const SizeType &size, const SizeType &capacity);

    /**
     *
     * @param count
     * @return
     */
    T *GrowthAppend(SizeType count);

    /**
     * Reserves \p count of space in the given \p index.
     * The \p index must be valid.
     * @param index
     * @param count
     * @return the address can be inserted elements, or nullptr of failed.
     */
    T *GrowthPrepend(SizeType count);

    /**
     * Reserves \p count of space in the given \p index.
     * The \p index must be valid.
     * @param index
     * @param count
     * @return the address can be inserted elements, or nullptr of failed.
     */
    T *GrowthInsert(SizeType index, SizeType count);

    /**
     *
     * @param count
     * @return
     */
    T *AssignImpl(SizeType count);

    RefCount **data_; // The first bytes of the memory associated to this instance.
    T *first_; // The address of the first element.
    T *last_; // The address of the last element.
    T *end_; // The address of the end of the memory.
};

template<typename T>
T *List<T>::SimpleAllocate(const SizeType &size, const SizeType &capacity, List::RefCount *const &rc) {
    data_ = static_cast<RefCount **>(::malloc(TotCap(capacity)));
    assert(data_);
    *data_ = rc;
    first_ = (T *) (data_ + 1);
    last_ = first_ + size;
    end_ = first_ + capacity;
    return first_;
}

template<typename T>
T *List<T>::SimpleReallocate(const SizeType &size, const SizeType &capacity) {
    SizeType i = sizeof(RefCount *) + capacity * sizeof(T);
    RefCount **old = data_;
    RefCount *old_rc = *data_;
    data_ = reinterpret_cast<RefCount **>(::realloc(data_, TotCap(capacity)));
    assert(data_);
    if (old != data_) {
        *data_ = old_rc;
        first_ = (T *) (data_ + 1);
        last_ = first_ + size;
    }
    end_ = first_ + capacity;
    return first_;
}

template<typename T>
T *List<T>::GrowthAppend(SizeType count) {
    if (count) { // check if insertion can occur.
        if (data_) {
            SizeType old_size = last_ - first_, new_size = old_size + count;
            if (*data_ && (**data_).Value() > 1) {
                (**data_).DecrementRef();
                T *old = first_;
                TypeTrait::Copy(
                        List<T>::SimpleAllocate(new_size, List<T>::Cap(new_size), nullptr),
                        old,
                        old_size
                );
            } else {
                SizeType old_capacity = end_ - first_;
                if (new_size > old_capacity) {
                    List<T>::SimpleReallocate(new_size, Cap(new_size));
                } else {
                    last_ += count;
                }
            }
            return first_ + old_size;
        } else {
            return List<T>::SimpleAllocate(count, Cap(count), nullptr);
        }
    }
    return nullptr;
}

template<typename T>
T *List<T>::GrowthPrepend(SizeType count) {
    if (count) {
        if (data_) {
            SizeType old_size = last_ - first_, new_size = old_size + count;
            if (*data_ && (**data_).Value() > 1) {
                (**data_).DecrementRef();
                T *old = first_;
                TypeTrait::Copy(
                        List<T>::SimpleAllocate(new_size, List<T>::Cap(new_size), nullptr) + count,
                        old,
                        old_size
                );
            } else {
                SizeType old_capacity = end_ - first_;
                if (new_size > old_capacity) {
                    SizeType new_capacity = List<T>::Cap(new_size);
                    if (new_capacity - old_capacity > old_capacity * 2) {
                        T *old = first_;
                        TypeTrait::Copy(
                                List<T>::SimpleAllocate(new_size, new_capacity, *data_) + count,
                                old,
                                old_size
                        );
                    } else {
                        T *pos = List<T>::SimpleReallocate(new_size, new_capacity) + count;
                        TypeTrait::Move(
                                pos + count,
                                first_,
                                old_size
                        );
                    }
                } else {
                    TypeTrait::Move(first_ + count, first_, old_size);
                    last_ += count;
                }
            }
            return first_;
        } else {
            return List<T>::SimpleAllocate(count, Cap(count), nullptr);
        }
    }
    return nullptr;
}

template<typename T>
T *List<T>::GrowthInsert(SizeType index, SizeType count) {
    if (count && data_) { // check if insertion can occur.
        SizeType old_size = last_ - first_; // count the size for verifying and future.
        assert(index < old_size);
        SizeType new_size = old_size + count;
        if (*data_ && (**data_).Value() > 1) {
            (**data_).DecrementRef();
            T *old = first_;
            TypeTrait::Copy(
                    List<T>::SimpleAllocate(new_size, List<T>::Cap(new_size), nullptr),
                    old,
                    index
            ); // Copy separately~
            TypeTrait::Copy(first_ + index + count, old + count, old_size - index);
        } else {
            SizeType old_capacity = end_ - first_;
            if (new_size > old_capacity) {
                SizeType new_capacity = List<T>::Cap(new_size);
                if (new_capacity - old_capacity > old_capacity * 2) {
                    T *old = first_;
                    TypeTrait::Copy(
                            List<T>::SimpleAllocate(new_size, new_capacity, *data_),
                            old,
                            index
                    );
                    TypeTrait::Copy(first_ + index + count, old + index, old_size - index);
                } else {
                    T *pos = List<T>::SimpleReallocate(new_size, new_capacity) + index + count;
                    TypeTrait::Move(
                            pos + index + count,
                            first_ + index,
                            old_size - index
                    );
                }
            } else {
                TypeTrait::Move(first_ + index + count, first_ + index, old_size - index);
                last_ += count;
            }
        }
        return first_ + index;
    }
    return nullptr;
}

template<typename T>
T *List<T>::AssignImpl(SizeType count) {
    if (count) {
        if (data_) {
            if (*data_ && (**data_).Value() > 1) {
                (**data_).DecrementRef();
                return List<T>::SimpleAllocate(count, Cap(count), nullptr);
            } else {
                SizeType old_capacity = end_ - first_;
                for (; last_ != first_; --last_) {
                    TypeTrait::Destroy(last_);
                }
                if (count > old_capacity) {
                    SizeType new_capacity = Cap(count);
                    if (new_capacity - old_capacity > old_capacity * 2) {
                        this->~List();
                        return List<T>::SimpleAllocate(count, new_capacity, nullptr);
                    } else {
                        return List<T>::SimpleReallocate(count, new_capacity);
                    }
                } else {
                    last_ = first_ + count;
                    return first_;
                }
            }
        } else {
            return List<T>::SimpleAllocate(count, Cap(count), nullptr);
        }
    } else {
        Clear();
        return nullptr;
    }
}

#endif //ESCAPIST_LIST_H
