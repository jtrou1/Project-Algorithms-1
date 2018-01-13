#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <vector>
#include "curve.h"
#include "hashtable.h"

void read_file(const char*, const char*);
void print_segments(const char*, const char*);
void print_file(const char*, const vector<vector<int> >&, double, double);
void print_curve(ofstream&, const Curve&, int);

#endif
