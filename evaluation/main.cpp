#include <iostream>
#include "process_raw_data.h"
#include <thread>
#include <vector>
#include "word2vec.h"
#include "global_constants.h"
#include "median_of_distances.h"
#include "calculate_tweet_vector.h"
#include "tweet_tokenizer.h"
#include "graph.h"

extern "C" {
#include "html_decoder/entities.h"
}
#include "pairwise_distance.h"
#include "effective_edges.h"
#include "effective_hashtags.h"
#include <algorithm>
#include <fstream>

using namespace std;

int main() {
    cout << "cleaning " << sizeof(raw_data_file_names) / sizeof(raw_data_file_names[0]) << " files ..." << endl;
    initialize_tweet_tokenizer();
    vector<thread> v;
    for (auto file_path : raw_data_file_names) {
        auto absolute_file_path = new string((string) absolute_path_to_resources + file_path);
        thread th(process_raw_data, (*absolute_file_path).data());
        v.push_back(move(th));
    }
    word2vec_setup();
    for(auto &th : v)
        th.join();
    cout << "done cleaning" << endl;

    for (int i = 0; i < number_of_cal_tweet_vec_files; i++) {
        string input = (string)absolute_path_to_resources + cal_tweet_vec_inputs[i];
        string input_ids = input + ".ids";
        string output = (string)absolute_path_to_resources + cal_tweet_vec_outputs[i];
        string output_ids = output + ".ids";
        cout << "calculating tweet vectors" << endl;
        int n = calc_tweet_vec(input.data(), input_ids.data(), output.data(), output_ids.data());
        cout << "done" << endl;
        cout << "calculating pairwise distances" << endl;
        pairwise_distance(n, output.data());
        cout << "done" << endl;
        cout << "calculating sigma" << endl;
        double sigma = get_median_of_distances(n) / median_division_factor;
        cout << "sigma: " << sigma << " done" << endl;
        cout << "generating edges and writing them" << endl;
        auto tweet_edges = get_effective_tweet_edges(n, sigma);
        ifstream in_ids(output_ids);
        std::vector<int> id_determiner(n);
        for(auto &x : id_determiner)
            in_ids >> x;
        ofstream out((string)absolute_path_to_resources + "edges_c");
        for(auto edge : tweet_edges)
            out << id_determiner[get<0>(edge)] << " " << id_determiner[get<1>(edge)] << " " << get<2>(edge) << endl;
    }
    return 0;
}
