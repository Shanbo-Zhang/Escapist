//
// Created by Escap on 1/8/2023.
//

#ifndef ESCAPIST_JSON_H
#define ESCAPIST_JSON_H

#include "../General.h"
#include "String.h"
#include "ArrayList.h"

#include "../General.h"
#include "ArrayList.h"
#include "String.h"

enum class JsonErrorCode {
    NoError = 0,
};

enum class JsonValueType {
    Undefined = 0,
    Null = 0x00000001, // null
    Boolean = 0x00000002, // true or false
    Number = 0x00000004,
    Integer = 0x00000008 | Number,
    LongInteger = 0x00000010 | Number,
    FloatingPoint = 0x00000020 | Number,
    String = 0x00000040,
    Array = 0x00000080,
    Object = 0x00000100
};

class JsonValue;

class JsonMember;

class JsonArray;

class JsonObject;

class JsonArray {
private:
    ArrayList<JsonValue> values;

public:
    JsonArray() : values() {}

    JsonArray(const JsonArray &arr) noexcept = default;
};

class JsonObject {
private:
    ArrayList<JsonMember> values;

public:
    JsonObject() : values() {}

    JsonObject(const JsonObject &obj) = default;


};

class JsonValue {
private:
    JsonValueType type;
    union {
        bool boolean;
        int integer;
        long long longInteger;
        double floatingPoint;
        String *string;
        JsonArray *array;
        JsonObject *object;
    };

public:
    JsonValue() noexcept: type(JsonValueType::Undefined) {}

    JsonValue(const JsonValueType &valueType) : type(valueType) {}

    JsonValue(const bool &value) noexcept
            : type(JsonValueType::Boolean),
              boolean(value) {}

    JsonValue(const int &value) noexcept
            : type(JsonValueType::Integer),
              integer(value) {}

    JsonValue(const long long &value) noexcept
            : type(JsonValueType::LongInteger),
              longInteger(value) {}

    JsonValue(const double &value) noexcept
            : type(JsonValueType::FloatingPoint),
              floatingPoint(value) {}

    JsonValue(const String &value) noexcept
            : type(JsonValueType::String),
              string(new String(value)) {}

    JsonValue(const JsonArray &value) noexcept
            : type(JsonValueType::Array),
              array(new JsonArray(value)) {}

    JsonValue(const JsonObject &value) noexcept
            : type(JsonValueType::Object),
              object(new JsonObject(value)) {}

    ~JsonValue() noexcept {
        JsonValue::FreeValue();
    }

    void FreeValue() noexcept {
        switch (type) {
            case JsonValueType::String:
                if (string) {
                    string->~String();
                    ::free((void *) string);
                }
            case JsonValueType::Array:
                if (array) {
                    array->~JsonArray();
                    ::free((void *) array);
                }
            case JsonValueType::Object:
                if (object) {
                    // TODO: add JsonObject destructor.
                    ::free((void *) object);
                }
        }
    }

    JsonValueType GetType() const noexcept {
        return type;
    }

    bool IsNull() const noexcept {
        return type == JsonValueType::Null;
    }

    bool IsBoolean() const noexcept {
        return type == JsonValueType::Boolean;
    }

    bool IsNumber() const noexcept {
        return ((int) type) | (((int) JsonValueType::Number) == ((int) type));
    }

    bool IsInteger() const noexcept {
        return type == JsonValueType::Integer;
    }

    bool IsLongInteger() const noexcept {
        return type == JsonValueType::LongInteger;
    }

    bool IsFloatingPoint() const noexcept {
        return type == JsonValueType::FloatingPoint;
    }

    bool IsString() const noexcept {
        return type == JsonValueType::String;
    }

    bool IsArray() const noexcept {
        return type == JsonValueType::Array;
    }

    bool IsObject() const noexcept {
        return type == JsonValueType::Object;
    }

    bool GetBoolean() const {
        assert(type == JsonValueType::Boolean);
        return boolean;
    }

    int GetInteger() const {
        assert(JsonValue::IsNumber());
        switch (type) {
            case JsonValueType::Integer:
                return integer;
            case JsonValueType::LongInteger:
                return (int) longInteger;
            case JsonValueType::FloatingPoint:
                return (int) floatingPoint;
            default:
                assert(false);
        }
    }

    long long GetLongInteger() const {
        assert(JsonValue::IsNumber());
        switch (type) {
            case JsonValueType::LongInteger:
                return longInteger;
            case JsonValueType::Integer:
                return (long long) integer;
            case JsonValueType::FloatingPoint:
                return (long long) floatingPoint;
            default:
                assert(false);
        }
    }

    double GetFloatingPoint() const {
        assert(JsonValue::IsNumber());
        switch (type) {
            case JsonValueType::FloatingPoint:
                return floatingPoint;
            case JsonValueType::Integer:
                return (double) integer;
            case JsonValueType::LongInteger:
                return (double) longInteger;
            default:
                assert(false);
        }
    }

    void SetNull() noexcept {
        if (type != JsonValueType::Null) {
            JsonValue::FreeValue();
            type = JsonValueType::Null;
        }
    }

    void SetBoolean(const bool &value) noexcept {
        if (type != JsonValueType::Boolean) {
            JsonValue::FreeValue();
            type = JsonValueType::Boolean;
        }
        boolean = value;
    }

    void SetInteger(const int &value) noexcept {
        if (type != JsonValueType::Integer) {
            JsonValue::FreeValue();
            type = JsonValueType::Integer;
        }
        integer = value;
    }

    void SetLongInteger(const long long &value) noexcept {
        if (type != JsonValueType::LongInteger) {
            JsonValue::FreeValue();
            type = JsonValueType::LongInteger;
        }
        longInteger = value;
    }

    void SetFloatingPoint(const double &value) noexcept {
        if (type != JsonValueType::FloatingPoint) {
            JsonValue::FreeValue();
            type = JsonValueType::FloatingPoint;
        }
        floatingPoint = value;
    }

    void SetString(const String &value) noexcept {
        if (type != JsonValueType::String) {
            JsonValue::FreeValue();
            type = JsonValueType::String;
            string = (String *) ::malloc(sizeof(String));
            new(string)String(value);
        } else {
            string->Assign(value);
        }
    }
};

class JsonMember {
private:
    String key;
    JsonValue value;

public:
    JsonMember() = delete;

    JsonMember(const String &theKey, const JsonValue &theValue) noexcept
            : key(theKey), value(theValue) {}

    JsonMember(const JsonMember &other) noexcept
            : key(other.key), value(other.value) {}

    String &GetKey() noexcept {
        return key;
    }

    const String &GetConstKey() noexcept {
        return key;
    }

    void SetKey(const String &newKey) noexcept {
        key.Assign(newKey);
    }

    JsonValue &GetValue() {
        return value;
    }

    const JsonValue &GetConstValue() const {
        return value;
    }
};


#endif //ESCAPIST_JSON_H
