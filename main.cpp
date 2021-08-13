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

    const char* inputs[] = {"near_washington_1015_1016_tweets.csv.2020-10-15.txt",
                            "near_washington_1016_1017_tweets.csv.2020-10-16.txt",
                            "near_washington_1017_1018_tweets.csv.2020-10-17.txt",
                            "near_washington_1018_1019_tweets.csv.2020-10-18.txt",
                            "near_washington_1019_1022_tweets.csv.2020-10-19.txt",
                            "near_washington_1019_1022_tweets.csv.2020-10-20.txt",
                            "near_washington_1019_1022_tweets.csv.2020-10-21.txt"};
    const char* outputs[] = {"2020-10-15.txt", "2020-10-16.txt", "2020-10-17.txt",
                             "2020-10-18.txt", "2020-10-19.txt", "2020-10-20.txt", "2020-10-21.txt"};

    calc_tweet_vec_all(inputs, outputs, 7);

    cout << get_median_of_distances(33025) << endl;
    return 0;
}
