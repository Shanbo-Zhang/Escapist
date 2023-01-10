//
// Created by Escap on 1/7/2023.
//

#ifndef ESCAPIST_STRING_H
#define ESCAPIST_STRING_H

#include "../General.h"
#include "Internal/ReferenceCount.h"
#include <memory>
#include <cstring>

template<typename Ch>
class CharTrait {
public:
    static inline void Copy(Ch *dest, const Ch *src, SizeType size) {
        assert(dest && src && size);
        for (; size; --size, ++dest, ++src) {
            *dest = *src;
        }
    }

    static inline void Move(Ch *dest, const Ch *src, SizeType size) {
        assert(dest && src && size);
        if (dest <= src || dest >= (src + size)) {
            for (; size; ++dest, ++src, --size)
                *dest = *src;
        } else {
            dest = dest + size - 1;
            src = src + size - 1;
            for (; size; --dest, --src, --size)
                *dest = *src;
        }
    }

    static inline void Fill(Ch *dest, const Char &val, SizeType size) {
        for (; size > 0; ++dest, --size) {
            *dest = val;
        }
    }

    static inline SizeType GetLength(const Ch *src) {
        SizeType len = 0;
        for (; *src; ++src, ++len);
        return len;
    }

    static inline int Compare(const Ch *left, const Ch *right) {
        assert(left && right);
        if (left == right) {
            return 0;
        }
        for (; *left && *right && *left == *right; ++left, ++right);
        return *left - *right;
    }

    static inline int Compare(const Ch *left, const Ch *right, SizeType size) {
        assert(left && right && size);
        for (; *left && *right && *left == *right && size; ++left, ++right, --size);
        return *left - *right;
    }

    static inline int CompareNoCase(const Ch *left, const Ch *right) {
        assert(left && right);
        if (left == right) {
            return 0;
        }
        for (; *left && *right && tolower(*left) == tolower(*right); ++left, ++right);
        return *left - *right;
    }

    static inline int CompareNoCase(const Ch *left, const Ch *right, SizeType size) {
        assert(left && right);
        for (; *left && *right && tolower(*left) == tolower(*right) && size; ++left, ++right, --size);
        return *left - *right;
    }

    static Ch *IndexOf(const Char *data, const Char &ch) {

    }

    static Ch *IndexOf(const Char *data, const Char *target) {

    }

    static Ch *LastIndexOf(const Char *data, const Char &ch) {

    }

    static Ch *LastIndexOf(const Char *data, const Char *target) {

    }

    static const Char *FirstNotOf(const Char *data, const Char target) {
        if (data && target) {
            const Char *dataPos = data;
            while (*dataPos) {
                if (*dataPos != target) {
                    return dataPos;
                }
                ++dataPos;
            }
        }
        return nullptr;
    }

    static const Char *LastNotOf(const Char *data, const Char target) {
        if (data && target) {
            const Char *dataPos = data;
            const Char *lastPos = nullptr;
            while (*dataPos) {
                if (*dataPos != target) {
                    lastPos = dataPos;
                }
                ++dataPos;
            }
            return lastPos;
        }
        return nullptr;
    }

    static void Reverse(const Ch *data) {

    }
};

template<>
class CharTrait<char> {
public:
    static inline void Copy(char *dest, const char *src, SizeType size) {
        ::memcpy((void *) dest, (const void *) src, size);
    }

    static inline void Move(char *dest, const char *src, SizeType size) {
        ::memmove((void *) dest, (const void *) src, size);
    }

    static inline void Fill(char *dest, const char &ch, SizeType count) {
        ::memset((void *) dest, ch, count);
    }

    static inline int Compare(const char *left, const char *right) {
        return ::strcmp(left, right);
    }

    static inline int CompareN(const char *left, const char *right, SizeType size) {
        return ::strncmp(left, right, size);
    }

    static inline int CompareNoCase(const char *left, const char *right) {
        return ::_stricmp(left, right);
    }

    static inline SizeType GetLength(const char *src) {
        return ::strlen(src);
    }

    static inline char *IndexOf(const char *data, const char &ch) {
        return ::strchr(data, ch);
    }

    static inline char *IndexOf(const char *data, const char *target) {
        return ::strstr(data, target);
    }

    static inline char *LastIndexOf(const char *data, const char &ch) {
        return ::strrchr(data, ch);
    }

    static inline char *LastIndexOf(const char *data, const char *target) {
        if (data && target) {
            const char *dataPos = data;
            const char *targetPos = target;
            const char *currentPos = nullptr;
            const char *lastPos = nullptr;
            while (*dataPos) {
                currentPos = dataPos;
                while (*dataPos && *targetPos && *dataPos == *targetPos) {
                    ++dataPos;
                    ++targetPos;
                }
                if (!(*targetPos)) {
                    lastPos = currentPos;
                }
                targetPos = target;
                ++dataPos;
            }
            return const_cast<char *>(lastPos);
        }
        return nullptr;
    }

    static inline const char *FirstNotOf(const char *data, const char target) {
        if (data && target) {
            const char *dataPos = data;
            while (*dataPos) {
                if (*dataPos != target) {
                    return dataPos;
                }
                ++dataPos;
            }
        }
        return nullptr;
    }

    static inline const char *LastNotOf(const char *data, const char target) {
        if (data && target) {
            const char *dataPos = data;
            const char *lastPos = nullptr;
            while (*dataPos) {
                if (*dataPos != target) {
                    lastPos = dataPos;
                }
                ++dataPos;
            }
            return lastPos;
        }
        return nullptr;
    }

