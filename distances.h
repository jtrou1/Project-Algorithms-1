#ifndef DISTANCES_H
#define DISTANCES_H

#include <iostream>
#include <vector>
#include "curve.h"

using namespace std;

static Curve default_curve;
extern double **mem_distance;

void find_closest_point(vector<double>&, const vector<double>&, const vector<double>&, double);
double euclidean_distance_square(const vector<double>&, const vector<double>&);
void mean_point(vector<double> &mean, const vector<double> &pnt_1, const vector<double> &pnt_2);
double discrete_frechet_distance(const Curve&, const Curve&, Curve& = default_curve, bool = false);
double dynamic_time_wrapping(const Curve&, const Curve&);
double c_rmsd(const Curve&, const Curve&, const char*);
double compute_distance(const Curve &curve_1, const Curve &curve_2, const char*);

#endif
