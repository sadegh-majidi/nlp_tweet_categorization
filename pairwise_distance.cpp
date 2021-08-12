#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "global_constants.h"
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")


inline double euclidean_distances(const std::vector<double> &x, const std::vector<double> &y) {
    double sum = 0;
    for (int i = 0; i < x.size(); ++i) {
        sum += x[i] * y[i];
    }
    return -2 * sum;
}

void calculate_bulk_distance(const std::vector<std::vector<double>> &M,
                             const std::vector<double> cached_sum,
                             int X_begin_index, int X_end_index, std::string file_name) {
    std::ofstream output_file(pairwise_distance_output_directory_path + file_name);
    output_file << X_begin_index << " " << X_end_index << '\n';
    for (int i = X_begin_index; i < X_end_index; ++i) {
        for (int j = 0; j < i; ++j) {
            double euclidean_distance;
            euclidean_distance = euclidean_distances(M[i], M[j]) + cached_sum[i] + cached_sum[j];
            output_file << euclidean_distance << ' ';
        }
    }
    output_file.close();

}

auto load_all_vectors(const char* file_address) {
    std::ifstream in(file_address);
    int n;
    in >> n;

    std::vector<std::vector<double>> all_vectors(n, std::vector<double>());
    std::vector<double> cached_sum(n, 0);
    for(int i = 0; i < n; i++) {
        all_vectors[i].resize(VECTOR_COLUMNS);
        for(auto &x : all_vectors[i])
        {
            in >> x;
            cached_sum[i] += x * x;
        }
    }
    in.close();
    return std::make_pair(all_vectors, cached_sum);
}


void pairwise_distance(const char* file_address) {
    const auto [M, cached_sum] = load_all_vectors(file_address);
    int bulk_count = 8;
    int offset = 0, n = (int)M.size();
    long long cur = 0;
    long long each = 1LL * n * (n - 1) / 2 / bulk_count;
    std::vector<std::thread> v;
    for(int i = 0; i < bulk_count; i++) {
        int first = offset;
        while(offset < n && 1LL * offset * (offset - 1) / 2 - cur < each)
            offset++;
        cur = 1LL * offset * (offset - 1) / 2;
        std::string file_name;
        file_name += (char)('0' + i);
        std::thread thread_obj(calculate_bulk_distance, M, cached_sum,
                               first, offset,
                               file_name);
        v.push_back(move(thread_obj));
    }

    for (auto &th : v)
        th.join();
}