    static void Reverse(char *data) {
        ::strrev(data);
    }
};

template<>
class CharTrait<wchar_t> {
public:
    static inline void Copy(wchar_t *dest, const wchar_t *src, SizeType size) {
        ::memcpy((void *) dest, (const void *) src, size * sizeof(wchar_t));
    }

    static inline void Move(wchar_t *dest, const wchar_t *src, SizeType size) {
        ::memmove((void *) dest, (const void *) src, size * sizeof(wchar_t));
    }

    static inline void Fill(wchar_t *dest, const wchar_t &ch, SizeType count) {
        for (; count > 0; ++dest, --count) {
            *dest = ch;
        }
    }

    static inline int Compare(const wchar_t *left, const wchar_t *right) {
        return ::wcscmp(left, right);
    }

    static inline int CompareN(const wchar_t *left, const wchar_t *right, SizeType size) {
        return ::wcsncmp(left, right, size);
    }

    static inline int CompareNoCase(const wchar_t *left, const wchar_t *right) {
        return ::_wcsicmp(left, right);
    }

    static inline SizeType GetLength(const wchar_t *src) {
        return ::wcslen(src);
    }

    static inline wchar_t *IndexOf(const wchar_t *data, const wchar_t &ch) {
        return ::wcschr(data, ch);
    }

    static inline wchar_t *IndexOf(const wchar_t *data, const wchar_t *target) {
        return ::wcsstr(data, target);
    }

    static inline wchar_t *LastIndexOf(const wchar_t *data, const wchar_t &ch) {
        return ::wcsrchr(data, ch);
    }

    static inline wchar_t *LastIndexOf(const wchar_t *data, const wchar_t *target) {
        if (data && target) {
            const wchar_t *dataPos = data;
            const wchar_t *targetPos = target;
            const wchar_t *currentPos = nullptr;
            const wchar_t *lastPos = nullptr;
            while (*dataPos) {
                currentPos = dataPos;
                while (*dataPos && *targetPos && *dataPos == *targetPos) {
                    ++dataPos;
                    ++targetPos;
                }
                if (!(*targetPos)) {
                    lastPos = currentPos;
                }
                targetPos = target;
                ++dataPos;
            }
            return const_cast<wchar_t *>(lastPos);
        }
        return nullptr;
    }

    static inline const wchar_t *FirstNotOf(const wchar_t *data, const wchar_t target) {
        if (data && target) {
            const wchar_t *dataPos = data;
            while (*dataPos) {
                if (*dataPos != target) {
                    return dataPos;
                }
                ++dataPos;
            }
        }
        return nullptr;
    }

    static inline const wchar_t *LastNotOf(const wchar_t *data, const wchar_t target) {
        if (data && target) {
            const wchar_t *dataPos = data;
            const wchar_t *lastPos = nullptr;
            while (*dataPos) {
                if (*dataPos != target) {
                    lastPos = dataPos;
                }
                ++dataPos;
            }
            return lastPos;
        }
        return nullptr;
    }

    static void Reverse(wchar_t *data) {
        ::wcsrev(data);
    }
};

// TODO: How to support UTF8, UTF16, etc. encoding in common string class.

template<typename Ch>
class BasicString {
private:
    using ReferenceCount = EscapistPrivate::ReferenceCount;

    struct GeneralBuffer {
        ReferenceCount **buf_;
        Ch *str_;
        SizeType len_;
        SizeType capacity_;
    };

    static constexpr SizeType SmallStringCapacity = sizeof(GeneralBuffer) / sizeof(Ch);
    static constexpr SizeType SmallStringLengthIndex = BasicString<Ch>::SmallStringCapacity - 1;

    enum class StringMode {
        Null = 0x00000000,
        DirectCopy = 0x00000001,
        SmallString = 0x00000002,
        UseGeneralBuffer = 0x00000004,
        NeedAllocate = 0x00000008,
        EagerCopy = 0x00000010 | NeedAllocate | UseGeneralBuffer,
        CopyOnWrite = 0x00000020 | NeedAllocate | UseGeneralBuffer,
    } mode_;

    union {
        unsigned char bytes_[sizeof(GeneralBuffer)];
        Ch sso_[BasicString<Ch>::SmallStringCapacity];
        GeneralBuffer buf_;
    };

    static constexpr SizeType TotalCapacity(SizeType strCapacity) noexcept {
        return sizeof(ReferenceCount *) + strCapacity * sizeof(Ch);
    }

    SizeType GetSmallLength() const {
        assert(mode_ == StringMode::SmallString);
        return SizeType(BasicString<Ch>::SmallStringCapacity - sso_[BasicString<Ch>::SmallStringLengthIndex]);
    }

    void SetSmallLength(SizeType length, bool putZero) noexcept {
        if (mode_ != StringMode::SmallString) { // When it switches from another mode.
            mode_ = StringMode::SmallString;
        }
        sso_[BasicString<Ch>::SmallStringLengthIndex] = Ch(BasicString<Ch>::SmallStringCapacity - length);
        if (putZero
            && length != BasicString<Ch>::SmallStringLengthIndex) {
            // If the length is Length Index, we don't need to do an extra assignment because the last is zero.
            sso_[length] = Ch(0);
        }
    }

    void InitEager(SizeType length, bool putZero, ReferenceCount *const ref = nullptr) noexcept {
        if (mode_ != StringMode::NeedAllocate) { // When it switches from another mode.
            mode_ = StringMode::NeedAllocate;
        }
        buf_.len_ = length; // Assignment
        buf_.capacity_ = length * (long double) 1.5; // Narrowing conversion from 'SizeType'?
        assert(buf_.buf_ = (ReferenceCount **) ::malloc(BasicString<Ch>::TotalCapacity(buf_.capacity_)));
        *buf_.buf_ = ref; // Assign the reference count pointer.
        buf_.str_ = (Ch *) (buf_.buf_ + 1);
        if (putZero) {
            buf_.str_[buf_.len_] = Ch(0);
        }
    }

