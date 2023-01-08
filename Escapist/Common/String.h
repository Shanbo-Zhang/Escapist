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
    using Self = CharTrait<Ch>;
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

    static SizeType IndexOf(const Char *data, const Char &ch) {

    }

    static SizeType IndexOf(const Char *data, const Char *target) {

    }

    static SizeType LastIndexOf(const Char *data, const Char &ch) {

    }

    static SizeType LastIndexOf(const Char *data, const Char *target) {

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

    static inline const char *IndexOf(const char *data, const char &ch) {
        return ::strchr(data, ch);
    }

    static inline const char *IndexOf(const char *data, const char *target) {
        return ::strstr(data, target);
    }

    static inline const char *LastIndexOf(const char *data, const char &ch) {
        return ::strrchr(data, ch);
    }

    static inline const char *LastIndexOf(const char *data, const char *target) {
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
            return lastPos;
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

    static inline const wchar_t *IndexOf(const wchar_t *data, const wchar_t &ch) {
        return ::wcschr(data, ch);
    }

    static inline const wchar_t *IndexOf(const wchar_t *data, const wchar_t *target) {
        return ::wcsstr(data, target);
    }

    static inline const wchar_t *LastIndexOf(const wchar_t *data, const wchar_t &ch) {
        return ::wcsrchr(data, ch);
    }

    static inline const wchar_t *LastIndexOf(const wchar_t *data, const wchar_t *target) {
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
            return lastPos;
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

    void InitEager(SizeType length, bool putZero) noexcept {
        if (mode_ != StringMode::NeedAllocate) { // When it switches from another mode.
            mode_ = StringMode::NeedAllocate;
        }
        buf_.len_ = length; // Assignment
        buf_.capacity_ = length * (long double) 1.5; // Narrowing conversion from 'SizeType'?
        assert(buf_.buf_ = (ReferenceCount **) ::malloc(BasicString<Ch>::TotalCapacity(buf_.capacity_)));
        *buf_.buf_ = nullptr; // Assign the reference count pointer.
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
    BasicString(const Ch &ch, SizeType count, SizeType frontOffset = 0, SizeType backOffset = 0) noexcept {
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

    BasicString(const BasicString<Ch> &other) noexcept: mode_(other.mode_), buf_(other.buf_) {
        if (mode_ == StringMode::NeedAllocate) { // we need to change something in this mode.
            if (*buf_.buf_) { // We copy it directly, but we need to increase the reference count if it has.
                (**buf_.buf_).IncrementRef();
            } else { // If it doesn't have, we need to create and initialize it by 2
                assert(*buf_.buf_ = (ReferenceCount *) ::malloc(sizeof(ReferenceCount)));
                new(*buf_.buf_)ReferenceCount(2);
            }
        }
    }

    BasicString(const BasicString<Ch> &other, SizeType length, SizeType, SizeType otherFrontOffset = 0,
                SizeType currentFrontOffset = 0, SizeType currentBackOffset = 0) noexcept {
        if (!currentFrontOffset && !currentBackOffset && !otherFrontOffset && length == other.GetLength()) {
            new(this)BasicString<Ch>(other);
        } else {
            new(this)BasicString<Ch>(other.GetConstData() + otherFrontOffset, length,
                                     currentFrontOffset, currentBackOffset);
        }
    }

    SizeType GetLength() const {
        switch (mode_) {
            case StringMode::Null:
                return 0;
            case StringMode::SmallString:
                return BasicString<Ch>::GetSmallLength();
            case StringMode::NeedAllocate:
                return buf_.len_;
            default:
                assert(false);
        }
    }

    SizeType GetCapacity() const {
        switch (mode_) {
            case StringMode::Null:
                return 0;
            case StringMode::SmallString:
                return BasicString<Ch>::SmallStringCapacity;
            case StringMode::NeedAllocate:
                return (*buf_.buf_ && (**buf_.buf_).GetValue() > 1) ? 0 : buf_.capacity_;
            default:
                assert(false);
        }
    }

    bool IsEmpty() const noexcept {
        switch (mode_) {
            case StringMode::Null:
                return true;
            case StringMode::SmallString:
                return !BasicString<Ch>::GetSmallLength();
            case StringMode::NeedAllocate:
                return !buf_.len_;
            default:
                assert(false);
        }
    }

    bool IsNull() const {
        switch (mode_) {
            case StringMode::Null:
                return true;
            case StringMode::SmallString:
                return false;
            case StringMode::NeedAllocate:
                return !(buf_.str_ && buf_.capacity_);
            default:
                assert(false);
        }
    }

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
            default:
                assert(false);
        }
    }

    const Ch *GetConstData() const {
        switch (mode_) {
            case StringMode::Null:
                return nullptr;
            case StringMode::SmallString:
                return sso_;
            case StringMode::NeedAllocate:
                return buf_.str_;
            default:
                break;
        }
        assert(false);
    }
};

using StringA = BasicString<char>;
using StringW = BasicString<wchar_t>;
using String = BasicString<Char>;

#endif //ESCAPIST_STRING_H
