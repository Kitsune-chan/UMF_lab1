#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

void build_problem(const std::vector<double>& x_coords,
    const std::vector<double>& y_coords,
    std::vector<double>& d0,
    std::vector<double>& d1,
    std::vector<double>& d2,
    std::vector<double>& d3,
    std::vector<double>& d4,
    std::vector<double>& b);

#endif