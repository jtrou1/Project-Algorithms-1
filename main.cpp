#include <iostream>
#include <Eigen/Dense>
#include "file_functions.h"
#include "cluster.h"
#include "distances.h"
#include "help_functions.h"
#include "hashtable.h"

using namespace std;


int main(int argc, const char *argv[]) {
    ios::sync_with_stdio(false);
    srand(time(NULL));

    num_of_clusters = 1, global_k = 1, global_L = 1;

    const char *file_name = get_arguments(argv, argc, "-f", false);
    const char *type = get_arguments(argv, argc, "-t", false);
    const char *dist_func = get_arguments(argv, argc, "-d", false);
    
    if (!strcmp(type, "3")) {
        clock_t begin = clock();
        print_segments("athens.csv", "segment.csv");
        
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "Print Segments completed (" << elapsed_secs << " secs)" << endl; 

        return 0;
    }
    
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

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        
        cout << "C-RMSD/FRB completed (" << elapsed_secs << " secs)" << endl;
        print_file("crmsd.dat", clusters, silhouette_value, elapsed_secs); 

        // clear data
        silhouette_value = 0;
        clear_distances();

        // run CRMSD/DFT
        begin = clock();
        clustering(centroids, clusters, "C-RMSD/DFT", silhouette_value, 1, 2);
        
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        
        cout << "C-RMSD/DFT completed (" << elapsed_secs << " secs)" << endl; 
        print_file("frechet.dat", clusters, silhouette_value, elapsed_secs); 
    } else if (!strcmp(type, "2")) { // roads
        clock_t begin = clock();
        
        if (!strcmp(dist_func, "DFT")) {    
            clustering(centroids, clusters, "C-RMSD/DFT", silhouette_value, 1, 2);
            
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

            cout << "C-RMSD/DFT completed (" << elapsed_secs << " secs)" << endl; 
            print_file("kmedoid_ways_frechet.dat", clusters, silhouette_value, elapsed_secs); 
        } else if (!strcmp(dist_func, "DTW")) {    
            clustering(centroids, clusters, "C-RMSD/DTW", silhouette_value, 1, 2);
            
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            
            cout << "C-RMSD/DTW completed (" << elapsed_secs << " secs)" << endl; 
            print_file("kmedoid_ways_dtw.dat", clusters, silhouette_value, elapsed_secs); 
        }
        
        // clear data
        silhouette_value = 0;
        clear_distances();

        // run LSH
        begin = clock();
        int max_points = 0, dim = input_curves[0].get_dimension();
        double delta = 0.2;

        for (int i = 0; i < (int)input_curves.size(); ++i) {
            int value = input_curves[i].get_length();
            max_points = max(max_points, value);
        }
        
        for (int i = 0; i < max_points * global_k * dim; ++i) {
            vec_r.push_back(rand() % MAX_R);
        }
        
        int table_size = (int)input_curves.size() / 4 + 1;
        HashTable hashtable(table_size, 0);

        insert_curves_into_hashtables(hashtable, delta, "classic");
        silhouette_value = hashtable_silhouette(hashtable, clusters);
        
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "LSH completed (" << elapsed_secs << " secs)" << endl; 
        
        print_file("lsh_ways_clustering.dat", clusters, silhouette_value, elapsed_secs); 
    }

    free_distances();
    return 0;
}
