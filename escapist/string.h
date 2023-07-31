/*
 *
 */

#ifndef ESCAPIST_STRING_H
#define ESCAPIST_STRING_H

#include "base.h"
#include <type_traits>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>

template<typename Ch>
struct ICharTrait {
    /**
     * Copy the first {count} of characters from {src} to {dest}
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Copy(Ch *dest, const Ch *src, SizeType count) {
        assert(dest && src && count);
        for (; count > 0; ++dest, ++src, --count) {
            *dest = *src;
        }
    }

    /**
     * Copy the first {count} of characters from {src} to {dest},
     * but allow {dest} and {src} to overlap
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Move(Ch *dest, const Ch *src, SizeType count) {
        assert(dest && src && count);
        if ((dest <= src) || (dest > src + count)) {
            for (; count; ++dest, ++src, --count) {
                *dest = *src;
            }
        } else {
            dest = dest + count - 1;
            src = src + count - 1;
            for (; count; --dest, --src, --count)
                *dest = *src;
        }
    }

    /**
     * Fill [val] character [count] times to [dest]
     * @param dest destination, the address to be filled.
     * @param val character to fill {dest}
     * @param count how many {val} to be filled.
     */
    static inline void Fill(Ch *dest, const Ch &val, SizeType count) {
        for (; count > 0; ++dest, --count) {
            *dest = val;
        }
    }

    /**
     * @param src null-terminated string
     * @return the length if the given string
     */
    static inline SizeType Length(const Ch *src) {
        SizeType len = 0;
        for (; *src; ++src, ++len);
        return len;
    }

    /**
     * Compare between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     */
    static inline int Compare(const Ch *left, const Ch *right) {
        assert(left && right);
        if (left == right) {
            return 0;
        }
        for (; *left && *right && *left == *right; ++left, ++right);
        return *left - *right;
    }

    /**
     * Compare the first {count} characters between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     */
    static inline int Compare(const Ch *left, const Ch *right, SizeType count) {
        assert(left && right && count);
        // Terminate if some strings terminate,
        // or the one does not equal to another,
        // or the maximum count reaches.
        for (; *left && *right && *left == *right && count; ++left, ++right, --count);
        return *left - *right;
    }

    /**
     * Compare between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const Ch *left, const Ch *right) {
        assert(left && right);
        if (left == right) {
            return 0;
        }
        for (; *left && *right && tolower(*left) == tolower(*right); ++left, ++right);
        return *left - *right;
    }

    /**
     * Compare the first \p count characters between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const Ch *left, const Ch *right, SizeType count) {
        assert(left && right);
        for (; *left && *right && tolower(*left) == tolower(*right) && count; ++left, ++right, --count);
        return *left - *right;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const Ch *Find(const Ch *src, const Ch &val) {
        assert(src);
        for (; *src; ++src) {
            if (*src == val) {
                return src;
            }
        }
        return nullptr;
    }

    /**
     * Find the first occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const Ch *Find(const Ch *src, const Ch &val, SizeType count) {
        assert(src && count);
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                return src;
            }
        }
        return nullptr;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const Ch *Find(const Ch *src, const Ch *sub) {
        assert(src);
        if (!sub) {
            return nullptr;
        }
        for (; *src; ++src) {
            if (*src == *sub) { // the first char occurs, start to check
                const Ch *src_ref = src, // prevent {src} from changing to ensure every char is scanned.
                *sub_ref = sub; // also make a copy of {sub} to go back if doesn't match.
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref; ++src_ref, ++sub_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    return src; // Thus, return it!
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return nullptr;
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const Ch *Find(const Ch *src, const Ch *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const Ch *src_ref = src, // prevent {src} from changing to ensure every char is scanned.
                *sub_ref = sub; // also make a copy of {sub} to go back if doesn't match.
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    return src; // Thus, return it!
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return nullptr;
    }

    /**
     * Find the last occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const Ch *ReverseFind(const Ch *src, const Ch &val) {
        assert(src);
        const Ch *last = nullptr;
        for (; *src; ++src) {
            if (*src == val) {
                last = src;
            }
        }
        return last;
    }

    /**
     * Find the last occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const Ch *ReverseFind(const Ch *src, const Ch &val, SizeType count) {
        assert(src && count);
        const Ch *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                last = src;
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const Ch *ReverseFind(const Ch *src, const Ch *sub) {
        assert(src);
        if (!sub) {
            return nullptr;
        }
        const Ch *last = nullptr;
        for (; *src; ++src) {
            if (*src == *sub) { // the first char occurs, start to check
                const Ch *src_ref = src,
                        *sub_ref = sub;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref; ++src_ref, ++sub_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const Ch *ReverseFind(const Ch *src, const Ch *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        const Ch *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const Ch *src_ref = src,
                        *sub_ref = sub;
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    // TODO: Find & ReverseFind Ignore Case
    // TODO: NthFind & NthReverseFind & Ignore Case

    /**
     * Reverse the given null-terminated string \p src
     * @param src given null-terminated string
     */
    static inline void Reverse(Ch *src) {
        ICharTrait<Ch>::Reverse(src, ICharTrait<Ch>::Length(src));
    }

