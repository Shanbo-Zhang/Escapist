/**
 * @author Escapist
 * @date January 6th 2023
 * @version 1.0
 * @description
 *
 */

#ifndef ESCAPIST_ARRAYLIST_H
#define ESCAPIST_ARRAYLIST_H

#include "../General.h"
#include "Internal/ReferenceCount.h"
#include "Internal/TypeTrait.h"

/**
 *
 * @tparam T
 */
template<typename T>
class ArrayList {
    static_assert(std::is_nothrow_copy_constructible<T>::value, "T must be copy constructible!");

    using ReferenceCount = EscapistPrivate::ReferenceCount;
    using TypeTrait = typename EscapistPrivate::TypeTraitPatternSelector<T>::TypeTrait;

    /**
     * Head of data body. Composition:\n
     * First 8 bytes points to a pointer of reference count, it can be null.\n
     * Behind bytes store all data, this part will never smaller than [MinimumCapacity]
     */
    ReferenceCount **buf_;

    /**
     * Behind bytes that are mentioned on above comment.
     */
    T *data_;

    /**
     * data size
     */
    SizeType size_;

    /**
     * data capacity, whose value will never smaller than [MinimumCapacity]
     */
    SizeType capacity_;

    static constexpr SizeType MinimumCapacity = (8 * sizeof(T *)) / sizeof(T);
    static constexpr bool EnableMinimumCapacity = ArrayList<T>::MinimumCapacity;

    /**
     * To enable MinimumCapacity, we need to set the capacity based on different circumstances.\n
     * Case 1: 0, we don't need to allocate any thing.\n
     * Case 2: EnableMinimumCapacity and it's smaller than MinimumCapacity: MinimumCapacity.\n
     * Case 3: > MinimumCapacity: dataSize * 1.5.\n \n
     * Why we use MinimumCapacity?\n
     * If we add elements one by one from null, it'll frequently and repetitively copy and paste.
     * To prevent this, we add MinimumCapacity to prevent frequent copy-and-paste.\n
     * @author Escapist
     * @date Jan. 6th 2023
     * @param dataSize data size
     * @return data capacity
     */
    static SizeType CalcCapacity(const SizeType &dataSize) noexcept {
        if (dataSize) {
            if (ArrayList<T>::EnableMinimumCapacity // Check if type is eligible for minimum capacity
                && dataSize < ArrayList<T>::MinimumCapacity) {
                return ArrayList<T>::MinimumCapacity;
            } else {
                return dataSize * 1.5; // Otherwise, the dataSize is too big, so just allocate normally.
            }
        }
        return 0;
    }

    /**
     * the memory compose of a reference count pointer and capacity * sizeof(T)
     * @param dataCapacity current capacity of the data.
     * @return total capacity.
     * @bug On the first debug, I found that I forgot to multiply sizeof(T), lol lol.
     */
    static constexpr SizeType TotalCapacity(const SizeType &dataCapacity) noexcept {
        return sizeof(ReferenceCount *) + dataCapacity * sizeof(T);
    }

    /**
     * Upgrade of ReferenceCount::IncrementRef()\n
     * Case 1: RC has been initialized, just add 1 to stored value.\n
     * Case 2: RC has not been initialized, but we'll share it. So we allocate and initialize it by 2.
     */
    void IncrementRef() {
        assert(buf_);
        if (*buf_) {
            (**buf_).IncrementRef();
        } else {
            *buf_ = (ReferenceCount *) ::malloc(sizeof(ReferenceCount));
            new(*buf_)ReferenceCount(2);
        }
    }

    /**
     * 1. Simple allocate the data by current capacity.\n
     * 2. Assignment of reference count pointer and data pointer.
     * @param ref initial reference count pointer, only applied when we're enlarging buffer.
     */
    void SimpleAllocate(ReferenceCount *const &ref) {
        assert(buf_ = (ReferenceCount **) ::malloc(ArrayList<T>::TotalCapacity(capacity_)));
        // TODO: Why sometimes malloc fails and return nullptr? UIUC CS 233 / CS 340 / CS 341!
        data_ = (T *) (buf_ + 1); // Point the data to one pointer behind the head.
        *buf_ = ref;
    }

