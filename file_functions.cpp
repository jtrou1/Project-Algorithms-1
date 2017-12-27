#include <iostream>
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

void print_file(const char *file_name, const vector<vector<int> > &clusters, double silhouette_value, int k) {
    ofstream out_file(file_name);
    
    out_file << "k: " << k << "\n";
    out_file << "s: " << silhouette_value << "\n";
    
    for (int i = 0; i < (int)clusters.size(); ++i) {
        for (int j = 0; j < (int)clusters[i].size() - 1; ++j) {
            out_file << clusters[i][j] << " ";
        }
            
        out_file << clusters[i][(int)clusters[i].size() - 1] << "\n";
    }
}
