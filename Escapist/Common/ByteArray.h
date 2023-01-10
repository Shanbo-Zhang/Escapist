//
// Created by Escap on 1/7/2023.
//

#ifndef ESCAPIST_BYTEARRAY_H
#define ESCAPIST_BYTEARRAY_H

#include "../General.h"
#include "ArrayList.h"
#include "String.h"
#include <tchar.h>

using byte = unsigned char;

class ByteArray : public ArrayList<byte> {
private:
    SizeType mark;

public:
    ByteArray() noexcept: ArrayList<byte>(), mark(0) {}

    ByteArray(const byte *data, SizeType size, SizeType offset = 0) noexcept:
            ArrayList<byte>(data, size, offset), mark(0) {}

    ByteArray(const ByteArray &other) noexcept:
            ArrayList<byte>(other), mark(0) {}

    ByteArray(const ByteArray &other, SizeType size, SizeType otherOffset, SizeType currentOffset) noexcept:
            ArrayList<byte>(other, size, otherOffset, currentOffset), mark(0) {}

    ByteArray &ResetMark() noexcept {
        mark = 0;
    }

    ByteArray &IgnoreBytes(const SizeType &count) noexcept {
        ++mark;
    }

    template<typename T>
    T ReadSimpleValue() noexcept {
        static_assert(std::is_fundamental<T>::value);
        assert(GetSize() - mark >= sizeof(T));
        T rtn = *((T *) (GetConstData() + mark));
        mark += sizeof(T);
        return rtn;
    }

    template<typename T>
    ByteArray &WriteSimpleValue(const T &value) noexcept {
        static_assert(std::is_fundamental<T>::value);
        Append((byte *) &value, sizeof(T));
        return *this;
    }

    bool ReadBoolean() noexcept {
        return ReadSimpleValue<bool>();
    }

    char ReadChar() noexcept {
        return ReadSimpleValue<char>();
    }

    byte ReadByte() noexcept {
        return ReadSimpleValue<byte>();
    }

    short ReadShort() noexcept {
        return ReadSimpleValue<short>();
    }

    int ReadInt() noexcept {
        return ReadSimpleValue<int>();
    }

    unsigned int ReadUnsignedInt() noexcept {
        return ReadSimpleValue<unsigned int>();
    }

    long ReadLong() noexcept {
        return ReadSimpleValue<long>();
    }

    unsigned long ReadUnsignedLong() noexcept {
        return ReadSimpleValue<unsigned long>();
    }

    long long ReadLongLong() noexcept {
        return ReadSimpleValue<long long>();
    }

    unsigned long long ReadUnsignedLongLong() noexcept {
        return ReadSimpleValue<unsigned long long>();
    }

    float ReadFloat() noexcept {
        return ReadSimpleValue<float>();
    }

    double ReadDouble() noexcept {
        return ReadSimpleValue<double>();
    }

    long double ReadLongDouble() noexcept {
        return ReadSimpleValue<long double>();
    }

    ByteArray &WriteBoolean(bool value) noexcept {
        return WriteSimpleValue<bool>(value);
    }

    ByteArray &WriteChar(char value) noexcept {
        return WriteSimpleValue<char>(value);
    }

    ByteArray &WriteByte(byte value) noexcept {
        return WriteSimpleValue<byte>(value);
    }

    ByteArray &WriteShort(short value) noexcept {
        return WriteSimpleValue<short>(value);
    }

    ByteArray &WriteInt(int value) noexcept {
        return WriteSimpleValue<int>(value);
    }

    ByteArray &WriteUnsignedInt(unsigned int value) noexcept {
        return WriteSimpleValue<unsigned int>(value);
    }

    ByteArray &WriteLong(long value) noexcept {
        return WriteSimpleValue<long>(value);
    }

    ByteArray &WriteUnsignedLong(unsigned long value) noexcept {
        return WriteSimpleValue<unsigned long>(value);
    }

    ByteArray &WriteLongLong(long long value) noexcept {
        return WriteSimpleValue<long long>(value);
    }

    ByteArray &WriteUnsignedLongLong(unsigned long long value) noexcept {
        return WriteSimpleValue<unsigned long long>(value);
    }

    ByteArray &WriteFloat(float value) noexcept {
        return WriteSimpleValue<float>(value);
    }

    ByteArray &WriteDouble(double value) noexcept {
        return WriteSimpleValue<double>(value);
    }

    ByteArray &WriteLongDouble(long double value) noexcept {
        return WriteSimpleValue<long double>(value);
    }

    void ReadBytes(ByteArray &dest, SizeType maximum) noexcept {
        if (maximum >= ByteArray::GetSize() - mark) {
            maximum = ByteArray::GetSize() - mark;
        }
        dest.Assign(ByteArray::GetConstData() + mark, maximum);
    }

    ByteArray &WriteBytes(const ByteArray &src) noexcept {
        if (!src.IsEmpty()) {
            ByteArray::Append(src);
        }
        return *this;
    }

    String GetString() const noexcept {
        String result;
        Char each[4] = {0};
        for (SizeType index = 0; index < GetSize(); ++index) {
            wsprintf(each, L"%d", GetConstAt(index));
            result.Append(each);
            if (index < GetSize() - 1) {
                result.Append(L',');
            }
        }
        return (String&&)result.Prepend('{').Append(L'}');
    }
};

#endif //ESCAPIST_BYTEARRAY_H