    /**
     * 1. Reallocate the data by current capacity.\n
     * 2. Reassign the reference count pointer and data pointer.
     */
    void SimpleReallocate() {
        assert(buf_);
        ReferenceCount **oldBuf = buf_;
        ReferenceCount *oldRef = *buf_;
        assert(buf_ = (ReferenceCount **) ::realloc(buf_, ArrayList<T>::TotalCapacity(capacity_)));
        if (oldBuf != buf_) { // If buffer changed its address, the data pointer still points to old reference count.
            data_ = (T *) (buf_ + 1);
            *buf_ = oldRef;
        }
    }

    /**
     * Grow the indicated size at the end of data.
     * @param growthSize size
     * @return position to add new data.
     */
    T *GrowthAppend(SizeType growthSize) {
        if (growthSize) {
            if (data_) { // Check if we have data before.
                SizeType oldSize = size_; // We might change the value of this member variable, so store it at first!
                size_ += growthSize; // Move out because all 3 cases need to change the size.
                if (*buf_ && (**buf_).GetValue() > 1) { // Case 1: this object is sharing with another object.
                    T *const oldData = data_; // Store old data and detach from old memory.
                    (**buf_).DecrementRef();
                    capacity_ = ArrayList<T>::CalcCapacity(size_);
                    ArrayList<T>::SimpleAllocate(nullptr);
                    TypeTrait::Copy(data_, oldData, oldSize); // Copy from old data finally.
                    // Now, current objects is irrelevant to old shared memory.
                } else {
                    // directly operate in buffer
                    if (size_ > capacity_) { // Case 2: is not sharing, but the capacity is not large enough to
                        capacity_ = ArrayList<T>::CalcCapacity(size_);
                        ArrayList<T>::SimpleReallocate();
                    }
                    // Case 3: the capacity is large enough, we just need to reset the size.
                }
                return data_ + oldSize;
            } else {
                /** @bug 2nd parameters: ArrayList<T>::CalcCapacity(size_), size = 0 actually. */
                new(this)ArrayList<T>(growthSize, ArrayList<T>::CalcCapacity(growthSize));
                return data_;
            }
        }
        return data_ + size_;
    }

    /**
     * Grow the indicated size at the beginning of data, and move existed date to the right.
     * @param growthSize size
     */
    void GrowthPrepend(SizeType growthSize) {
        if (growthSize) {
            if (data_) { // Check if we have data before.
                SizeType oldSize = size_;
                size_ += growthSize;
                if (*buf_ && (**buf_).GetValue() > 1) { // Case 1: this object is sharing with another object.
                    T *const oldData = data_; // Store old data and detach from old memory.
                    (**buf_).DecrementRef();
                    capacity_ = ArrayList<T>::CalcCapacity(size_);
                    ArrayList<T>::SimpleAllocate(nullptr);
                    TypeTrait::Copy(data_ + growthSize, oldData, oldSize); // Copy from old data finally.
                    // For Prepend, we have to reserve spaces for new data.
                } else {
                    // directly operate in buffer
                    if (size_ > capacity_) { // Case 2: is not sharing, but the capacity is not large enough to
                        // Different from Append, in Prepend, if the growthSize is too large, we have to move that after ::realloc.
                        // It'll make it slower.
                        SizeType oldCapacity = capacity_;
                        capacity_ = ArrayList<T>::CalcCapacity(size_);
                        if (capacity_ - oldCapacity > oldCapacity * 2) {
                            // If we grow too large, leftover space might not large enough.
                            // At this time, we simply reallocate and copy it to right place.
                            T *oldData = data_;
                            ArrayList<T>::SimpleAllocate(*buf_);
                            EscapistPrivate::PodTypeTrait<T>::Copy(data_ + growthSize, oldData, oldSize);
                            ::free((void *) buf_);
                            // Because in this case, this object doesn't share with any other objects, we don't need to run the constructor.
                            // But remember to free the old data.
                            // We don't need to free RC pointer because it was assigned to new position in SimpleAllocate
                        } else {
                            ArrayList<T>::SimpleReallocate();
                            TypeTrait::Move(data_ + growthSize, data_, oldSize);
                        }
                    } else {
                        // Case 3: the capacity is large enough, we just need to reset the size.
                        // In Prepend, we need to move the data because the new data will be put in front of previous data.
                        TypeTrait::Move(data_ + growthSize, data_, oldSize);
                    }
                }
            } else {
                new(this)ArrayList<T>(growthSize, ArrayList<T>::CalcCapacity(size_));
            }
        }
    }

