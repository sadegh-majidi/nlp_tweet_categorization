#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include "csv-parser/single_include/csv.hpp"
#include "tweet_tokenizer.h"
#include "stop_words.h"

using namespace std;

unordered_set<char> st;

bool is_stopword(const std::string& word) {
    for (auto & stopword : stopwords) {
        if (word == stopword)
            return true;
    }
    return false;
}

std::vector<std::string> clean_content(string_view content) {
    static const char arr[] = {'#', '(', ')', ':', '\n', '\r', '!', '"', '$', '-', '&', '.', ':', '\'', ';', '*'};
    static const regex tag_regex("@\\w+");
    static const regex num_regex("\\d+");
    static const regex url_regex(R"((https?):((//)|(\\\\))+([\w\d:#@%/;$()~_?\+-=\\\.&](#!)?)*)");
    if (st.empty()) {
        for (char i : arr)
            st.insert(i);
    }
    string cleaned;
    for (char ch : content)
        if (st.find(ch) == st.end())
            cleaned += ch;
        else
            cleaned += ' ';
    cleaned = regex_replace(cleaned, url_regex, "");
    cleaned = regex_replace(cleaned, tag_regex, "");
    cleaned = regex_replace(cleaned, num_regex, "");

    std::vector<std::string> tokenized_cleaned = tweet_tokenizer(cleaned, true, false);

    return tokenized_cleaned;
}

vector<string> hashtag_tokenizer(string_view s, int min_size) {
    char delimiter = ',';
    vector<string> tokenized;
    string token;
    int last = -1, i;
    for (i = 0; i < (int) s.size(); i++)
        if (s[i] == delimiter) {
            token = s.substr(last + 1, i - last - 1);
            if (token.length() > min_size)
                tokenized.push_back(token);
            last = i;
        }
    if (i - last - 1 > min_size) {
        token = s.substr(last + 1, i - last - 1);
        tokenized.push_back(token);
    }
    return tokenized;
}


void process_raw_data(const char *file_address, const char *result_file_address) {
    static const vector<string> arr = {"date", "content", "id", "hashtag"};
    csv::CSVReader in(file_address);

    for (auto &row : in) {
        bool language_is_en = row["lang"].get<string_view>() == "en";
        bool has_hashtags = row["hashtag"].get<string_view>().size() > 2;
        if (language_is_en && has_hashtags) {
            std::vector<std::string> content = clean_content(row["content"].get<string_view>());
            //TODO: write tokens to file and eliminate stopwords here
        }
    }
}
