#ifndef LIST_H
#define LIST_H

#include <fstream>
#include "curve.h"

class List {
    class Node {
        public:
            Curve curve, grid_curve;
            Node *next;

            Node(const Curve&, const Curve&);
    };

    Node *head, *tail;

    public:
        List();
        ~List();
        void insert(const Curve&, const Curve&);
        void print_list() const;
        void search(vector<Curve>&, const Curve&, const Curve&, const char*, const char*, double, vector<bool>&, bool = true) const;
        void print_list_in_file(ofstream&) const;
        bool is_empty() const;
        void insert_list_to_cluster(vector<int>&) const;
        int get_first_id() const;
};

#endif
