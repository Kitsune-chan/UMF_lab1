#include "domain.h"
#include "utils.h"
#include <cmath>


double x1_left = 0;
double x1_right = 3;
double y1_down = 0;
double y1_top = 2;

double x2_left = 1;
double x2_right = 2;
double y2_down = 2;
double y2_top = 3;

//   пример
//   основной прямоугольник: x ∈ [0, 3], y ∈ [0, 2]
//   верхняя перекладина:    x ∈ [1, 2], y ∈ [2, 3]
bool inside_domain(double x, double y) {

    bool in_main = (x >= x1_left - EPS && x <= x1_right + EPS && y >= y1_down - EPS && y <= y1_top + EPS);
    bool in_top = (x >= x2_left - EPS && x <= x2_right + EPS && y >= y2_down - EPS && y <= y2_top + EPS);
    return in_main || in_top;
}

// определяем тип границы
int boundary_type(double x, double y, double& beta, double& u_beta) {
    beta = 0.0; u_beta = 0.0;
    if (!inside_domain(x, y)) return -1;

    // Нижняя грань основного
    if (double_equal(y, y1_down) && x >= x1_left - EPS && x <= x1_right + EPS)
        return 1;
    // Левая грань основного
    if (double_equal(x, x1_left) && y >= y1_down - EPS && y <= y1_top + EPS)
        return 1;
    // Правая грань основного
    if (double_equal(x, x1_right) && y >= y1_down - EPS && y <= y1_top + EPS)
        return 1;
    // Верхняя грань основного (исключая участок перекладины)
    if (double_equal(y, y1_top) &&
        ((x >= x1_left - EPS && x <= x2_left + EPS) ||
            (x >= x2_right - EPS && x <= x1_right + EPS)))
        return 1;
    // Левая грань перекладины (только для y > y2_down, т.е. выше стыка)
    if (double_equal(x, x2_left) && y > y2_down + EPS && y <= y2_top + EPS)
        return 1;
    // Правая грань перекладины
    if (double_equal(x, x2_right) && y > y2_down + EPS && y <= y2_top + EPS)
        return 1;
    // Верхняя грань перекладины (третье условие)
    if (double_equal(y, y2_top) && x >= x2_left - EPS && x <= x2_right + EPS) {
        beta = 1.0;
        u_beta = 1.0;
        return 3;
    }
    return 0; // внутренняя точка
}

double lambda(double /*x*/, double /*y*/) { return 1.0; }
double gamma(double  /*x*/, double /*y*/) { return 0.0; }

//0; 0; -2
//1; 0.25y; y*y - 14
double f_source(double x, double y) { return -2; }
double u_exact(double x, double y) { return y*y - 14; }