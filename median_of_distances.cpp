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
#include "cmath"
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

class MultiFileReader {
public:
    explicit MultiFileReader(const std::vector<std::string> &_file_paths) {
        file_paths = _file_paths;
        current_in = std::ifstream(file_paths[0]);
    }
    void get_next_string(std::string &result) {
        current_in >> result;
        if(current_in.eof())
            current_in = std::ifstream(file_paths[++current_index]), get_next_string(result);
    }
private:
    std::vector<std::string> file_paths;
    int current_index = 0;
    std::ifstream current_in;
};

double get_median_of_distances(int n) {
    std::vector<std::string> file_paths;
    for(int i = 0; i < pairwise_distance_threads; i++)
        file_paths.emplace_back((std::string)pairwise_distance_output_directory_path + (char)('0' + i));
    MultiFileReader mfr(file_paths);
    n = (n + 1) / 2;
    std::vector<double> s(n, 0);
    double min = 1e9, max = -1e9;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < i; j++) {
            std::string result;
            mfr.get_next_string(result);
            double value = std::sqrt(std::stod(result));
            s[i] += value;
            s[j] += value;
            min = std::min(min, value);
            max = std::max(max, value);
        }

    std::cout << min << " " << max << std::endl;
    std::nth_element(s.begin(), s.begin() + n / 2, s.end());
    return s[n / 2] / n;
}
