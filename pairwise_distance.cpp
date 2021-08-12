#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <math.h>
#include <fstream>
#include<thread>

double euclidean_distances(std::vector<int> x, std::vector<int> y) {
    double sum = 0;
    for (int i = 0; i < x.size(); ++i) {
        sum += pow(x[i] - y[i], 2);
    }
    return sum;
}

void calculate_bulk_distance(std::vector<std::vector<int>> X, std::vector<std::vector<int>> Y, int X_begin_index) {
    for (int i = 0; i < X.size(); ++i) {
        for (int j = 0; j < Y.size(); ++j) {
            double euclidean_distance;
            euclidean_distance = euclidean_distances(X[i], Y[j]);
            char J = '0' + j;
            char I = '0' + i;
            std::string file_name;
            file_name.push_back(I);
            file_name.push_back('_');
            file_name.push_back(J);
            std::ofstream output_file("pairwise_distance/" + file_name + ".txt");
            output_file << "i: " << i << "  j: " << j << "  euclidean_distance: " << euclidean_distance;
            output_file.close();
        }
    }

}


void pairwise_distance(std::vector<std::vector<int>> X, std::vector<std::vector<int>> Y) {
    int bulk_count = 8;
    int bulk_size = X.size() / bulk_count;
    if (bulk_size == 0)
        bulk_size = X.size();
    int offset = 0;
    std::vector<std::thread> v;
    while (offset < X.size()) {
        __gnu_cxx::__normal_iterator<std::vector<int> *, std::vector<std::vector<int>>> first = X.begin() + offset;
        __gnu_cxx::__normal_iterator<std::vector<int> *, std::vector<std::vector<int>>> last =
                X.begin() + offset + bulk_size;
        std::vector<std::vector<int>> newVec(first, last);
        std::thread thread_obj(calculate_bulk_distance, X, Y, offset);
        v.push_back(move(thread_obj));
        offset += bulk_size;
    }

    for (auto &th : v)
        th.join();
}

