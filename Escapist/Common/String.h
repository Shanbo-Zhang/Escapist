//
// Created by Escap on 1/7/2023.
//

#ifndef ESCAPIST_STRING_H
#define ESCAPIST_STRING_H

#include "../General.h"
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

template<typename Ch>
class BasicString {

};

#endif //ESCAPIST_STRING_H
