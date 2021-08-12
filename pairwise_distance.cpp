#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include<thread>

double euclidean_distances(std::vector<double> x, std::vector<double> y) {
    double sum = 0;
    for (int i = 0; i < x.size(); ++i) {
        sum += (x[i] - y[i]) * (x[i] - y[i]);
    }
    return sum;
}

void calculate_bulk_distance(std::vector<std::vector<double>> X, std::vector<std::vector<double>> Y, int X_begin_index,
                             std::string file_name) {
    std::ofstream output_file("pairwise_distance/" + file_name + ".txt");
    for (int i = 0; i < X.size(); ++i) {
        for (int j = 0; j < Y.size(); ++j) {
            double euclidean_distance;
            euclidean_distance = euclidean_distances(X[i], Y[j]);
            output_file << i + X_begin_index << " " << j << " " << euclidean_distance << '\n';
        }
    }
    output_file.close();

}


void pairwise_distance(std::vector<std::vector<double>> X, std::vector<std::vector<double>> Y) {
    int bulk_count = 2;
    int bulk_size = X.size() / bulk_count;
    if (bulk_size == 0)
        bulk_size = X.size();
    int offset = 0;
    std::vector<std::thread> v;
    while (offset < X.size()) {
        auto first = X.begin() + offset;
        auto last = X.begin() + offset + bulk_size;
        std::vector<std::vector<double>> newVec(first, last);
        char I = '0' + offset / bulk_size;
        std::string file_name;
        file_name.push_back(I);
        std::thread thread_obj(calculate_bulk_distance, newVec, Y, offset, file_name);
        v.push_back(move(thread_obj));
        offset += bulk_size;
    }

    for (auto &th : v)
        th.join();
}
