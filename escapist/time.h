#ifndef ESCAPIST_TIME_H
#define ESCAPIST_TIME_H

#include "base.h"

enum class DayOfWeek : int {
    kMonday,
    kTuesday,
    kWednesday,
    kThursday,
    kFriday,
    kSaturday,
    kSunday
};

class Date {
public:
    Date() : Date(-1, -1, -1) {}

    Date(const int &year, const int &month, const int &day)
            : year_(year), month_(month), day_(day) {}

    Date(const Date &other) noexcept = default;

    ~Date() noexcept = default;

    bool IsValid() const noexcept {
        return year_ > 0 && month_ > 0 && day_ > 0;
    }

    static int Compare(const int &left_year, const int &left_month, const int &left_day,
                       const int &right_year, const int &right_month, const int &right_day) {
        if (left_year == right_year) {
            if (left_month == right_month) {
                if (left_day == right_day) {
                    return 0;
                } else {
                    return left_day < right_day ? -1 : 1;
                }
            } else {
                return left_month < right_month ? -1 : 1;
            }
        } else {
            return left_year < right_year ? -1 : 1;
        }
    }

    int CompareTo(const int &year, const int &month, const int &day) const noexcept {
        assert(Date::IsValid());
        return Date::Compare(year_, month_, day_, year, month, day);
    }

    int CompareTo(const Date &other) const {
        assert(other.IsValid());
        return Date::CompareTo(other.year_, other.month_, other.day_);
    }

    bool IsBefore(const int &year, const int &month, const int &day) const {
        return Date::CompareTo(year, month, day) == -1;
    }

    bool IsBefore(const Date &other) const {
        return Date::CompareTo(other) == -1;
    }

    bool IsAfter(const int &year, const int &month, const int &day) const {
        return Date::CompareTo(year, month, day) == 1;
    }

    bool IsAfter(const Date &other) const {
        return Date::CompareTo(other) == 1;
    }

    bool Equals(const int &year, const int &month, const int &day) const {
        return Date::CompareTo(year, month, day) == 0;
    }

    bool Equals(const Date &other) const {
        return Date::CompareTo(other) == 0;
    }

    int Year() const noexcept {
        return year_;
    }

    int Month() const noexcept {
        return month_;
    }

    int Day() const noexcept {
        return day_;
    }

    Date &SetYear(const int &value) {
        year_ = value;
        return *this;
    }

    Date &SetMonth(const int &value) {
        month_ = value;
        return *this;
    }

    Date &SetDay(const int &value) {
        day_ = value;
        return *this;
    }

private:
    int year_;
    int month_;
    int day_;
};

#endif //ESCAPIST_TIME_H