    /**
     * Grow the indicated size at the middle of data. Data on the right will be moved.
     * @param growthIndex target index
     * @param growthSize size
     * @return true if data is reserved successfully.
     */
    bool GrowthInsert(SizeType growthIndex, SizeType growthSize) {
        if (growthIndex < size_ && growthSize) {
            if (data_) {
                SizeType oldSize = size_;
                size_ += growthSize;
                if (*buf_ && (**buf_).GetValue() > 1) { // Case 1: this object is sharing with another object.
                    T *const oldData = data_; // Store old data and detach from old memory.
                    (**buf_).DecrementRef();
                    capacity_ = ArrayList<T>::CalcCapacity(size_);
                    ArrayList<T>::SimpleAllocate(nullptr);
                    TypeTrait::Copy(data_, oldData, growthIndex);
                    TypeTrait::Copy(data_ + growthIndex + growthSize, oldData + growthIndex,
                                    oldSize - growthIndex);
                    // For Insert, we have to reserve spaces in middle of data.
                    // Therefore, we need to copy separately.
                } else {
                    // directly operate in buffer
                    if (size_ > capacity_) { // Case 2: is not sharing, but the capacity is not large enough
                        // Similar to Prepend, we need to move after reallocate. So this mechanism is maintained.
                        SizeType oldCapacity = capacity_;
                        capacity_ = ArrayList<T>::CalcCapacity(size_);
                        if (capacity_ - oldCapacity > oldCapacity * 2) {
                            T *oldData = data_;
                            ArrayList<T>::SimpleAllocate(*buf_);
                            EscapistPrivate::PodTypeTrait<T>::Copy(data_, oldData, growthIndex);
                            EscapistPrivate::PodTypeTrait<T>::Copy(data_ + growthIndex + growthSize,
                                                                   oldData + growthIndex, oldSize - growthIndex);
                            ::free((void *) buf_);
                            // Because in this case, this object doesn't share with any other objects, we don't need to run the constructor.
                            // But remember to free the old data.
                            // We don't need to free RC pointer because it was assigned to new position in SimpleAllocate
                        } else {
                            ArrayList<T>::SimpleReallocate();
                            TypeTrait::Move(data_ + growthIndex + growthSize, data_ + growthIndex,
                                            oldSize - growthIndex);
                            // Data before the index stayed static, but after the index should be moved.
                        }
                    } else {
                        // Case 3: the capacity is large enough, we just need to reset the size.
                        // In Insert, we need to move the data just after the index.
                        TypeTrait::Move(data_ + growthIndex + growthSize, data_ + growthIndex, oldSize - growthIndex);
                    }
                }
                return true;
            }
        }
        return false; // In these two cases, we cannot insert.
        // Because: the data is null, or the index is invalid.
    }

    void AssignReset(SizeType newSize) {
        if (newSize) {
            if (data_ && size_) {
                if (*buf_ && (**buf_).GetValue() > 1) {
                    (**buf_).DecrementRef();
                    size_ = newSize;
                    capacity_ = ArrayList<T>::CalcCapacity(size_);
                    ArrayList<T>::SimpleAllocate(nullptr);
                } else {
                    SizeType oldSize = size_;
                    size_ = newSize;
                    if (size_ > capacity_) {
                        SizeType oldCapacity = capacity_;
                        capacity_ = ArrayList<T>::CalcCapacity(size_);
                        if (capacity_ - oldCapacity > oldCapacity * 2) {
                            T *oldData = data_;
                            ArrayList<T>::SimpleAllocate(*buf_);
                        } else {
                            ArrayList<T>::SimpleReallocate();
                        }
                    }
                }
            } else {
                size_ = newSize;
                capacity_ = ArrayList<T>::CalcCapacity(newSize);
                ArrayList<T>::SimpleAllocate(nullptr);
            }
        } else {
            if (buf_ && *buf_ && (**buf_).GetValue() > 1) {
                (**buf_).DecrementRef();
                new(this)ArrayList<T>();
            } else {
                TypeTrait::Destroy(data_, size_);
                size_ = 0;
            }
        }
    }

public:
    ArrayList() noexcept: buf_(nullptr), data_(nullptr), size_(0), capacity_(0) {}

