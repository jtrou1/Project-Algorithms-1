#include <iostream>
#include <Eigen/Dense>
#include "file_functions.h"
#include "cluster.h"
#include "distances.h"
#include "help_functions.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    srand(time(NULL));

    num_of_clusters = 2;
    global_k = 2;
    global_L = 3;

    read_file("input.txt");

    mem_distance = new double*[(int)input_curves.size()];

    for (int i = 0; i < (int)input_curves.size(); ++i) {
        mem_distance[i] = new double[(int)input_curves.size()];

        for (int j = 0; j < (int)input_curves.size(); ++j) {
            mem_distance[i][j] = -1;
        }
    }

    vector<const Curve*> centroids(num_of_clusters);
    vector<vector<int> > clusters(num_of_clusters);
    double silhouette = 0;

    clustering(centroids,clusters, "C-RMSD", silhouette, 1, 2);

    return 0;
}
