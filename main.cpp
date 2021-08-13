#include <iostream>
#include "process_raw_data.h"
#include <thread>
#include <vector>
#include "word2vec.h"
#include "global_constants.h"
#include "median_of_distances.h"
#include "calculate_tweet_vector.h"
#include "tweet_tokenizer.h"
extern "C" {
#include "html_decoder/entities.h"
}
#include "pairwise_distance.h"

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
        string output = (string)absolute_path_to_resources + cal_tweet_vec_outputs[i];
        calc_tweet_vec(input.data(), output.data());
        int n = pairwise_distance(output.data());
        cout << get_median_of_distances(n) << endl;
    }
    return 0;
}
