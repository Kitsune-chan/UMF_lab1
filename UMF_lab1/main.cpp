#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "grid.h"
#include "domain.h"
#include "matrix.h"
#include "solver.h"
#include "utils.h"

int main() {
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Ошибка: не удалось открыть файл input.txt" << std::endl;
        return 1;
    }

    auto x_intervals = read_intervals(fin);
    auto y_intervals = read_intervals(fin);

    std::cout << "=== файл прочитан ===" << std::endl;

    fin.close();

    std::vector<double> x_coords = generate_coords(x_intervals);
    std::vector<double> y_coords = generate_coords(y_intervals);

    std::cout << "\n=== сетка сгенерирована ===" << std::endl;

    int Nx = x_coords.size();
    int Ny = y_coords.size();
    std::cout << "Сетка: " << Nx << " x " << Ny << " = " << Nx * Ny << " узлов" << std::endl;

    // Построение матрицы и правой части
    std::vector<double> d0, d1, d2, d3, d4, b;
    build_problem(x_coords, y_coords, d0, d1, d2, d3, d4, b);

    // Решение СЛАУ методом SOR
    double omega = 1.5;       // параметр релаксации
    double tol = 1e-10;        // точность
    int max_iter = 100000;    // максимальное число итераций
    std::vector<double> solution = point_sor(d0, d1, d2, d3, d4, b, Nx, Ny, omega, tol, max_iter);

    // Оценка погрешности только внутри области
    double err_max = 0.0, err_norm = 0.0;
    int count = 0;
    for (int j = 0; j < Ny; ++j) {
        double y = y_coords[j];
        for (int i = 0; i < Nx; ++i) {
            double x = x_coords[i];
            if (inside_domain(x, y)) {
                int idx = j * Nx + i;
                double err = std::fabs(solution[idx] - u_exact(x, y));
                err_max = std::max(err_max, err);
                err_norm += err * err;
                ++count;

                std::cout << std::scientific << std::setprecision(6)
                    << idx << "\t" << x << "\t" << y << "\t"
                    << solution[idx] << "\t" << u_exact(x, y) << "\t" << err << std::endl;
            }
            
            
        }
    }
    err_norm = std::sqrt(err_norm / count);

    std::cout << "\n=== Оценка погрешности ===" << std::endl;
    std::cout << "Максимальная ошибка: " << std::scientific << std::setprecision(3) << err_max << std::endl;
    std::cout << "Среднеквадратичная ошибка: " << std::scientific << std::setprecision(3) << err_norm << std::endl;



    return 0;
}