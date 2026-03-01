#include "grid.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

std::vector<Interval> read_intervals(std::ifstream& fin) {
    int n;
    fin >> n;
    std::vector<Interval> intervals(n);
    for (int i = 0; i < n; ++i) {
        fin >> intervals[i].start >> intervals[i].end >> intervals[i].h0 >> intervals[i].k;
    }
    return intervals;
}

std::vector<double> generate_coords(const std::vector<Interval>& intervals) {
    std::vector<double> coords;
    for (const auto& inv : intervals) {
        double x = inv.start;
        coords.push_back(x);
        double h = inv.h0;
        int step = 0;
        while (x + h < inv.end + EPS) {
            x += h;
            coords.push_back(x);
            step++;
            h = inv.h0 * std::pow(inv.k, step); // следующий шаг
        }

        if (!double_equal(x, inv.end)) {
            coords.push_back(inv.end);
        }
    }
    return coords;
}