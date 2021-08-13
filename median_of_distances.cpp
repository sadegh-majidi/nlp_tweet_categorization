//
// Created by mahdi on 11.08.21.
//

#include <vector>
#include <string>
#include <fstream>
#include "median_of_distances.h"
#include "global_constants.h"
#include <algorithm>
#include <iostream>
#include <atomic>
#include <thread>
#include "cmath"
//#include "calculate_distances.h"

double get_dist(int x, int y) {
    return 0;
}

const int iterations = 100;

bool is_bigger_than_median(int n, double m) {
    int res = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            res += (get_dist(i, j) <= m);
    return res >= n * (n - 1) / 4;
}

class MultiFileReader {
public:
    explicit MultiFileReader(const std::vector<std::string> &_file_paths) {
        file_paths = _file_paths;
        current_in = std::ifstream(file_paths[0]);
    }

    void get_next_string(std::string &result) {
        current_in >> result;
        if (current_in.eof())
            current_in = std::ifstream(file_paths[++current_index]), get_next_string(result);
    }

private:
    std::vector<std::string> file_paths;
    int current_index = 0;
    std::ifstream current_in;
};


void calculate_min_max(int n, const std::string &file_path, std::vector<double> &s, std::atomic<double> &min,
                       std::atomic<double> &max) {
    std::ifstream f(file_path);
    int x_start;
    int x_end;
    f >> x_start;
    f >> x_end;
    for (int i = x_start; i < x_end; i++)
        for (int j = 0; j < i; j++) {
            std::string result;
            f >> result;
            double value = std::sqrt(std::stod(result));
            s[i] += value;
            s[j] += value;
            min = std::min<double>(min, value);
            max = std::max<double>(max, value);
        }
}

double get_median_of_distances(int n) {
//    std::vector<std::string> file_paths;
//    for (int i = 0; i < pairwise_distance_threads; i++)
//        file_paths.emplace_back((std::string) pairwise_distance_output_directory_path + (char) ('0' + i));
//    MultiFileReader mfr(file_paths);
//    n = (n + 1) / 2;
    std::vector<double> s(n, 0);
    std::atomic<double> min = 1e9;
    std::atomic<double> max = -1e9;
    std::vector<std::thread> v;
    for (int i = 0; i < pairwise_distance_threads; i++) {
        std::string file_path = (std::string) pairwise_distance_output_directory_path + (char) ('0' + i);
        std::thread thread_obj(calculate_min_max, n, file_path, std::ref(s), std::ref(min), std::ref(max));
        v.push_back(move(thread_obj));
    }

    for (auto &th : v)
        th.join();
//    for (int i = 0; i < n; i++)
//        for (int j = 0; j < i; j++) {
//            std::string result;
//            mfr.get_next_string(result);
//            double value = std::sqrt(std::stod(result));
//            s[i] += value;
//            s[j] += value;
//            min = std::min(min, value);
//            max = std::max(max, value);
//        }

    std::cout << min << " " << max << std::endl;
    std::nth_element(s.begin(), s.begin() + n / 2, s.end());
    return s[n / 2] / n;
}
