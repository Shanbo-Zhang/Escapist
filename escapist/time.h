#ifndef ESCAPIST_TIME_H
#define ESCAPIST_TIME_H

#include "base.h"
#include <ctime>

class Clock {
public:
    Clock() : hour_(0), minute_(0), second_(0) {}

    Clock(int hour, int minute, int second) noexcept
            : hour_(hour), minute_(minute), second_(second) {
        assert(hour >= 0 && hour <= 23);
        assert(minute >= 0 && minute <= 59);
        assert(second >= 0 && second <= 59);
    }

    Clock(time_t stamp) noexcept
            : second_(int(stamp % 60)), minute_(int((stamp /= 60) % 60)), hour_(int((stamp /= 60) % 24)) {}

    Clock(const Clock &other) noexcept
            : hour_(other.hour_), minute_(other.minute_), second_(other.second_) {}

    int Hour() const noexcept {
        return hour_;
    }

    int Minute() const noexcept {
        return minute_;
    }

    int Second() const noexcept {
        return second_;
    }

    Clock &SetHour(const int &value) {
        assert(value >= 0 && value <= 23);
        hour_ = value;
        return *this;
    }

    Clock& SetMinute(const int& value){
        assert(value >= 0 && value <= 59);
        minute_=value;
        return *this;
    }

    Clock& SetSecond(const int& value){
        assert(value >= 0 && value <= 59);
        second_=value;
        return *this;
    }

private:
    int hour_;
    int minute_;
    int second_;
};

class Calendar {
public:
    /**
     * Creates an empty, invalid Calendar instance.
     */
    Calendar() : year_(0), month_(0), day_(0), hour_(0), minute_(0), second_(0) {}

    /**
     * Creates an Calendar instance from another instance
     * @param other
     */
    Calendar(const Calendar &other)
            : year_(other.year_), month_(other.month_), day_(other.day_),
              hour_(other.hour_), minute_(other.minute_), second_(other.second_) {}

    /**
     * Creates a Calendar instance from the system timestamp.
     * @param time
     */
    Calendar(const time_t &time) {

    }

    /**
     * Creates a Calendar instance from a certain time
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param minute
     * @param second
     */
    Calendar(const int &year, const int &month, const int &day,
             const int &hour = 0, const int &minute = 0, const int &second = 0)
            : year_(year), month_(month), day_(day), hour_(hour), minute_(minute), second_(second) {}


private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;
};

#endif //ESCAPIST_TIME_H
