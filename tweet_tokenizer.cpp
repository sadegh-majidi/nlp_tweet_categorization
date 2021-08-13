#include <string>
#include <vector>
#include <regex>
#include <iostream>

extern "C" {
#include "html_decoder/entities.h"
}
using namespace std;


regex WORD_REGEX;

string EMOTICONS = R"((?:[<>]?[:;=8][-o*']?[\)\]\([dDpP/:}{@|\\]|[\)\]\([dDpP/:}{@|\\][-o*']?[:;=8][<>]?|<3))";

regex emoticons(EMOTICONS);

string PHONE_NUMBER = R"((?:(?:\+?[01][*\-.\)]*)?(?:[\(]?\d{3}[ *\-.\)]*)?\d{3}[ *\-.\)]*\d{4}))";

string HTML_TAGS = "<[^>\\s]+>";

string ASCII_ARROWS = "[\\-]+>|<[\\-]+";

string TWITTER_USERNAME = "(?:@[\\w_]+)";

string TWITTER_HASHTAG = R"((?:\#+[\w_]+[\w\'_\-]*[\w_]+))";

string EMAIL_ADDRESS = R"([\w.+-]+@[\w-]+\.(?:[\w-]\.?)+[\w-])";

string WORD_WITH_DASH_AND_APOSTROPHE = R"((?:[^\W\d_](?:[^\W\d_]|['\-_])+[^\W\d_]))";

string NUMBERS = R"((?:[+\-]?\d+[,/.:-]\d+[+\-]?))";

string SIMPLE_WORD = "(?:[\\w_]+)";

string ELLIPSIS = R"((?:\.(?:\s*\.){1,}))";

string EXTRA = "(?:\\S)";

string URLS = R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))";

string shorten_pattern = "(.)\\1{3,}";

string bad_pattern = "([^a-zA-Z0-9])\\1{3,}";


vector<string> WORD_RE{URLS, PHONE_NUMBER, EMOTICONS, HTML_TAGS, ASCII_ARROWS, TWITTER_USERNAME, TWITTER_HASHTAG,
                       EMAIL_ADDRESS, WORD_WITH_DASH_AND_APOSTROPHE, NUMBERS, SIMPLE_WORD, ELLIPSIS};

void initialize_tweet_tokenizer() {
    string WORD_PATTERN;
    if (WORD_PATTERN.empty()) {
        WORD_PATTERN += "(";
        for (int i = 0; i < WORD_RE.size(); ++i) {
            WORD_PATTERN += WORD_RE[i];
            if (i != WORD_RE.size() - 1)
                WORD_PATTERN += "|";
        }
        WORD_PATTERN += ")";
    }
    WORD_REGEX = regex(WORD_PATTERN, std::regex_constants::icase);
}


string reduce_lengthening(string text, const string &pattern) {
    regex repeat_regexp(pattern);
    using strmatch = std::match_results<std::string::iterator>;
    strmatch res;
    string result;
    int offset = 0;
    auto itr = text.begin();

    while (std::regex_search(itr, text.end(), res, repeat_regexp)) {
        string t = res[0];
        result += text.substr(offset, res.position()) + t[0] + t[0] + t[0];
        offset += res.position() + res.length();
        itr += res.position() + res.length();
    }
    result += text.substr(offset, text.length());
    return result;

}

vector<string> find_word(string text) {
    vector<string> result;

    std::regex_iterator<std::string::iterator> rit(text.begin(), text.end(), WORD_REGEX);
    std::regex_iterator<std::string::iterator> rend;

    while (rit != rend) {
        result.push_back(rit->str());
        ++rit;
    }

    return result;

}


vector<string> tweet_tokenizer(string text, bool reduce_len = false,
                               bool preserve_case = true) {
    decode_html_entities_utf8(text.data(), NULL);
    if (reduce_len)
        text = reduce_lengthening(text, shorten_pattern);
    text = reduce_lengthening(text, bad_pattern);
    vector<string> words = find_word(text);
    if (!preserve_case) {
        for (int i = 0; i < words.size(); ++i) {
            cmatch cm;
            if (!regex_match(words[i], emoticons))
                for (int j = 0; j < words[i].length(); ++j) {
                    words[i][j] = tolower(words[i][j]);
                }
        }
    }
    return words;
}