    /**
     * Reverse the first \p count of characters of the given null-terminated string \p src
     * @param src given null-terminated string
     */
    static inline void Reverse(Ch *src, SizeType count) {
        assert(src && count);
        Ch *begin = src,
                *end = src + count - 1;
        while (end - begin > 0) {
            *begin = (*begin) ^ (*end);
            *end = (*begin) ^ (*end);
            *begin = (*begin) ^ (*end);
            ++begin, --end;
        }
        // return src;
    }
};

template<>
struct ICharTrait<char> {
    /**
     * Copy the first {count} of characters from {src} to {dest}
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Copy(char *dest, const char *src, SizeType count) {
        ::memcpy(reinterpret_cast<void *>(dest), reinterpret_cast<const void *>(src), count);
    }

    /**
     * Copy the first {count} of characters from {src} to {dest},
     * but allow {dest} and {src} to overlap
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Move(char *dest, const char *src, SizeType count) {
        ::memmove(reinterpret_cast<void *>(dest), reinterpret_cast<const void *>(src), count);
    }

    /**
     * Fill [val] character [count] times to [dest]
     * @param dest destination, the address to be filled.
     * @param val character to fill {dest}
     * @param count how many {val} to be filled.
     */
    static inline void Fill(char *dest, const char &val, SizeType count) {
        ::memset(reinterpret_cast<char *>(dest), val, count);
    }

    /**
     * @param src null-terminated string
     * @return the length if the given string
     */
    static inline SizeType Length(const char *src) {
        return ::strlen(src);
    }

    /**
     * Compare between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     */
    static inline int Compare(const char *left, const char *right) {
        return ::strcmp(left, right);
    }

    /**
     * Compare the first {count} characters between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     */
    static inline int Compare(const char *left, const char *right, SizeType count) {
        return ::memcmp(reinterpret_cast<const void *>(left), reinterpret_cast<const void *>(right), count);
    }

