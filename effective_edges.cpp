//
// Created by mahdi on 13.08.21.
//

#include <vector>
#include <thread>
#include <fstream>
#include <cmath>
#include "effective_hashtags.h"
#include "global_constants.h"


void calculate_effective_edges(int n, const double &sigma, const std::string &file_path, std::vector<std::tuple<int, int, double>> *edges) {
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
            if(value <= sigma) edges->emplace_back(i, j, value);
        }
}


std::vector<std::tuple<int, int, double>> get_effective_tweet_edges(int n, double sigma) {
    std::vector<std::tuple<int, int, double>> edges[pairwise_distance_threads];
    std::vector<std::thread> v;
    for (int i = 0; i < pairwise_distance_threads; i++) {
        std::string file_path = (std::string) pairwise_distance_output_directory_path + (char) ('0' + i);
        std::thread thread_obj(calculate_effective_edges, n, std::ref(sigma), std::ref(file_path), &edges[i]);
        v.push_back(move(thread_obj));
    }

    for (auto &th : v)
        th.join();

    for(int i = 1; i < pairwise_distance_threads; i++) {
        for(auto &x : edges[i])
            edges[0].push_back(std::move(x));
        edges[i].clear();
    }

    return edges[0];
}


