#ifndef DOMAIN_H
#define DOMAIN_H

// область и краевые условия
bool inside_domain(double x, double y);
int boundary_type(double x, double y, double& beta, double& u_beta);

// параметры уравнения
double lambda(double x, double y);
double gamma(double x, double y);
double f_source(double x, double y);
double u_exact(double x, double y);

#endif