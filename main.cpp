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

using namespace std;

int main() {
    cout << "cleaning " << sizeof(raw_data_file_names) / sizeof(raw_data_file_names[0]) << " files ..." << endl;
    initialize_tweet_tokenizer();
    vector<thread> v;
    for (auto[file_path, dest_path] : raw_data_file_names) {
        auto absolute_file_path = new string((string) absolute_path_to_resources + file_path);
        thread th(process_raw_data, (*absolute_file_path).data());
        v.push_back(move(th));
    }
    word2vec_setup();
    for(auto &th : v)
        th.join();
    cout << "done cleaning" << endl;

    calc_tweet_vec_all(cal_tweet_vec_inputs, cal_tweet_vec_outputs, number_of_cal_tweet_vec_files);

    cout << get_median_of_distances(33025) << endl;
    return 0;
}
