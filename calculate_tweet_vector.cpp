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
#include "global_constants.h"

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
                for (int i = 0; i < VECTOR_COLUMNS; i++)
                    vec[i] += v[i];
        }
        v.clear();
    }

    if (!vec.empty()) {
        for (int i = 0; i < VECTOR_COLUMNS; i++)
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

        tweet.resize(words_size, "");
        for (auto &word : tweet)
            infile >> word;

        std::vector<double> v = calc_tweet_mat(tweet);
        if (!v.empty()) {
            for (int j = 0; j < VECTOR_COLUMNS; j++) {
                outfile << v[j];
                if (j + 1 < VECTOR_COLUMNS)
                    outfile << " ";
            }
            outfile << std::endl;
        }
        tweet.clear();
        v.clear();
    }
    infile.close();
    outfile.close();

}
