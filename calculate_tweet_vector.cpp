//
// Created by sadegh on 10.08.21.
//

#include <vector>
#include <string>
#include <regex>
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

////////////////////////////////////////

bool is_stopword(const std::string& word) {
    for (auto & stopword : stopwords) {
        if (word == stopword)
            return true;
    }
    return false;
}

std::vector<double> calc_tweet_mat(const std::string& tweet) {
    std::vector<std::vector<double>> tweet_mat;

    if (WORD_PATTERN.empty()) {
        for (int i = 0; i < WORD_RE.size(); ++i) {
            WORD_PATTERN += WORD_RE[i];
            if (i != WORD_RE.size() - 1)
                WORD_PATTERN += "|";
        }
    }
    std::regex WORD_REGEX(WORD_PATTERN);

    std::vector<std::string> words = tweet_tokenizer(WORD_REGEX, tweet, true, false);
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
