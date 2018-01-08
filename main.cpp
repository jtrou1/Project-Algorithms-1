#include <iostream>
#include <Eigen/Dense>
#include "file_functions.h"
#include "cluster.h"
#include "distances.h"
#include "help_functions.h"

using namespace std;

int main(int argc, const char *argv[]) {
    ios::sync_with_stdio(false);
    srand(time(NULL));

    num_of_clusters = 1;
    global_k = 2;
    global_L = 3;

    const char *file_name = get_arguments(argv, argc, "-f", false);
    read_file(file_name);
    cout << "Read file" << endl;
    
    initialize_distances();

    vector<const Curve*> centroids(num_of_clusters);
    vector<vector<int> > clusters(num_of_clusters);
    double silhouette_value = 0;
    
    // run C-RMSD/FRB
    clustering(centroids, clusters, "C-RMSD/FRB", silhouette_value, 1, 2);
    print_file("crmsd.dat", clusters, silhouette_value); 
    cout << "Ran C-RMSD/FRB" << endl;
    
    // clear data
    silhouette_value = 0;
    clear_distances();

    // run CRMSD/DFT
    clustering(centroids, clusters, "C-RMSD/DFT", silhouette_value, 1, 2);
    print_file("frechet.dat", clusters, silhouette_value); 
    cout << "Ran C-RMSD/DFT" << endl;
    
    free_distances();
    return 0;
}
