#ifndef CLUSTER_H
#define CLUSTER_H

#include "curve.h"
#include "hashtable.h"
#include <vector>

using namespace std;

void clustering(vector<const Curve*>&, vector<vector<int> >&, const char*, double, int, int);
void silhouette(const vector<const Curve*>&, vector<vector<int> >&, vector<double>&, const char*);

#endif
