#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include "file_functions.h"
#include "help_functions.h"

using namespace std;

typedef pair<double, double> PDD;

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

void print_segments(const char *init_csv, const char *final_csv) {
    ifstream init_file(init_csv);
    ofstream final_file(final_csv);
    int way_id, int_id = 0, dim = 2;
    double curvature;
    char chr;
    vector<double> point(dim);
    map<PDD, int> mem_points;

    while (init_file >> way_id) {
        init_file >> chr;
        while (init_file >> chr && chr >= 'a' && chr <= 'z') {} // read 'road type' and 'comma'

        while (1) {
            for (int i = 0; i < dim; ++i) {
                init_file >> point[i] >> chr;
            }

            if (!mem_points.count(make_pair(point[0], point[1]))) {
                mem_points[make_pair(point[0], point[1])] = 1;
            } else {
                ++mem_points[make_pair(point[0], point[1])];
            }

            if (init_file.eof()) {
                break;
            }

            if (chr != ',') {
                init_file.seekg(-1, init_file.cur);
                break;
            }
        }
    }

    init_file.clear();
    init_file.seekg(0, init_file.beg);

    while (init_file >> way_id) {
        init_file >> chr;
        while (init_file >> chr && chr >= 'a' && chr <= 'z') {} // read 'road type' and 'comma'

        Curve curve;
        curvature = 0;

        while (1) {
            for (int i = 0; i < dim; ++i) {
                init_file >> point[i] >> chr;
            }

            if (curve.get_length() >= 3) {
                double x0 = curve.get_coord_point(0, curve.get_length() - 3);
                double y0 = curve.get_coord_point(1, curve.get_length() - 3);
                double x1 = curve.get_coord_point(0, curve.get_length() - 2);
                double y1 = curve.get_coord_point(1, curve.get_length() - 2);
                double x2 = curve.get_coord_point(0, curve.get_length() - 1);
                double y2 = curve.get_coord_point(1, curve.get_length() - 1);
                double x3 = point[0];
                double y3 = point[1];

                double circ_rad_1 = circumradius(x0, y0, x1, y1, x2, y2);
                double circ_rad_2 = circumradius(x1, y1, x2, y2, x3, y3);

                curvature = min(circ_rad_1, circ_rad_2);
            }

            if (curve.is_empty() || curve.get_last_point() != point) { // remove duplicates
                if (curvature > 0.05) {
                    final_file << int_id++ << "," << way_id << ",";

                    vector<double> keep_pnt = curve.get_last_point();
                    curve.remove_last_point();

                    print_curve(final_file, curve, dim);

                    curvature = 0;
                    curve.clear_curve();

                    curve.insert_point(keep_pnt);
                } else if (mem_points[make_pair(point[0], point[1])] >= 2 && curve.get_length() > 10) {
                    final_file << int_id++ << "," << way_id << ",";
                    print_curve(final_file, curve, dim);

                    curvature = 0;
                    curve.clear_curve();
                    --mem_points[make_pair(point[0], point[1])];
                }

                curve.insert_point(point);
            }

            if (init_file.eof()) {
                break;
            }

            if (chr != ',') {
                init_file.seekg(-1, init_file.cur);
                break;
            }
        }

        if (curve.get_length() >= 2) {
            final_file << int_id++ << "," << way_id << ",";
            print_curve(final_file, curve, dim);
        }
    }

    init_file.close();
    final_file.close();
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

void print_curve(ofstream &final_file, const Curve &curve, int dim) {
    final_file << curve.get_length() << ",";

    for (int i = 0; i < curve.get_length(); ++i) {
        for (int j = 0; j < dim; ++j) {
            final_file << curve.get_coord_point(j, i);

            if (i < curve.get_length() - 1 || j < dim - 1) {
                final_file << ",";
            }
        }
    }

    final_file << "\n";
}
