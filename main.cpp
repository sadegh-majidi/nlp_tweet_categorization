#include <iostream>
#include "process_raw_data.h"
#include <sys/stat.h>
#include <thread>
#include <vector>
#include "word2vec.h"
#include "calculate_tweet_vector.h"
#include "tweet_tokenizer.h"
extern "C" {
#include "html_decoder/entities.h"
}

using namespace std;

const char *absolute_path_to_resources = "/home/mahdi/Desktop/nc-final-project/files_twitter/";

const char* data_path[][2] = {
        {"near_washington_1015_1016_tweets.csv", "cleaned_near_washington_1015_1016_tweets.csv"},
        {"near_washington_1016_1017_tweets.csv", "cleaned_near_washington_1016_1017_tweets.csv"},
        {"near_washington_1017_1018_tweets.csv", "cleaned_near_washington_1017_1018_tweets.csv"},
        {"near_washington_1018_1019_tweets.csv", "cleaned_near_washington_1018_1019_tweets.csv"},
        {"near_washington_1019_1022_tweets.csv", "cleaned_near_washington_1019_1022_tweets.csv"}
};

int main() {
    cout << "cleaning " << sizeof(data_path) / sizeof(data_path[0]) << " files ..." << endl;
    initialize_tweet_tokenizer();
    vector<thread> v;
    for (auto[file_path, dest_path] : data_path) {
        auto absolute_file_path = new string((string) absolute_path_to_resources + file_path);
//        auto absolute_dest_path = new string((string) absolute_path_to_resources + dest_path);
//        struct stat info{};
//        if (stat((*absolute_dest_path).data(), &info) != 0 || info.st_mode & S_IFDIR) {
        thread th(process_raw_data, (*absolute_file_path).data());
        v.push_back(move(th));
//        }
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

    return 0;
}
