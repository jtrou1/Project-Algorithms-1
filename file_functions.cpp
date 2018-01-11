#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "file_functions.h"

using namespace std;

void read_file(const char *file_name, const char *type) {
    ifstream file(file_name);
    vector<double> point;
    int num_conform, num_points, dim;
    int way_id, int_id = 0;
    char chr;
    
    if (!strcmp(type, "1")) { // particles
        dim = 3;
        point.resize(dim);
        
        file >> num_conform >> num_points;
        
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
    } else if (!strcmp(type, "2")) { // roads
        dim = 2;
        point.resize(dim);

        while (file >> int_id) {
            file >> chr >> way_id >> chr >> num_points >> chr;
            Curve curve("", int_id, dim);

            for (int i = 1; i <= num_points; ++i) {
                for (int j = 0; j < dim; ++j) {
                    file >> point[j];

                    if (i < num_points || j < dim - 1) {
                        file >> chr;
                    }
                }

                if (!curve.is_empty() && curve.get_last_point() == point) { // remove duplicates
                    continue;
                }

                curve.insert_point(point);
            }
            
            input_curves.push_back(curve);
        }
    }

    file.close();
}

void print_file(const char *file_name, const vector<vector<int> > &clusters, double silhouette_value) {
    ofstream file(file_name);
    int non_empty_clusters = 0;

    for (int i = 0; i < (int)clusters.size(); ++i) {
        if (!clusters[i].empty()) {
            ++non_empty_clusters;
        }
    }

    file << "k: " << non_empty_clusters << "\n";
    file << "s: " << silhouette_value << "\n";
    
    for (int i = 0; i < (int)clusters.size(); ++i) {
        if (clusters[i].empty()) {
            continue;
        }

        for (int j = 0; j < (int)clusters[i].size() - 1; ++j) {
            file << clusters[i][j] << " ";
        }
            
        file << clusters[i][(int)clusters[i].size() - 1] << "\n";
    }

    file.close();
}
