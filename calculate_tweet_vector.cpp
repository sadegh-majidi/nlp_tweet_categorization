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
#include "stop_words.h"
#include "word2vec.h"

////////////////////////////////////////

std::string WORD_PATTERN;

std::string EMOTICONS = R"((?:[<>]?[:;=8][-o*']?[\)\]\([dDpP/:}{@|\\]|[\)\]\([dDpP/:}{@|\\][-o*']?[:;=8][<>]?|<3))";
//regex EMOTICONS(h);

std::regex emoticons(EMOTICONS);

std::string PHONE_NUMBER = R"((?:(?:\+?[01][*\-.\)]*)?(?:[\(]?\d{3}[ *\-.\)]*)?\d{3}[ *\-.\)]*\d{4}))";

std::string HTML_TAGS = "<[^>\\s]+>";

std::string ASCII_ARROWS = "[\\-]+>|<[\\-]+";

std::string TWITTER_USERNAME = "(?:@[\\w_]+)";

std::string TWITTER_HASHTAG = R"((?:\#+[\w_]+[\w\'_\-]*[\w_]+))";

std::string EMAIL_ADDRESS = R"([\w.+-]+@[\w-]+\.(?:[\w-]\.?)+[\w-])";

std::string WORD_WITH_DASH_AND_APOSTROPHE = R"((?:[^\W\d_](?:[^\W\d_]|['\-_])+[^\W\d_]))";

std::string NUMBERS = R"((?:[+\-]?\d+[,/.:-]\d+[+\-]?))";

std::string SIMPLE_WORD = "(?:[\\w_]+) ";

std::string ELLIPSIS = R"((?:\.(?:\s*\.){1,}) )";

std::string EXTRA = "(?:\\S) ";

std::string URLS = R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))";

std::string shorten_pattern = "(.)\\1{3,}";

std::string bad_pattern = "([^a-zA-Z0-9])\\1{3,}";


std::vector<std::string> WORD_RE{EMOTICONS, PHONE_NUMBER, HTML_TAGS, ASCII_ARROWS, TWITTER_HASHTAG, TWITTER_USERNAME,
                       EMAIL_ADDRESS, WORD_WITH_DASH_AND_APOSTROPHE, NUMBERS, SIMPLE_WORD, ELLIPSIS, EXTRA, URLS};


std::string reduce_lengthening(std::string text, const std::string &pattern) {
    std::regex repeat_regexp(pattern);
    using strmatch = std::match_results<std::string::iterator>;
    strmatch res;
    std::string result;
    int offset = 0;
    auto itr = text.begin();

    while (std::regex_search(itr, text.end(), res, repeat_regexp)) {
        //        std::cout << "[" << res[0] << "]" << std::endl;
        std::string t = res[0];
        result += text.substr(offset, res.position()) + t[0] + t[0] + t[0];
        offset += res.position() + res.length();
        itr += res.position() + res.length();
    }
    result += text.substr(offset, text.length());
    //    cout << text << "|" << '\n';
    //    cout << result << "|";
    return result;

}

std::vector<std::string> find_word(std::string text, const std::regex &WORD_REGEX) {
    std::smatch m;
    std::vector<std::string> result;
    while (std::regex_search(text, m, WORD_REGEX)) {
        result.push_back(m[0]);
        text = m.suffix().str();
    }
    return result;
}


std::vector<std::string> tweet_tokenizer(const std::regex &WORD, std::string text, bool reduce_len = false,
                               bool preserve_case = true) {
    if (reduce_len)
        text = reduce_lengthening(text, shorten_pattern);
    text = reduce_lengthening(text, bad_pattern);
    std::vector<std::string> words = find_word(text, WORD);
    if (!preserve_case) {
        for (int i = 0; i < words.size(); ++i) {
            std::cmatch cm;
            if (!regex_match(words[i], emoticons))
                for (int j = 0; j < words[i].length(); ++j) {
                    words[i][j] = tolower(words[i][j]);
                }
        }
    }
    return words;
}


//if (WORD_PATTERN.empty()) {
//    for (int i = 0; i < WORD_RE.size(); ++i) {
//        WORD_PATTERN += WORD_RE[i];
//        if (i != WORD_RE.size() - 1)
//            WORD_PATTERN += "|";
//    }
//}
//std::regex WORD_REGEX(WORD_PATTERN);

//std::vector<std::string> words = tweet_tokenizer(WORD_REGEX, tweet, true, false);
////////////////////////////////////////

bool is_stopword(const std::string& word) {
    for (auto & stopword : stopwords) {
        if (word == stopword)
            return true;
    }
    return false;
}

std::vector<double> calc_tweet_mat(const std::vector<std::string>& words) {
    std::vector<std::vector<double>> tweet_mat;

    for (auto & word : words) {
        if (!is_stopword(word)) {
            std::vector<double> v = get_vector(word);
            if (!v.empty())
                tweet_mat.push_back(v);
        }
    }

    if (!tweet_mat.empty()) {
        std::vector<double> vec;
        int tweet_mat_size = (int)tweet_mat.size();

        for (int i = 0; i < 300; i++) {
            double sum = 0.0;

            for (auto & entry : tweet_mat)
                sum += entry[i];

            vec.push_back(sum / tweet_mat_size);
        }

        return vec;
    } else
        return {};
}

void calc_tweet_vec(const char *file_address, const char *result_file_address) {
    std::ifstream infile(file_address);
    std::vector<std::vector<double>> tweet_vec;

    int n;
    infile >> n;

    for (int i = 0; i < n; i++) {
        std::vector<std::string> tweet;
        int words_size;
        infile >> words_size;

        for (int j = 0; j < words_size; j++) {
            std::string temp;
            infile >> temp;
            tweet.push_back(temp);
        }

        std::vector<double> v = calc_tweet_mat(tweet);
        if (!v.empty())
            tweet_vec.push_back(v);
    }
    infile.close();

    std::ofstream outfile(result_file_address);
    outfile << tweet_vec.size() << std::endl;
    for(auto &vec : tweet_vec) {
        for (int i = 0; i < 300; i++) {
            outfile << vec[i];
            if (i < 299)
                outfile << " ";
        }
        outfile << std::endl;
    }
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
