#ifndef CLUSTER_H
#define CLUSTER_H

#include "curve.h"
#include "hashtable.h"
#include <vector>

using namespace std;

void clustering(vector<double>&, vector<const Curve*>&, vector<vector<int> >&, const char*, int, int);
void silhouette(const vector<const Curve*>&, vector<vector<int> >&, vector<double>&, const char*);

#endif
