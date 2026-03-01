#ifndef UTILS_H
#define UTILS_H

#include <cmath>

const double EPS = 1e-12;

inline bool double_equal(double a, double b) {
    return std::fabs(a - b) < EPS;
}

#endif 