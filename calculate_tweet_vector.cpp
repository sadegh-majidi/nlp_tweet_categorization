//
// Created by sadegh on 10.08.21.
//

#include <vector>
#include <string>
#include "calculate_tweet_vector.h"
#include "stop_words.h"
#include "word2vec.h"


bool is_stopword(const std::string& word) {
    for (auto & stopword : stopwords) {
        if (word == stopword)
            return true;
    }
    return false;
}

std::vector<double> calc_tweet_mat(const std::string& tweet) {
    std::vector<std::vector<double>> tweet_mat;
    std::vector<std::string> words = tokenize(tweet);
    for (auto & word : words) {
        if (!is_stopword(word)) {
            std::vector v = get_vector(word);
            if (!v.empty())
                tweet_mat.push_back(v);
        }
    }

    words = std::vector<std::string>();

    if (!tweet_mat.empty()) {
        std::vector<double> vec;

        for (int i = 0; i < 300; i++) {
            double sum = 0.0;

            for (auto & entry : tweet_mat)
                sum += entry[i];

            vec.push_back(sum / (int)tweet_mat.size());
        }

        return vec;
    } else
        return {};
}

std::vector<std::vector<double>> calc_tweet_vec(const std::vector<std::string>& cleaned_tweets) {
    std::vector<std::vector<double>> tweet_vec;
    for (auto & tweet : cleaned_tweets) {
        tweet_vec.push_back(calc_tweet_mat(tweet));
    }

    return tweet_vec;
}
