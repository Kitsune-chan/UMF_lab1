#ifndef GRID_H
#define GRID_H

#include <vector>
#include <fstream>

struct Interval {
    double start, end, h0, k;
};

std::vector<Interval> read_intervals(std::ifstream& fin);
std::vector<double> generate_coords(const std::vector<Interval>& intervals);

#endif 