    /**
     * Initialize by indicated size and capacity. capacity cannot smaller than size!
     */
    ArrayList(SizeType size, SizeType capacity)
            : size_(size), capacity_(capacity) {
        assert(size <= capacity);
        if (capacity_) {
            ArrayList<T>::SimpleAllocate(nullptr);
        }
    }

    /**
     * Initialize by an indicated value.
     * @param value indicated value
     * @param count count of value
     * @param offset reserved count before values.
     */
    ArrayList(const T &value, SizeType count = 1, SizeType offset = 0)
            : size_(count + offset), capacity_(ArrayList<T>::CalcCapacity(size_)) {
        if (count) { // Check if we need to allocate data from heap. (Another name: Free Store, CS 128)
            ArrayList<T>::SimpleAllocate(nullptr);
            TypeTrait::Fill(data_ + offset, value, count); // Finally, fill values repetitively.
        } else {
            new(this)ArrayList<T>();
        }
    }

    /**
     * Initialize by data
     * @param data indicated data
     * @param size size of data
     * @param offset reserved count before data.
     */
    ArrayList(const T *data, SizeType size, SizeType offset = 0)
            : size_(size + offset), capacity_(ArrayList<T>::CalcCapacity(size_)) {
        if (data && size) {
            ArrayList<T>::SimpleAllocate(nullptr);
            TypeTrait::Copy(data_ + offset, data, size);
        } else {
            new(this)ArrayList<T>();
        }
    }

