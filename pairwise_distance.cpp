#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include<thread>
#include <math.h>

std::vector<std::vector<double>> M;

double euclidean_distances(std::vector<double> x, std::vector<double> y) {
    double sum = 0;
    for (int i = 0; i < x.size(); ++i) {
        sum += (x[i] - y[i]) * (x[i] - y[i]);
    }
    return sum;
}

void calculate_bulk_distance(int X_begin_index, int X_end_index, std::string file_name) {
    std::ofstream output_file("pairwise_distance/" + file_name + ".txt");
    for (int i = X_begin_index; i < X_end_index; ++i) {
        for (int j = 0; j < M.size(); ++j) {
            double euclidean_distance;
            euclidean_distance = euclidean_distances(M[i], M[j]);
            output_file << i << " " << j << " " << euclidean_distance << '\n';
        }
    }
    output_file.close();

}


void pairwise_distance() {
    int bulk_count = 1;
    int bulk_size = M.size() / bulk_count;
    if (bulk_size == 0)
        bulk_size = M.size();
    int offset = 0;
    std::vector<std::thread> v;
    while (offset < M.size()) {
        char I = '0' + offset / bulk_size;
        std::string file_name;
        file_name.push_back(I);
        std::thread thread_obj(calculate_bulk_distance, offset, std::min(offset + bulk_size, (int) M.size()),
                               file_name);
        v.push_back(move(thread_obj));
        offset += bulk_size;
    }

    for (auto &th : v)
        th.join();
}
