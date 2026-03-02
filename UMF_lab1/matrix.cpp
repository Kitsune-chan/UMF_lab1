#include "matrix.h"
#include "domain.h"
#include "utils.h"
#include <vector>
#include <cmath>

void build_problem(const std::vector<double>& x_coords,
    const std::vector<double>& y_coords,
    std::vector<double>& d0,
    std::vector<double>& d1,
    std::vector<double>& d2,
    std::vector<double>& d3,
    std::vector<double>& d4,
    std::vector<double>& b) {
    int Nx = x_coords.size();
    int Ny = y_coords.size();
    int N = Nx * Ny;
    d0.assign(N, 0.0); d1.assign(N, 0.0); d2.assign(N, 0.0);
    d3.assign(N, 0.0); d4.assign(N, 0.0); b.assign(N, 0.0);

    for (int j = 0; j < Ny; ++j) {
        double y = y_coords[j];
        double hy_prev = (j > 0) ? y - y_coords[j - 1] : 0.0;
        double hy_next = (j < Ny - 1) ? y_coords[j + 1] - y : 0.0;

        for (int i = 0; i < Nx; ++i) {
            double x = x_coords[i];
            int idx = j * Nx + i;

            double hx_prev = (i > 0) ? x - x_coords[i - 1] : 0.0;
            double hx_next = (i < Nx - 1) ? x_coords[i + 1] - x : 0.0;

            double beta, u_beta;
            int btype = boundary_type(x, y, beta, u_beta);

            if (btype == -1) {
                d0[idx] = 1.0;
                b[idx] = 0.0;
                continue;
            }

            double lam = lambda(x, y);
            double gam = gamma(x, y);

            double coef_xR = (hx_next > EPS) ? 2.0 / (hx_next * (hx_prev + hx_next)) : 0.0;
            double coef_xL = (hx_prev > EPS) ? 2.0 / (hx_prev * (hx_prev + hx_next)) : 0.0;
            double coef_yU = (hy_next > EPS) ? 2.0 / (hy_next * (hy_prev + hy_next)) : 0.0;
            double coef_yD = (hy_prev > EPS) ? 2.0 / (hy_prev * (hy_prev + hy_next)) : 0.0;

            // Вклад оператора Лапласа + γ
            d0[idx] = lam * (coef_xR + coef_xL + coef_yU + coef_yD) + gam;
            if (i > 0)   d2[idx] = -lam * coef_xL;
            if (i < Nx - 1) d1[idx] = -lam * coef_xR;
            if (j > 0)   d4[idx] = -lam * coef_yD;
            if (j < Ny - 1) d3[idx] = -lam * coef_yU;

            b[idx] = f_source(x, y);

            // Корректировка для граничных узлов
            if (btype == 1) {
                d0[idx] = 1.0;
                d1[idx] = d2[idx] = d3[idx] = d4[idx] = 0.0;
                b[idx] = u_exact(x, y);
            }
            else if (btype == 3) {
                // Производная по направлению внешней нормали (вверх) ∂u/∂n = ∂u/∂y
                // Уравнение: λ*(u - u_prev)/hy_prev + β*(u - u_beta) = 0 => (λ/hy_prev + β)*u - (λ/hy_prev)*u_prev = β*u_beta
                d0[idx] = lam / hy_prev + beta;
                d4[idx] = -lam / hy_prev; // связь с нижним соседом
                d1[idx] = d2[idx] = d3[idx] = 0.0;
                b[idx] = beta * u_beta;
            }
        }
    }
}