    /**
     * Initialize by another object, add reference count.
     * @param other another object
     */
    ArrayList(const ArrayList<T> &other) noexcept
            : buf_(other.buf_), data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        if (buf_ && data_ && size_) {
            ArrayList<T>::IncrementRef();
        } else {
            new(this)ArrayList<T>();
        }
    }

    ArrayList(const ArrayList<T> &other, SizeType size, SizeType offset) noexcept {
        if (other.buf_ && other.data_) {
            if (size && size >= other.size_) {
                new(this)ArrayList<T>(other);
            } else {
                new(this)ArrayList<T>(other.data_ + offset, size);
            }
        } else {
            new(this)ArrayList<T>();
        }
    }

    ArrayList(const ArrayList<T> &other, SizeType size, SizeType dataOffset, SizeType currentOffset) noexcept {
        if (size && other.buf_ && other.data_) {
            if (size == other.size_ && !dataOffset && !currentOffset) {
                new(this)ArrayList<T>(other);
            } else {
                new(this)ArrayList<T>(other.data_ + dataOffset, size, currentOffset);
            }
        } else {
            new(this)ArrayList<T>();
        }
    }

    ~ArrayList() noexcept {
        if (buf_ && data_) {
            if (*buf_) {
                if ((**buf_).GetValue() > 1) {
                    (**buf_).DecrementRef();
                    return;
                } else {
                    ::free((void *) (*buf_));
                }
            }
            TypeTrait::Destroy(data_, size_);
            ::free((void *) buf_);
        }
    }

    SizeType GetSize() const noexcept {
        return data_ ? size_ : 0;
    }

    SizeType GetCapacity() const noexcept {
        return capacity_;
    }

    T *GetData() noexcept {
        if (data_) {
            if (buf_ && *buf_ && (**buf_).GetValue() > 1) { // This object is sharing, detach at first.
                T *oldData = data_;
                (**buf_).DecrementRef();
                capacity_ = ArrayList<T>::CalcCapacity(size_);
                ArrayList<T>::SimpleAllocate(*buf_);
                TypeTrait::Copy(data_, oldData, size_);
            }
            return data_;
        }
        return nullptr;
    }

    const T *GetConstData() const {
        return data_;
    }

    T &GetAt(SizeType index) {
        assert(data_ && index < size_);
        if (buf_ && *buf_ && (**buf_).GetValue() > 1) { // This object is sharing, detach at first.
            T *oldData = data_;
            (**buf_).DecrementRef();
            capacity_ = ArrayList<T>::CalcCapacity(size_);
            ArrayList<T>::SimpleAllocate(*buf_);
            TypeTrait::Copy(data_, oldData, size_);
        }
        return data_[index];
    }

    const T &GetConstAt(SizeType index) const {
        assert(data_ && index < size_);
        return data_[index];
    }

    ArrayList<T> &SetAt(SizeType index, const T &value) {
        assert(data_ && index < size_);
        if (buf_ && *buf_ && (**buf_).GetValue() > 1) { // This object is sharing, detach at first.
            T *oldData = data_;
            (**buf_).DecrementRef();
            capacity_ = ArrayList<T>::CalcCapacity(size_);
            ArrayList<T>::SimpleAllocate(*buf_);
            TypeTrait::Copy(data_, oldData, index);
            TypeTrait::Copy(data_ + index + 1, oldData - index, size_ - index);
        }
        new(data_ + index)T(value);
    }

    bool IsEmpty() const noexcept {
        return data_ && size_;
    }

    bool IsNull() const noexcept {
        return buf_ && capacity_;
    }

    bool IsEmptyOrNull() const noexcept {
        return buf_ && data_ && size_ && capacity_;
    }

    ArrayList<T> &Empty() noexcept {
        if (data_ && size_) {
            if (buf_ && *buf_ && (**buf_).GetValue() > 1) {
                (**buf_).DecrementRef();
                new(this)ArrayList<T>();
            } else {
                TypeTrait::Destroy(data_, size_);
                size_ = 0;
            }
        }
        return *this;
    }

    ArrayList<T> &EnsureCapacity(SizeType capacity) noexcept {
        if (capacity > capacity_) {
            if (buf_ && *buf_ && (**buf_).GetValue() > 1) { // This object is sharing, detach at first.
                T *oldData = data_;
                (**buf_).DecrementRef();
                capacity_ = capacity;
                ArrayList<T>::SimpleAllocate(*buf_);
                TypeTrait::Copy(data_, oldData, size_);
            } else {
                capacity_ = capacity;
                ArrayList<T>::SimpleReallocate();
            }
        }
        return *this;
    }

    ArrayList<T> &Append(const T &value, SizeType count = 1, SizeType offset = 0) noexcept {
        if (count) {
            TypeTrait::Fill(ArrayList<T>::GrowthAppend(offset + count) + offset, value, count);
        }
        return *this;
    }

    ArrayList<T> &Append(const T *data, SizeType size, SizeType offset = 0) noexcept {
        if (data && size) {
            TypeTrait::Copy(ArrayList<T>::GrowthAppend(offset + size) + offset, data, size);
            /** @bug The second parameters are data_ at first, lol. */
        }
        return *this;
    }

    ArrayList<T> &Append(const ArrayList<T> &other) noexcept {
        if (data_) {
            if (other.data_ && other.size_) {
                return ArrayList<T>::Append(other.data_, other.size_, 0);
            }
        } else {
            new(this)ArrayList<T>(other);
        }
        return *this;
    }

    ArrayList<T> &Append(const ArrayList<T> &other, SizeType size, SizeType otherOffset, SizeType currentOffset) {
        if (other.data_ && other.size_) {
            if (size == other.size_ && !otherOffset && !currentOffset) {
                return ArrayList<T>::Append(other);
            } else {
                return ArrayList<T>::Append(other.data_ + otherOffset, size, currentOffset);
            }
        }
        return *this;
    }

    ArrayList<T> &Prepend(const T &value, SizeType count = 1, SizeType offset = 0) noexcept {
        if (count) {
            ArrayList<T>::GrowthPrepend(offset + count);
            TypeTrait::Fill(data_ + offset, value, count);
        }
        return *this;
    }

    ArrayList<T> &Prepend(const T *data, SizeType size, SizeType offset = 0) noexcept {
        if (data && size) {
            ArrayList<T>::GrowthPrepend(offset + size);
            TypeTrait::Copy(data_ + offset, data, size);
        }
        return *this;
    }

    ArrayList<T> &Prepend(const ArrayList<T> &other) noexcept {
        if (data_) {
            if (other.data_ && other.size_) {
                return ArrayList<T>::Prepend(other.data_, other.size_, 0);
            }
        } else {
            new(this)ArrayList<T>(other);
        }
        return *this;
    }

    ArrayList<T> &Prepend(const ArrayList<T> &other, SizeType size,
                          SizeType otherOffset = 0, SizeType currentOffset = 0) {
        if (other.data_ && other.size_) {
            if (size == other.size_ && !otherOffset && !currentOffset) {
                return ArrayList<T>::Prepend(other);
            } else {
                return ArrayList<T>::Prepend(other.data_ + otherOffset, size, currentOffset);
            }
        }
        return *this;
    }

    ArrayList<T> &Insert(SizeType index, const T &value, SizeType count = 1, SizeType offset = 0) noexcept {
        if (count && ArrayList<T>::GrowthInsert(index, offset + count)) {
            TypeTrait::Fill(data_ + index + offset, value, count);
        }
        return *this;
    }

    ArrayList<T> &Insert(SizeType index, const T *data, SizeType size, SizeType offset = 0) noexcept {
        if (data && size && ArrayList<T>::GrowthInsert(index, offset + size)) {
            TypeTrait::Copy(data_ + index + offset, data, size);
        }
        return *this;
    }

    ArrayList<T> &Insert(SizeType index, const ArrayList<T> &other) noexcept {
        if (data_) {
            if (other.data_ && other.size_) {
                return ArrayList<T>::Insert(index, other.data_, other.size_, 0);
            }
        } else {
            new(this)ArrayList<T>(other);
        }
        return *this;
    }

    ArrayList<T> &Insert(SizeType index, const ArrayList<T> &other, SizeType size,
                         SizeType otherOffset = 0, SizeType currentOffset = 0) {
        if (size && other.data_ && other.size_) {
            if (size == other.size_ && !otherOffset && !currentOffset) {
                return ArrayList<T>::Insert(index, other);
            } else {
                return ArrayList<T>::Insert(index, other.data_ + otherOffset, size, currentOffset);
            }
        }
        return *this;
    }

    ArrayList<T> &Delete(SizeType index, SizeType count, bool copyTo = false) noexcept {
        if (index < size_ && count) {
            if (buf_ && *buf_ && (**buf_).GetValue() > 1) {
                T *oldData = data_;
                (**buf_).DecrementRef();
                capacity_ = ArrayList<T>::CalcCapacity(size_);
                ArrayList<T>::SimpleAllocate(*buf_);
                TypeTrait::Copy(data_, oldData, index);
                TypeTrait::Copy(data_ + index, oldData + index + count, size_ - index - count);
            } else {
                TypeTrait::Move(data_ + index, data_ + index + count, size_ - index - count);
            }
            size_ -= count;
        }
        return *this;
    }

    ArrayList<T> &Assign(const T &value, SizeType count, SizeType offset = 0) noexcept {
        ArrayList<T>::AssignReset(count + offset);
        if (count) {
            TypeTrait::Fill(data_ + offset, value, count);
        }
        return *this;
    }

    ArrayList<T> &Assign(const T *data, SizeType size, SizeType offset = 0) noexcept {
        ArrayList<T>::AssignReset(size + offset);
        if (data && size) {
            TypeTrait::Copy(data_ + offset, data, size);
        }
        return *this;
    }

    ArrayList<T> &Assign(const ArrayList<T> &other) noexcept {
        ArrayList<T>::Empty();
        if (other.data_ && other.size_) {
            new(this)ArrayList<T>(other);
        } else {
            new(this)ArrayList<T>();
        }
        return *this;
    }

    ArrayList<T> &Assign(const ArrayList<T> &other, SizeType size,
                         SizeType otherOffset=0, SizeType currentOffset=0) noexcept {
        if (size) {
            if (size == other.size_ && !otherOffset && !currentOffset) {
                return ArrayList<T>::Assign(other);
            } else {
                return ArrayList<T>::Empty();
            }
        } else {
            return ArrayList<T>::Assign(other.data + otherOffset, size, currentOffset);
        }
    }

    ArrayList<T> Left(SizeType count) const noexcept {
        if (count >= size_) {
            return *this;
        } else {
            return ArrayList<T>(data_, count);
        }
    }

    ArrayList<T> Right(SizeType count) const noexcept {
        if (count >= size_) {
            return *this;
        } else {
            return ArrayList<T>(data_ + size_ - count, count);
        }
    }

    ArrayList<T> Middle(SizeType index, SizeType count) const noexcept {
        if (index < size_ && count) {
            return *this;
        } else {
            return ArrayList<T>(data_ + index, count);
        }
    }
};

#endif //ESCAPIST_ARRAYLIST_H
