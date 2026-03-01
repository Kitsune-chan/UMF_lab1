#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

std::vector<double> point_sor(const std::vector<double>& d0,
    const std::vector<double>& d1,
    const std::vector<double>& d2,
    const std::vector<double>& d3,
    const std::vector<double>& d4,
    const std::vector<double>& b,
    int Nx, int Ny,
    double omega, double tol, int max_iter);

#endif