//
// Created by Escap on 1/17/2023.
//

#ifndef ESCAPIST_RECT_H
#define ESCAPIST_RECT_H

#include "../General.h"

class Rect {
private:
    long left_;
    long top_;
    long right_;
    long bottom_;

public:
    Rect() noexcept: left_(0), top_(0), right_(0), bottom_(0) {}

    Rect(const long &left, const long &top, const long &right, const long &bottom) noexcept
            : left_(left), top_(top), right_(right), bottom_(bottom) {}

    Rect(const Rect &other) noexcept
            : left_(other.left_), top_(other.top_), right_(other.right_), bottom_(other.bottom_) {}

    long GetLeft() const noexcept {
        return left_;
    }

    long GetTop() const noexcept {
        return top_;
    }

    long GetRight() const noexcept {
        return right_;
    }

    long GetBottom() const noexcept {
        return bottom_;
    }

    long GetWidth() const noexcept {
        return right_ - left_;
    }

    long GetHeight() const noexcept {
        return bottom_ - top_;
    }

    Rect &SetLeft(const long &left) noexcept {
        left_ = left;
        return *this;
    }

    Rect &SetTop(const long &top) noexcept {
        top_ = top;
        return *this;
    }

    Rect &SetRight(const long &right) noexcept {
        right_ = right;
        return *this;
    }

    Rect &SetBottom(const long &bottom) noexcept {
        bottom_ = bottom;
        return *this;
    }

    Rect &SetWidth(long value, bool changeLeft = false) {
        if (changeLeft) {
            left_ = right_ - value;
        } else {
            right_ = left_ + value;
        }
        return *this;
    }

    Rect &SetHeight(long value, bool changeTop = false) {
        if (changeTop) {
            top_ = bottom_ - value;
        } else {
            bottom_ = top_ + value;
        }
        return *this;
    }
};


#endif //ESCAPIST_RECT_H
