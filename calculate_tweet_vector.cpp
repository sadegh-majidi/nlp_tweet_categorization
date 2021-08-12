//
// Created by sadegh on 10.08.21.
//

#include <vector>
#include <string>
#include <thread>
#include <sys/stat.h>
#include <regex>
#include <fstream>
#include <iostream>
#include "calculate_tweet_vector.h"
#include "word2vec.h"

std::vector<double> calc_tweet_mat(const std::vector<std::string>& words) {
    int size = 0;
    std::vector<double> vec;

    for (auto & word : words) {
        std::vector<double> v = get_vector(word);
        if (!v.empty()) {
            size++;

            if (vec.empty())
                vec = v;
            else
                for (int i = 0; i < 300; i++)
                    vec[i] += v[i];
        }
    }

    if (!vec.empty()) {
        for (int i = 0; i < 300; i++)
            vec[i] /= size;

        return vec;
    } else
        return {};
}

void calc_tweet_vec(const char *file_address, const char *result_file_address) {
    std::ifstream infile(file_address);
    std::ofstream outfile(result_file_address);

    while (!infile.eof()) {
        std::vector<std::string> tweet;
        int words_size;
        infile >> words_size;

        for (int j = 0; j < words_size; j++) {
            std::string temp;
            infile >> temp;
            tweet.push_back(temp);
        }

        std::vector<double> v = calc_tweet_mat(tweet);
        if (!v.empty()) {
            for (int j = 0; j < 300; j++) {
                outfile << v[j];
                if (j < 299)
                    outfile << " ";
            }
            outfile << std::endl;
        }
    }
    infile.close();
    outfile.close();

}

void calc_tweet_vec_all(const char* input_data_path[], const char* output_data_path[], int size) {
    const char *absolute_path_to_resources = "/home/sadegh/Desktop/";

    std::vector<std::thread> v;
    for (int i = 0; i < size; i++) {
        auto absolute_file_path = new std::string((std::string) absolute_path_to_resources + input_data_path[i]);
        auto absolute_dest_path = new std::string((std::string) absolute_path_to_resources + output_data_path[i]);
        struct stat info{};
        if (stat((*absolute_dest_path).data(), &info) != 0 || info.st_mode & S_IFDIR) {
            std::thread th(calc_tweet_vec, (*absolute_file_path).data(), (*absolute_dest_path).data());
            v.push_back(move(th));
        }
    }

    for(auto &th : v)
        th.join();
}
