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
    const char *type = get_arguments(argv, argc, "-t", false);
    const char *dist_func = get_arguments(argv, argc, "-d", false);
    read_file(file_name, type);
    cout << "Read file completed" << endl;
    
    initialize_distances();

    vector<const Curve*> centroids(num_of_clusters);
    vector<vector<int> > clusters(num_of_clusters);
    double silhouette_value = 0;
    
    if (!strcmp(type, "1")) { // particles
        // run C-RMSD/FRB
        clock_t begin = clock();
        clustering(centroids, clusters, "C-RMSD/FRB", silhouette_value, 1, 2);
        print_file("crmsd.dat", clusters, silhouette_value); 

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "C-RMSD/FRB completed (" << elapsed_secs << " secs)" << endl;

        // clear data
        silhouette_value = 0;
        clear_distances();

        // run CRMSD/DFT
        begin = clock();
        clustering(centroids, clusters, "C-RMSD/DFT", silhouette_value, 1, 2);
        print_file("frechet.dat", clusters, silhouette_value); 
        
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "C-RMSD/DFT completed (" << elapsed_secs << " secs)" << endl; 
    } else if (!strcmp(type, "2")) { // roads
        clock_t begin = clock();
        
        if (!strcmp(dist_func, "DFT")) {    
            clustering(centroids, clusters, "C-RMSD/DFT", silhouette_value, 1, 2);
            print_file("kmeans_ways_frechet.dat", clusters, silhouette_value); 
            cout << "C-RMSD/DFT completed"; 
        } else if (!strcmp(dist_func, "DTW")) {    
            clustering(centroids, clusters, "C-RMSD/DTW", silhouette_value, 1, 2);
            print_file("kmeans_ways_dtw.dat", clusters, silhouette_value); 
            cout << "C-RMSD/DTW completed"; 
        }
            
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << " (" << elapsed_secs << " secs)" << endl;
    }

    free_distances();
    return 0;
}