    /**
     * Compare between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const char *left, const char *right) {
        return ::strcasecmp(left, right);
    }

    /**
     * Compare the first \p count characters between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const char *left, const char *right, SizeType count) {
        assert(left && right);
        for (; *left && *right && tolower(*left) == tolower(*right) && count; ++left, ++right, --count);
        return *left - *right;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const char *Find(const char *src, const char &val) {
        return ::strchr(src, val);
    }

    /**
     * Find the first occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const char *Find(const char *src, const char &val, SizeType count) {
        assert(src && count);
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                return src;
            }
        }
        return nullptr;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const char *Find(const char *src, const char *sub) {
        return ::strstr(src, sub);
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const char *Find(const char *src, const char *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const char *src_ref = src, // prevent {src} from changing to ensure every char is scanned.
                *sub_ref = sub; // also make a copy of {sub} to go back if doesn't match.
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    return src; // Thus, return it!
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return nullptr;
    }

    /**
     * Find the last occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const char *ReverseFind(const char *src, const char &val) {
        return ::strrchr(src, val);
    }

    /**
     * Find the last occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const char *ReverseFind(const char *src, const char &val, SizeType count) {
        assert(src && count);
        const char *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                last = src;
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const char *ReverseFind(const char *src, const char *sub) {
        assert(src);
        if (!sub) {
            return nullptr;
        }
        const char *last = nullptr;
        for (; *src; ++src) {
            if (*src == *sub) { // the first char occurs, start to check
                const char *src_ref = src,
                        *sub_ref = sub;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref; ++src_ref, ++sub_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const char *ReverseFind(const char *src, const char *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        const char *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const char *src_ref = src,
                        *sub_ref = sub;
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    // TODO: Find & ReverseFind Ignore Case
    // TODO: NthFind & NthReverseFind & Ignore Case

    /**
     * Reverse the given null-terminated string \p src
     * @param src given null-terminated string
     */
    static inline void Reverse(char *src) {
        ICharTrait<char>::Reverse(src, ICharTrait<char>::Length(src));
    }

    /**
     * Reverse the first \p count of characters of the given null-terminated string \p src
     * @param src given null-terminated string
     */
    static inline void Reverse(char *src, SizeType count) {
        assert(src && count);
        char *begin = src,
                *end = src + count - 1;
        while (end - begin > 0) {
            *begin = (*begin) ^ (*end);
            *end = (*begin) ^ (*end);
            *begin = (*begin) ^ (*end);
            ++begin, --end;
        }
        // return src;
    }
};

template<>
struct ICharTrait<wchar_t> {
    /**
     * Copy the first {count} of characters from {src} to {dest}
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Copy(wchar_t *dest, const wchar_t *src, SizeType count) {
        ::memcpy(dest, src, count * sizeof(wchar_t));
    }

    /**
     * Copy the first {count} of characters from {src} to {dest},
     * but allow {dest} and {src} to overlap
     * @param dest the place to be copied.
     * @param src the source
     * @param count how many characters to be copied.
     */
    static inline void Move(wchar_t *dest, const wchar_t *src, SizeType count) {
        ::memmove(reinterpret_cast<void *>(dest), reinterpret_cast<const void *>(src), count * sizeof(wchar_t));
    }

    /**
     * Fill [val] character [count] times to [dest]
     * @param dest destination, the address to be filled.
     * @param val character to fill {dest}
     * @param count how many {val} to be filled.
     */
    static inline void Fill(wchar_t *dest, const wchar_t &val, SizeType count) {
        ::wmemset(dest, val, count);
    }

    /**
     * @param src null-terminated string
     * @return the length if the given string
     */
    static inline SizeType Length(const wchar_t *src) {
        return ::wcslen(src);
    }

    /**
     * Compare between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     */
    static inline int Compare(const wchar_t *left, const wchar_t *right) {
        return ::wcscmp(left, right);
    }

    /**
     * Compare the first {count} characters between {left} string and {right} string.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     */
    static inline int Compare(const wchar_t *left, const wchar_t *right, SizeType count) {
        return ::wmemcmp(left, right, count);
    }