    /**
     * Initialize the string based on different length.\n
     * Different length means different mode.
     * @date January 8th 2023
     * @param length total length (includes frontOffset and backOffset)
     * @param putZero string is null-terminated
     * @return pointer that will be wrote, based on different mode.
     */
    Ch *Initialize(SizeType length, bool putZero) noexcept {
        if (length) { // We need to initialize.
            if (length < BasicString<Ch>::SmallStringCapacity) { // it can use Small String mode.
                BasicString<Ch>::SetSmallLength(length, putZero);
                return sso_;
            } else {
                BasicString<Ch>::InitEager(length, putZero);
                return buf_.str_;
            }
        } else {
            new(this)BasicString<Ch>(); // string is null.
            return nullptr;
        }
    }

    Ch *GrowthAppend(SizeType growthLength) {
        if (growthLength) {
            switch (mode_) {
                case StringMode::Null: // just initialize by indicated size.
                    return BasicString<Ch>::Initialize(growthLength, true);
                case StringMode::SmallString: {
                    SizeType oldLen = BasicString<Ch>::GetSmallLength(); // Store the old length.
                    SizeType newLen = oldLen + growthLength; // Calculate new length~
                    if (newLen < BasicString<Ch>::SmallStringCapacity) {
                        // If the new length are still large enough, keep in this mode.
                        BasicString<Ch>::SetSmallLength(newLen, true); // just set new length.
                        return sso_ + oldLen; // Append method will operate something in sso pointer
                    } else { // We need to switch to NeedAllocate mode because it's too large to accommodate
                        Ch oldStr[BasicString<Ch>::SmallStringCapacity]; // sso is in union, so we have to copy out from union
                        CharTrait<Ch>::Copy(oldStr, sso_, oldLen);
                        BasicString<Ch>::InitEager(newLen, true); // reinitialize and copy old data
                        CharTrait<Ch>::Copy(buf_.str_, oldStr, oldLen);
                        return buf_.str_ + oldLen; // we'll operate in heap.
                    }
                    assert(false); // never reach ~~~~~
                }
                case StringMode::NeedAllocate: {
                    if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                        Ch *const oldStr = buf_.str_; // Store old data pointer for copy
                        SizeType oldLen = buf_.len_; // Store old length because we'll assign a new value~
                        (**buf_.buf_).DecrementRef();
                        Ch *newStr = BasicString<Ch>::Initialize(oldLen + growthLength, true);
                        // Because string will be initialized in different mode based on new length, we have to store it before copy.
                        CharTrait<Ch>::Copy(newStr, oldStr, oldLen);
                        return newStr + oldLen;
                    } else {
                        SizeType oldLen = buf_.len_; // store it at first for copy.
                        buf_.len_ += growthLength;
                        if (buf_.len_ >= buf_.capacity_) { // Capacity isn't large enough, so enlarge
                            ReferenceCount **oldBuf = buf_.buf_;
                            ReferenceCount *oldRef = *buf_.buf_;
                            buf_.capacity_ = buf_.len_ * (long double) 1.5;
                            buf_.buf_ = (ReferenceCount **) ::realloc(buf_.buf_,
                                                                      BasicString<Ch>::TotalCapacity(buf_.capacity_));
                            if (buf_.buf_ != oldBuf) {
                                // If the address changed, data pointer and reference count pointer still point to old address.
                                *buf_.buf_ = oldRef;
                                buf_.str_ = (Ch *) (buf_.buf_ + 1);
                            }
                        }
                        // We don't need to do anything if the capacity is large enough.
                        buf_.str_[buf_.len_] = Ch(0); // Set 0 for new length
                        return buf_.str_ + oldLen;
                    }
                }
                case StringMode::DirectCopy: {
                    Ch *const oldStr = buf_.str_;
                    SizeType oldLen = buf_.len_;
                    Ch *newStr = BasicString<Ch>::Initialize(buf_.len_ + growthLength, true);
                    CharTrait<Ch>::Copy(newStr, oldStr, oldLen);
                    return newStr + oldLen;
                }
                default:
                    assert(false);
            }
        } else {
            return BasicString<Ch>::GetData() + BasicString<Ch>::GetLength();
        }
        return nullptr;
    }

    Ch *GrowthPrepend(SizeType growthLength) {
        if (growthLength) {
            switch (mode_) {
                case StringMode::Null: // just initialize by indicated size.
                    return BasicString<Ch>::Initialize(growthLength, true);
                case StringMode::SmallString: {
                    SizeType oldLen = BasicString<Ch>::GetSmallLength();
                    SizeType newLen = oldLen + growthLength;
                    if (newLen < BasicString<Ch>::SmallStringCapacity) {
                        // If the new length are still large enough, keep in this mode.
                        BasicString<Ch>::SetSmallLength(newLen, true);
                        CharTrait<Ch>::Move(sso_ + growthLength, sso_, oldLen);
                        // Because we need to prepend, so we need to reserve data in front.
                        return sso_;
                    } else { // We need to switch to NeedAllocate mode because it's too large to accommodate
                        Ch oldStr[BasicString<Ch>::SmallStringCapacity]; // sso is in union, so we have to copy out from union
                        CharTrait<Ch>::Copy(oldStr, sso_, oldLen);
                        BasicString<Ch>::InitEager(newLen, true); // reinitialize and copy old data
                        CharTrait<Ch>::Copy(buf_.str_ + growthLength, oldStr, oldLen); // ** + growthLength
                        return buf_.str_; // we'll operate in heap.
                    }
                    assert(false); // never reach ~~~~~
                }
                case StringMode::NeedAllocate: {
                    if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                        Ch *const oldStr = buf_.str_; // Store old data pointer for copy
                        SizeType oldLen = buf_.len_; // Store old length because we'll assign a new value~
                        (**buf_.buf_).DecrementRef();
                        Ch *newStr = BasicString<Ch>::Initialize(oldLen + growthLength, true);
                        // Because string will be initialized in different mode based on new length, we have to store it before copy.
                        CharTrait<Ch>::Copy(newStr + growthLength, oldStr, oldLen); // ** growthLength
                        return newStr;
                    } else {
                        SizeType oldLen = buf_.len_; // store it at first for copy.
                        buf_.len_ += growthLength;
                        if (buf_.len_ >= buf_.capacity_) { // Capacity isn't large enough, so enlarge
                            // Different from append, we need to move data.
                            // If we realloc and move, it'll case two move.
                            // Therefore, if difference between those two capacity, it tend to allocate new data and copy it.
                            ReferenceCount **oldBuf = buf_.buf_;
                            SizeType oldCapacity = buf_.capacity_;
                            buf_.capacity_ = buf_.len_ * (long double) 1.5;
                            if (buf_.capacity_ - oldCapacity > oldCapacity * 2) {
                                Ch *const oldStr = buf_.str_;
                                BasicString<Ch>::InitEager(buf_.len_, true);
                                CharTrait<Ch>::Copy(buf_.str_ + growthLength, oldStr, oldLen);
                                ::free((void *) oldBuf);
                            } else {
                                ReferenceCount *oldRef = *buf_.buf_;
                                buf_.buf_ = (ReferenceCount **) ::realloc(buf_.buf_,
                                                                          BasicString<Ch>::TotalCapacity(
                                                                                  buf_.capacity_));
                                if (buf_.buf_ != oldBuf) {
                                    *buf_.buf_ = oldRef;
                                    buf_.str_ = (Ch *) (buf_.buf_ + 1);
                                }
                                CharTrait<Ch>::Move(buf_.str_ + growthLength, buf_.str_, oldLen);
                            }
                        } else { // Different from append, we need to move existed data although buffer is large enough
                            CharTrait<Ch>::Move(buf_.str_ + growthLength, buf_.str_, oldLen);
                        }
                        // We don't need to do anything if the capacity is large enough.
                        buf_.str_[buf_.len_] = Ch(0); // Set 0 for new length
                        return buf_.str_;
                    }
                }
                case StringMode::DirectCopy: {
                    Ch *const oldStr = buf_.str_;
                    SizeType oldLen = buf_.len_;
                    Ch *newStr = BasicString<Ch>::Initialize(buf_.len_ + growthLength, true);
                    CharTrait<Ch>::Copy(newStr + growthLength, oldStr, oldLen);
                    return newStr;
                }
                default:
                    assert(false);
            }
        } else {
            return BasicString<Ch>::GetData();
        }
        return nullptr;
    }

    Ch *GrowthInsert(SizeType growthIndex, SizeType growthLength) {
        if (growthLength) {
            switch (mode_) {
                case StringMode::Null: // Invalid! We don't know where it'll insert~
                    return nullptr;
                case StringMode::SmallString: {
                    SizeType oldLen = BasicString<Ch>::GetSmallLength();
                    SizeType newLen = oldLen + growthLength;
                    if (newLen < BasicString<Ch>::SmallStringCapacity) {
                        // If the new length are still large enough, keep in this mode.
                        BasicString<Ch>::SetSmallLength(newLen, true);
                        CharTrait<Ch>::Move(sso_ + growthIndex + growthLength, sso_ + growthIndex,
                                            oldLen - growthIndex);
                        // We just need to move string behind growthIndex.
                        return sso_ + growthIndex;
                    } else {
                        Ch oldStr[BasicString<Ch>::SmallStringCapacity]; // sso is in union, so we have to copy out from union
                        CharTrait<Ch>::Copy(oldStr, sso_, oldLen);
                        BasicString<Ch>::InitEager(newLen, true);
                        CharTrait<Ch>::Copy(buf_.str_, oldStr, growthIndex);
                        CharTrait<Ch>::Copy(buf_.str_ + growthIndex + growthLength, oldStr + growthIndex,
                                            oldLen - growthIndex);
                        return buf_.str_ + growthIndex; // we'll operate in heap.
                    }
                    assert(false); // never reach ~~~~~
                }
                case StringMode::NeedAllocate: {
                    if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                        Ch *const oldStr = buf_.str_;
                        SizeType oldLen = buf_.len_;
                        (**buf_.buf_).DecrementRef();
                        Ch *newStr = BasicString<Ch>::Initialize(oldLen + growthLength, true);
                        // Because string will be initialized in different mode based on new length, we have to store it before copy.
                        CharTrait<Ch>::Copy(newStr, oldStr, growthIndex);
                        CharTrait<Ch>::Copy(newStr + growthIndex + growthLength, oldStr + growthIndex,
                                            oldLen - growthIndex);
                        return newStr + growthIndex;
                    } else {
                        SizeType oldLen = buf_.len_; // store it at first for copy.
                        buf_.len_ += growthLength;
                        if (buf_.len_ >= buf_.capacity_) { // Capacity isn't large enough, so enlarge
                            // Different from append, we need to move data.
                            // If we realloc and move, it'll case two move.
                            // Therefore, if difference between those two capacity, it tend to allocate new data and copy it.
                            ReferenceCount **oldBuf = buf_.buf_;
                            SizeType oldCapacity = buf_.capacity_;
                            buf_.capacity_ = buf_.len_ * (long double) 1.5;
                            if (buf_.capacity_ - oldCapacity > oldCapacity * 2) {
                                Ch *const oldStr = buf_.str_;
                                BasicString<Ch>::InitEager(buf_.len_, true);
                                CharTrait<Ch>::Copy(buf_.str_, oldStr, growthIndex);
                                CharTrait<Ch>::Copy(buf_.str_ + growthIndex + growthLength, oldStr + growthIndex,
                                                    oldLen - growthIndex);
                                ::free((void *) oldBuf);
                            } else {
                                ReferenceCount *oldRef = *buf_.buf_;
                                buf_.buf_ = (ReferenceCount **) ::realloc(buf_.buf_,
                                                                          BasicString<Ch>::TotalCapacity(
                                                                                  buf_.capacity_));
                                if (buf_.buf_ != oldBuf) {
                                    *buf_.buf_ = oldRef;
                                    buf_.str_ = (Ch *) (buf_.buf_ + 1);
                                }
                                CharTrait<Ch>::Move(buf_.str_ + growthIndex + growthLength, buf_.str_ + growthIndex,
                                                    oldLen - growthIndex);
                            }
                        } else { // Different from append, we need to move existed data although buffer is large enough
                            CharTrait<Ch>::Move(buf_.str_ + growthIndex + growthLength, buf_.str_ + growthIndex,
                                                oldLen - growthIndex);
                        }
                        // We don't need to do anything if the capacity is large enough.
                        buf_.str_[buf_.len_] = Ch(0); // Set 0 for new length
                        return buf_.str_ + growthIndex;
                    }
                }
                case StringMode::DirectCopy: {
                    Ch *const oldStr = buf_.str_;
                    SizeType oldLen = buf_.len_;
                    Ch *newStr = BasicString<Ch>::Initialize(buf_.len_ + growthLength, true);
                    CharTrait<Ch>::Copy(newStr, oldStr, growthIndex);
                    CharTrait<Ch>::Copy(newStr + growthIndex + growthLength, oldStr + growthIndex,
                                        oldLen - growthIndex);
                    return newStr;
                }
                default:
                    assert(false);
            }
        } else {
            return nullptr;
        }
        return nullptr;
    }

