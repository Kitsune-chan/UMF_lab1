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
    bool first = true;
    for (const auto& inv : intervals) {
        double x = inv.start;
        if (first) {
            coords.push_back(x);
            first = false;
        }
        else {
            // Если начало совпадает с последней точкой, не добавляем повторно
            if (!double_equal(coords.back(), x)) {
                coords.push_back(x);
            }
        }
        double h = inv.h0;
        int step = 0;
        while (x + h < inv.end + EPS) {
            x += h;
            coords.push_back(x);
            step++;
            h = inv.h0 * std::pow(inv.k, step);
        }
        // Добавляем конец интервала, если он не совпадает с последней точкой
        if (!double_equal(x, inv.end)) {
            coords.push_back(inv.end);
        }
    }
    return coords;
}