    /**
     * Compare between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @return zero two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const wchar_t *left, const wchar_t *right) {
        assert(left && right);
        if (left == right) {
            return 0;
        }
        for (; *left && *right && tolower(*left) == tolower(*right); ++left, ++right);
        return *left - *right;
    }

    /**
     * Compare the first \p count characters between {left} string and {right} string,
     * but ignore the case.
     * @param left the first null-terminated string
     * @param right the second null-terminated string
     * @param count the maximum count of compared characters.
     * @return zero if the first {count} of characters of two strings are equal
     * TODO: find a more efficient way!!!
     */
    static inline int CompareNoCase(const wchar_t *left, const wchar_t *right, SizeType count) {
        assert(left && right);
        for (; *left && *right && tolower(*left) == tolower(*right) && count; ++left, ++right, --count);
        return *left - *right;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const wchar_t *Find(const wchar_t *src, const wchar_t &val) {
        return ::wcschr(src, val);
    }

    /**
     * Find the first occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const wchar_t *Find(const wchar_t *src, const wchar_t &val, SizeType count) {
        assert(src && count);
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                return src;
            }
        }
        return nullptr;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const wchar_t *Find(const wchar_t *src, const wchar_t *sub) {
        return ::wcsstr(src, sub);
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const wchar_t *Find(const wchar_t *src, const wchar_t *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const wchar_t *src_ref = src, // prevent {src} from changing to ensure every char is scanned.
                *sub_ref = sub; // also make a copy of {sub} to go back if doesn't match.
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    return src; // Thus, return it!
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return nullptr;
    }

    /**
     * Find the last occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const wchar_t *ReverseFind(const wchar_t *src, const wchar_t &val) {
        return ::wcsrchr(src, val);
    }

    /**
     * Find the last occurrence of \p val in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param val the character to be searched in \p src
     * @param count maximum amount of characters to examine.
     * @return pointer to found character if found; nullptr if no such character is found.
     */
    static inline const wchar_t *ReverseFind(const wchar_t *src, const wchar_t &val, SizeType count) {
        assert(src && count);
        const wchar_t *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == val) {
                last = src;
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p val in the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const wchar_t *ReverseFind(const wchar_t *src, const wchar_t *sub) {
        assert(src);
        if (!sub) {
            return nullptr;
        }
        const wchar_t *last = nullptr;
        for (; *src; ++src) {
            if (*src == *sub) { // the first char occurs, start to check
                const wchar_t *src_ref = src,
                        *sub_ref = sub;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref; ++src_ref, ++sub_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    /**
     * Find the first occurrence of \p sub in the initial \p count of the null-terminated string \p src.
     * @param src the null-terminated string
     * @param sub the null-terminated string to be searched in \p src
     * @param count
     * @return pointer to the address of the first occurrence of \p sub if found; nullptr if no such character is found.
     */
    static inline const wchar_t *ReverseFind(const wchar_t *src, const wchar_t *sub, SizeType count) {
        assert(src && count);
        if (!sub) {
            return nullptr;
        }
        const wchar_t *last = nullptr;
        for (; *src && count; ++src, --count) {
            if (*src == *sub) { // the first char occurs, start to check
                const wchar_t *src_ref = src,
                        *sub_ref = sub;
                SizeType count_ref = count;
                for (; *src_ref && *sub_ref && *src_ref == *sub_ref && count_ref; ++src_ref, ++sub_ref, --count_ref);
                if (!*sub_ref) { // finished scanning, and all char in sub matched.
                    last = src;
                } // Otherwise, src has terminated, or they do not match at all.
            }
        }
        return last;
    }

    // TODO: Find & ReverseFind Ignore Case
    // TODO: NthFind & NthReverseFind & Ignore Case

    /**
 * Reverse the given null-terminated string \p src
 * @param src given null-terminated string
 */
    static inline void Reverse(wchar_t *src) {
        ICharTrait<wchar_t>::Reverse(src, ICharTrait<wchar_t>::Length(src));
    }

    /**
     * Reverse the first \p count of characters of the given null-terminated string \p src
     * @param src given null-terminated string
     */
    static inline void Reverse(wchar_t *src, SizeType count) {
        assert(src && count);
        wchar_t *begin = src,
                *end = src + count - 1;
        while (end - begin > 0) {
            *begin = (*begin) ^ (*end);
            *end = (*begin) ^ (*end);
            *begin = (*begin) ^ (*end);
            ++begin, --end;
        }
        // return src;
    }
};

template<typename Ch>
class BasicString {

};

#endif //ESCAPIST_STRING_H
