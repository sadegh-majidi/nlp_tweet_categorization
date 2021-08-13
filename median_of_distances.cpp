//
// Created by mahdi on 11.08.21.
//

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "median_of_distances.h"
#include "global_constants.h"
#include <algorithm>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include "cmath"


void calculate_min_max(int n, const std::string &file_path, std::vector<double> &s) {
    std::ifstream f(file_path);
    int x_start;
    int x_end;
    f >> x_start;
    f >> x_end;
    for (int i = x_start; i < x_end && i < n; i++)
        for (int j = 0; j < i; j++) {
            std::string result;
            f >> result;
            double value = std::sqrt(std::stod(result));
            s[i] += value;
            s[j] += value;
        }
}

double get_median_of_distances(int n) {
    n = (n + 1) / 2;
    std::vector<double> s[pairwise_distance_threads];
    std::vector<std::thread> v;
    for (int i = 0; i < pairwise_distance_threads; i++) {
        s[i].resize(n, 0);
        std::string file_path = (std::string) pairwise_distance_output_directory_path + (char) ('0' + i);
        std::thread thread_obj(calculate_min_max, n, file_path, std::ref(s[i]));
        v.push_back(move(thread_obj));
    }

    for (auto &th : v)
        th.join();

    for(int i = 1; i < pairwise_distance_threads; i++) {
        for(int j = 0; j < n; j++)
            s[0][j] += s[i][j];
        s[i].clear();
    }

    std::nth_element(s[0].begin(), s[0].begin() + n / 2, s[0].end());
    return s[0][n / 2] / n;
}
