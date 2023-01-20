//
// Created by Escap on 1/18/2023.
//

#ifndef ESCAPIST_POINT_H
#define ESCAPIST_POINT_H

class Point {
private:
    long x_;
    long y_;

public:
    Point()noexcept :x_(0), y_(0) {}
    Point(const long& x, const long& y) noexcept :x_(x), y_(y) {}
    Point(const Point& other)noexcept :x_(other.x_), y_(other.y_) {}

    long GetX()const noexcept { return x_; }
    long GetY()const noexcept { return y_; }

    void SetX(const long& x) noexcept { x_ = x; }
    void SetY(const long& y) noexcept { y_ = y; }
};

#endif //ESCAPIST_POINT_H