public:
    /**
     * Default Constructor, the string is null.
     * @date January 8th 2023
     */
    BasicString() noexcept: mode_(StringMode::Null) {}

    /**
     * Initialize the string by an indicated character and count.\n
     * If the ch or count is 0, it'll still be null.
     * @date January 8th 2023
     * @param ch the indicated character
     * @param count count of assignment
     * @param frontOffset reserved space before assignment
     * @param backOffset reserved space behind assignment
     */
    explicit BasicString(const Ch &ch, SizeType count = 1, SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (ch && count) { // Check if we need to use the memory.
            CharTrait<Ch>::Fill(
                    BasicString<Ch>::Initialize(frontOffset + count + backOffset, true) + frontOffset,
                    ch,
                    count
            );
        } else {
            new(this)BasicString<Ch>();
        }
    }

    /**
     * Initialize the string based on a valid string and a valid length.
     * If the str is nullptr or length is 0, it'll still be null.
     * @date January 8th 2023
     * @param str existed string
     * @param length length of string
     * @param frontOffset reserved space before assignment
     * @param backOffset reserved space behind assignment
     */
    BasicString(const Ch *str, SizeType length, SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (str && length) { // Check if we need to use the memory.
            CharTrait<Ch>::Copy(
                    BasicString<Ch>::Initialize(frontOffset + length + backOffset, true) + frontOffset,
                    str,
                    length
            );
        } else {
            new(this)BasicString<Ch>();
        }
    }

    /**
     * Initialize string based on null-terminated string.\n
     * @param str null-terminated string.
     */
    explicit BasicString(const Ch *str) noexcept
            : BasicString(str, CharTrait<Ch>::GetLength(str), 0, 0) {}

    /**
     * Copy constructor, initialize this string by another string object.\n
     * Null String: initialize this object as null, too.\n
     * Small String: Copy-and paste, because it's small.\n
     * Else: if it doesn't have reference count, then create and initialize it.\n
     * If it does have, plus one.
     * @date January 8th 2023
     * @param other another string object
     */
    BasicString(const BasicString<Ch> &other) noexcept: mode_(other.mode_), buf_(other.buf_) { // Copy at first
        if (mode_ == StringMode::NeedAllocate) { // we need to change something in this mode.
            if (*buf_.buf_) { // We copy it directly, but we need to increase the reference count if it has.
                (**buf_.buf_).IncrementRef();
            } else { // If it doesn't have, we need to create and initialize it by 2
                assert(*buf_.buf_ = (ReferenceCount *) ::malloc(sizeof(ReferenceCount)));
                new(*buf_.buf_)ReferenceCount(2);
            }
        }
    }

    BasicString(BasicString<Ch> &&other) noexcept: mode_(other.mode_), buf_(other.buf_) {
        ::memset(&other, 0, sizeof(BasicString<Ch>));
    }

    /**
     * Initialize constructor by another object.
     * @date January 8th 2023
     * @param other another string object
     * @param length indicated length of string
     * @param otherFrontOffset Because we cannot operate this like a pointer, this is to show how many characters we ignore in front of data.
     * @param currentFrontOffset reserved space before assignment
     * @param currentBackOffset reserved space behind assignment
     */
    BasicString(const BasicString<Ch> &other, SizeType length, SizeType, SizeType otherFrontOffset = 0,
                SizeType currentFrontOffset = 0, SizeType currentBackOffset = 0) noexcept {
        if (!currentFrontOffset && !currentBackOffset && !otherFrontOffset && length == other.GetLength()) {
            new(this)BasicString<Ch>(other);
        } else {
            new(this)BasicString<Ch>(other.GetConstData() + otherFrontOffset, length,
                                     currentFrontOffset, currentBackOffset);
        }
    }

    ~BasicString() noexcept {
        if (mode_ == StringMode::NeedAllocate) {
            if (*buf_.buf_) {
                if ((**buf_.buf_).GetValue() > 1) {
                    (**buf_.buf_).DecrementRef();
                    return;
                } else {
                    ::free((void *) *buf_.buf_);
                }
            }
            ::free((void *) buf_.buf_);
        }
    }

    /**
     * @date January 8th 2023
     * @return current length of string
     */
    SizeType GetLength() const {
        switch (mode_) {
            case StringMode::Null:
                return 0;
            case StringMode::SmallString:
                return BasicString<Ch>::GetSmallLength();
            case StringMode::NeedAllocate:
            case StringMode::DirectCopy:
                return buf_.len_;
            default:
                assert(false);
        }
    }

    /**
     * @date January 8th 2023
     * @return Current length of string\n
     * Tips: If this string is sharing with another string, then it'll return 0.
     */
    SizeType GetCapacity() const {
        switch (mode_) {
            case StringMode::Null:
            case StringMode::DirectCopy:
                return 0;
            case StringMode::SmallString:
                return BasicString<Ch>::SmallStringCapacity;
            case StringMode::NeedAllocate:
                return (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) ? 0 : buf_.capacity_;
            default:
                assert(false);
        }
    }

    /**
     * @date January 8th 2023
     * @return true if the string is empty (no content)
     */
    bool IsEmpty() const noexcept {
        switch (mode_) {
            case StringMode::Null:
                return true;
            case StringMode::SmallString:
                return !BasicString<Ch>::GetSmallLength();
            case StringMode::NeedAllocate:
            case StringMode::DirectCopy:
                return !buf_.len_;
            default:
                assert(false);
        }
    }

    /**
     * @date January 8th 2023
     * @return true if the object doesn't point to anything.
     */
    bool IsNull() const {
        switch (mode_) {
            case StringMode::Null:
                return true;
            case StringMode::SmallString:
                return false;
            case StringMode::NeedAllocate:
                return !(buf_.str_ && buf_.capacity_);
            case StringMode::DirectCopy:
                return !buf_.str_;
            default:
                assert(false);
        }
    }

    /**
     * @date January 8th 2023
     * @return true if empty or null.
     */
    bool IsEmptyOrNull() const {
        switch (mode_) {
            case StringMode::Null:
                return true;
            case StringMode::SmallString:
                return !BasicString<Ch>::GetSmallLength();
            case StringMode::NeedAllocate:
                return !(buf_.str_ && buf_.capacity_) || !buf_.len_;
            default:
                assert(false);
        }
    }

    Ch &GetAt(SizeType index) {
        switch (mode_) {
            case StringMode::SmallString:
                assert(index < BasicString<Ch>::SmallStringLengthIndex);
                return sso_[index];
            case StringMode::NeedAllocate:
                assert(index < buf_.len_);
                if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                    Ch *oldData = buf_.str_;
                    (**buf_.buf_).DecrementRef();
                    CharTrait<Ch>::Copy(
                            BasicString<Ch>::Initialize(buf_.len_, true),
                            oldData,
                            buf_.len_
                    );
                }
                return buf_.str_[index];
            case StringMode::DirectCopy:
                assert(index < buf_.len_);
                return buf_.str_[index];
            default:
                assert(false);
        }
    }

    const Ch &GetConstAt(SizeType index) const {
        switch (mode_) {
            case StringMode::SmallString:
                assert(index < BasicString<Ch>::SmallStringLengthIndex);
                return sso_[index];
            case StringMode::NeedAllocate:
            case StringMode::DirectCopy:
                assert(index < buf_.len_);
                return buf_.str_[index];
            default:
                assert(false);
        }
    }

    /**
     * return the string content pointer\n
     * Different from GetConstData, if it's sharing with another string, it'll detach from shared object.
     * @date January 8th 2023
     * @return string pointer
     */
    Ch *GetData() {
        switch (mode_) {
            case StringMode::Null:
                return nullptr;
            case StringMode::SmallString:
                return sso_;
            case StringMode::NeedAllocate:
                if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                    Ch *oldData = buf_.str_;
                    (**buf_.buf_).DecrementRef();
                    CharTrait<Ch>::Copy(
                            BasicString<Ch>::Initialize(buf_.len_, true),
                            oldData,
                            buf_.len_
                    );
                }
                return buf_.str_;
            case StringMode::DirectCopy:
                new(this)BasicString<Ch>(buf_.str_, buf_.len_, 0, 0);
                return buf_.str_;
            default:
                assert(false);
        }
        return nullptr;
    }

    /**
     * @date January 8th 2023
     * @return string's const pointer
     */
    const Ch *GetConstData() const {
        switch (mode_) {
            case StringMode::Null:
                return nullptr;
            case StringMode::SmallString:
                return sso_;
            case StringMode::NeedAllocate:
            case StringMode::DirectCopy:
                return buf_.str_;
            default:
                break;
        }
        assert(false);
    }

    SizeType IndexOf(const Ch &ch, SizeType from = 0) const noexcept {
        if (const Ch *curr = BasicString<Ch>::GetConstData()) {
            Ch *target = CharTrait<Ch>::IndexOf(curr + from, ch);
            return target ? target - curr : -1;
        }
        return -1;
    }

    SizeType IndexOf(const Ch *str, SizeType from = 0) const noexcept {
        if (const Ch *curr = BasicString<Ch>::GetConstData()) {
            Ch *target = CharTrait<Ch>::IndexOf(curr + from, str);
            return target ? target - curr : -1;
        }
        return -1;
    }

    SizeType IndexOf(const BasicString<Ch> &other, SizeType from = 0) const noexcept {
        return BasicString<Ch>::IndexOf(other.GetConstData(), from);
    }

    SizeType LastIndexOf(const Ch &ch, SizeType from = 0) const noexcept {
        if (const Ch *curr = BasicString<Ch>::GetConstData()) {
            Ch *target = CharTrait<Ch>::LastIndexOf(curr + from, ch);
            return target ? target - curr : -1;
        }
        return -1;
    }

    SizeType LastIndexOf(const Ch *str, SizeType from = 0) const noexcept {
        if (const Ch *curr = BasicString<Ch>::GetConstData()) {
            Ch *target = CharTrait<Ch>::LastIndexOf(curr + from, str);
            return target ? target - curr : -1;
        }
        return -1;
    }

    SizeType LastIndexOf(const BasicString<Ch> &other, SizeType from = 0) const noexcept {
        return BasicString<Ch>::LastIndexOf(other.GetConstData(), from);
    }

    /**
     * Add a character count of times at the end of string.\n
     * If the buffer is not large enough, it'll automatically enlarge and reinitialize.
     * @date January 8th 2023
     * @param ch indicated character
     * @param count count
     * @param frontOffset reserved data before added characters.
     * @param backOffset reserved data behind added characters.
     * @return itself
     */
    BasicString<Ch> &Append(const Ch &ch, SizeType count = 1,
                            SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (ch && count) {
            if (Ch *pos = BasicString<Ch>::GrowthAppend(frontOffset + count + backOffset)) {
                CharTrait<Ch>::Fill(pos + frontOffset, ch, count);
            }
        }
        return *this;
    }

    /**
     * Add a string by the indicated length at the end of string.\n
     * If the buffer is not large enough, it'll automatically enlarge and reinitialize.
     * @date January 8th 2023
     * @param str indicated string
     * @param length length of new string.
     * @param frontOffset reserved data before added characters.
     * @param backOffset reserved data behind added characters.
     * @return itself
     */
    BasicString<Ch> &Append(const Ch *str, SizeType length,
                            SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (str && length) {
            if (Ch *pos = BasicString<Ch>::GrowthAppend(frontOffset + length + backOffset)) {
                CharTrait<Ch>::Copy(pos + frontOffset, str, length);
            }
        }
        return *this;
    }

    /**
     * Add a string by the indicated length at the end of string.\n
     * Its length is decided by position of zero.
     * @param str indicated string
     * @return itself
     */
    BasicString<Ch> &Append(const Ch *str) noexcept {
        return BasicString<Ch>::Append(str, CharTrait<Ch>::GetLength(str), 0, 0);
    }

    /**
     * Add a string object at the end of string.\n
     * Its length is decided by its member variable.
     * @param other another object (cannot be itself)
     * @return itself
     */
    BasicString<Ch> &Append(const BasicString<Ch> &other) noexcept {
        if (mode_ == StringMode::Null) {
            new(this)BasicString<Ch>(other);
        } else {
            return BasicString<Ch>::Append(other.GetConstData(), other.GetLength());
        }
        return *this;
    }

    /**
     * Add a string object by indicated length at the end of string.\n
     * If the length is larger than its real length, it'll be detected.
     * @param other another object
     * @param length indicated length
     * @param otherOffset characters that'll be skipped behind the head of data.
     * @param currentFrontOffset reserved data before added characters.
     * @param currentBackOffset reserved data behind added characters.
     * @return
     */
    BasicString<Ch> &Append(const BasicString<Ch> &other, SizeType length, SizeType otherOffset = 0,
                            SizeType currentFrontOffset = 0, SizeType currentBackOffset = 0) {
        if (length == other.GetLength()) {
            return BasicString<Ch>::Append(other);
        } else {
            return BasicString<Ch>::Append(other.GetConstData() + otherOffset, length,
                                           currentFrontOffset, currentBackOffset);
        }
    }

    /**
     * Add a character count of times at the beginning of string.\n
     * If the buffer is not large enough, it'll automatically enlarge and reinitialize.
     * @date January 8th 2023
     * @param ch indicated character
     * @param count count
     * @param frontOffset reserved data before added characters.
     * @param backOffset reserved data behind added characters.
     * @return itself
     */
    BasicString<Ch> &Prepend(const Ch &ch, SizeType count = 1,
                             SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (ch && count) {
            if (Ch *pos = BasicString<Ch>::GrowthPrepend(frontOffset + count + backOffset)) {
                CharTrait<Ch>::Fill(pos + frontOffset, ch, count);
            }
        }
        return *this;
    }

    /**
     * Add a string by the indicated length at the end of string.\n
     * If the buffer is not large enough, it'll automatically enlarge and reinitialize.
     * @date January 8th 2023
     * @param str indicated string
     * @param length length of new string.
     * @param frontOffset reserved data before added characters.
     * @param backOffset reserved data behind added characters.
     * @return itself
     */
    BasicString<Ch> &Prepend(const Ch *str, SizeType length,
                             SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
        if (str && length) {
            if (Ch *pos = BasicString<Ch>::GrowthPrepend(frontOffset + length + backOffset)) {
                CharTrait<Ch>::Copy(pos + frontOffset, str, length);
            }
        }
        return *this;
    }

    /**
     * Add a string by the indicated length at the beginning of string.\n
     * Its length is decided by position of zero.
     * @param str indicated string
     * @return itself
     */
    BasicString<Ch> &Prepend(const Ch *str) noexcept {
        return BasicString<Ch>::Prepend(str, CharTrait<Ch>::GetLength(str), 0, 0);
    }


    /**
     * Add a string object at the beginning of string.\n
     * Its length is decided by its member variable.
     * @param other another object (cannot be itself)
     * @return itself
     */
    BasicString<Ch> &Prepend(const BasicString<Ch> &other) noexcept {
        if (mode_ == StringMode::Null) {
            new(this)BasicString<Ch>(other);
        } else {
            return BasicString<Ch>::Prepend(other.GetConstData(), other.GetLength());
        }
        return *this;
    }

    /**
     * Add a string object by indicated length at the beginning of string.\n
     * If the length is larger than its real length, it'll be detected.
     * @param other another object
     * @param length indicated length
     * @param otherOffset characters that'll be skipped behind the head of data.
     * @param currentFrontOffset reserved data before added characters.
     * @param currentBackOffset reserved data behind added characters.
     * @return
     */
    BasicString<Ch> &Prepend(const BasicString<Ch> &other, SizeType length, SizeType otherOffset = 0,
                             SizeType currentFrontOffset = 0, SizeType currentBackOffset = 0) {
        if (length == other.GetLength()) {
            return BasicString<Ch>::Prepend(other);
        } else {
            return BasicString<Ch>::Prepend(other.GetConstData() + otherOffset, length,
                                            currentFrontOffset, currentBackOffset);
        }
    }

    BasicString<Ch> &Insert(SizeType index, const Ch &ch, SizeType count = 1,
                            SizeType frontOffset = 0, SizeType backOffset = 0) {
        assert(index < BasicString<Ch>::GetLength());
        if (ch && count) {
            if (Ch *pos = BasicString<Ch>::GrowthInsert(index, frontOffset + count + backOffset)) {
                CharTrait<Ch>::Fill(pos + frontOffset, ch, count);
            }
        }
    }

    BasicString<Ch> &Insert(SizeType index, const Ch *str, SizeType length = 1,
                            SizeType frontOffset = 0, SizeType backOffset = 0) {
        assert(index < BasicString<Ch>::GetLength());
        if (str && length) {
            if (Ch *pos = BasicString<Ch>::GrowthInsert(index, frontOffset + length + backOffset)) {
                CharTrait<Ch>::Fill(pos + frontOffset, str, length);
            }
        }
    }

    BasicString<Ch> &Insert(SizeType index, const BasicString<Ch> &other, SizeType length, SizeType otherOffset = 0,
                            SizeType currentFrontOffset = 0, SizeType currentBackOffset = 0) {
        assert(index < BasicString<Ch>::GetLength());

        return BasicString<Ch>::Insert(index, other.GetConstData() + otherOffset, length,
                                       currentFrontOffset, currentBackOffset);
    }

    BasicString<Ch> &Insert(SizeType index, const Ch *str) {
        return BasicString<Ch>::Insert(index, str, CharTrait<Ch>::GetLength(str), 0, 0);
    }

    BasicString<Ch> &Insert(SizeType index, const BasicString<Ch> &other) {
        return BasicString<Ch>::Insert(index, other.GetConstData(), other.GetLength(), 0, 0);
    }

    BasicString<Ch> &Reverse() {
        switch (mode_) {
            case StringMode::SmallString:
                CharTrait<Ch>::Reverse(sso_);
                break;
            case StringMode::NeedAllocate: {
                if (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) {
                    Ch *oldStr = buf_.str_;
                    (**buf_.buf_).DecrementRef();
                    Ch *newStr = BasicString<Ch>::Initialize(buf_.len_, true);
                    for (SizeType index = 0; index < buf_.len_; ++index) {
                        newStr[index] = oldStr[buf_.len_ - 1 - index];
                    }
                } else {
                    CharTrait<Ch>::Reverse(buf_.str_);
                }
                break;
            }
            default:
                assert(false);
        }
        return *this; /** @bug I forgot to write this OwO. */
    }

    BasicString<Ch> Left(const SizeType &left) const noexcept {
        if (left >= BasicString<Ch>::GetLength()) {
            return (*this);
        } else {
            return BasicString<Ch>(BasicString<Ch>::GetConstData(), left);
        }
    }

    BasicString<Ch> Right(const SizeType &right) const noexcept {
        if (right >= BasicString<Ch>::GetLength()) {
            return *this;
        } else {
            return BasicString<Ch>(BasicString<Ch>::GetConstData() + BasicString<Ch>::GetLength() - right, right);
        }
    }

    BasicString<Ch> Middle(const SizeType &index, const SizeType &count) const noexcept {
        if (index + count > BasicString<Ch>::GetLength()) {
            return *this;
        }
        return BasicString<Ch>(BasicString<Ch>::GetConstData() + index, count);
    }

};

using StringA = BasicString<char>;
using StringW = BasicString<wchar_t>;
using String = BasicString<Char>;

#endif //ESCAPIST_STRING_H
