//
// Created by Escap on 1/6/2023.
//

#ifndef ESCAPIST_ARRAYLIST_H
#define ESCAPIST_ARRAYLIST_H

#include "../General.h"
#include "Internal/ReferenceCount.h"
#include "Internal/TypeTrait.h"

template<typename T>
class ArrayList {
    using ReferenceCount = EscapistPrivate::ReferenceCount;

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

    static constexpr SizeType TotalCapacity(const SizeType &dataCapacity) noexcept {
        return sizeof(ReferenceCount *) + dataCapacity;
    }

    void IncrementRef() {
        assert(buf_);
        if (*buf_) {
            (**buf_).IncrementRef();
        } else {
            *buf_ = (ReferenceCount *) ::malloc(sizeof(ReferenceCount));
            new(*buf_)ReferenceCount(2);
        }
    }

    void SimpleAllocate(const ReferenceCount *&ref) {
        assert(buf_ = (ReferenceCount **) ::malloc(ArrayList<T>::TotalCapacity(capacity_)));
        // TODO: Why sometimes malloc fails and return nullptr? UIUC CS 340 / CS 341!
        data_ = (T *) (buf_ + 1); // Point the data to one pointer behind the head.
        *buf_ = nullptr; // This point to reference count object. We don't share this object.
    }

public:
    ArrayList() noexcept: buf_(nullptr), data_(nullptr), size_(0), capacity_(0) {}

    ArrayList(const T &value, SizeType count, SizeType offset = 0)
            : size_(count + offset), capacity_(ArrayList<T>::CalcCapacity(size_)) {
        if (count) { // Check if we need to allocate data from heap. (Another name: Free Store, CS 128)
            ArrayList<T>::SimpleAllocate();
            TypeTrait<T>::Fill(data_ + offset, value, count); // Finally, fill values repetitively.
        } else {
            new(this)ArrayList<T>();
        }
    }

    ArrayList(const T *data, SizeType size, SizeType offset = 0)
            : size_(size + offset), capacity_(ArrayList<T>::CalcCapacity(size_)) {
        if (data && size) {
            ArrayList<T>::SimpleAllocate();
            TypeTrait<T>::Copy(data_ + offset, data, size);
        } else {
            new(this)ArrayList<T>();
        }
    }

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
            TypeTrait<T>::Destroy(data_, size_);
            ::free((void *) buf_);
        }
    }
};

#endif //ESCAPIST_ARRAYLIST_H
