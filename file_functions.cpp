#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include "file_functions.h"

using namespace std;

void read_file(const char *file_name) {
    ifstream file(file_name);
    vector<double> point;
    int num_conform, num_points, dim = 3, int_id = 0;
    
    file >> num_conform >> num_points;
    point.resize(dim);
    
    for (int i = 1; i <= num_conform; ++i) {
        Curve curve("", int_id++, dim);

        for (int j = 1; j <= num_points; ++j) {
            for (int k = 0; k < dim; ++k) {
                file >> point[k];
            }

            if (!curve.is_empty() && curve.get_last_point() == point) { // remove duplicates
                continue;
            }

            curve.insert_point(point);
        }
        
        input_curves.push_back(curve);
    }

    file.close();
}
