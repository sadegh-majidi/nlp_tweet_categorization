//
// Created by mahdi on 11.08.21.
//

#include "median_of_distances.h"
//#include "calculate_distances.h"

double get_dist(int x, int y) {
    return 0;
}

const int iterations = 100;

bool is_bigger_than_median(int n, double m) {
    int res = 0;
    for(int i = 0; i < n; i++)
        for(int j = i + 1; j < n; j++)
            res += (get_dist(i, j) <= m);
    return res >= n * (n - 1) / 4;
}

double get_median_of_distances_by_binary_search(int n) {
    double l = 0, r = 1000;
    for(int _ = 0; _ < iterations; _++) {
        double m = (l + r) / 2;
        if(is_bigger_than_median(n, m))
            r = m;
        else
            l = m;
    }
    return r;
}

double get_median_of_distances(int n) {
    return get_median_of_distances_by_binary_search(n);
}
