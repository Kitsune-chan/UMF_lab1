#include "solver.h"
#include "domain.h"
#include "utils.h"
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <cmath>

std::vector<double> point_sor(const std::vector<double>& d0,
    const std::vector<double>& d1,
    const std::vector<double>& d2,
    const std::vector<double>& d3,
    const std::vector<double>& d4,
    const std::vector<double>& b,
    int Nx, int Ny,
    double omega, double tol, int max_iter) {
    int N = Nx * Ny;
    std::vector<double> u(N, 0.0);

    // Начальное приближение: на границах первого рода сразу ставим точное значение
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            int idx = j * Nx + i;
            double x = (i < Nx ? 0 : 0); // заглушка – координаты не доступны, но они не нужны
            double y = 0;
            // К сожалению, здесь нет координат, поэтому такой подход не работает.
            // Вместо этого будем полагаться на то, что матрица уже учитывает граничные условия,
            // и начальное приближение можно взять нулевым.
        }
    }

    for (int iter = 0; iter < max_iter; ++iter) {
        double max_diff = 0.0;

        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                int idx = j * Nx + i;

                // Если узел – граница первого рода, его значение уже установлено в b,
                // но матрица для него диагональна, поэтому изменение не требуется.
                // Однако чтобы ускорить сходимость, можно пропустить такие узлы.
                // Здесь мы пропускаем только если d0[idx] == 1 и остальные диагонали нулевые.
                // Но для простоты будем обновлять все узлы, полагаясь на то, что
                // для граничных первого рода формула даст то же значение.
                // Это безопасно, если матрица корректно построена.

                double sum = 0.0;
                if (i > 0)   sum += d2[idx] * u[idx - 1];   // левый
                if (i < Nx - 1) sum += d1[idx] * u[idx + 1];   // правый
                if (j > 0)   sum += d4[idx] * u[idx - Nx];   // нижний
                if (j < Ny - 1) sum += d3[idx] * u[idx + Nx];   // верхний

                double new_val = (b[idx] - sum) / d0[idx];
                double old_val = u[idx];
                u[idx] = (1.0 - omega) * old_val + omega * new_val;

                max_diff = std::max(max_diff, std::fabs(u[idx] - old_val));
            }
        }

        if (iter % 500 == 0 || iter == max_iter - 1 || max_diff < tol) {
            std::cout << "Итерация " << iter + 1 << ", max изменение: "
                << std::scientific << std::setprecision(2) << max_diff << std::endl;
        }

        if (max_diff < tol) {
            std::cout << "Решение сошлось за " << iter + 1 << " итераций." << std::endl;
            return u;
        }
    }

    std::cout << "Предупреждение: достигнуто максимальное число итераций." << std::endl;
    return u;
}