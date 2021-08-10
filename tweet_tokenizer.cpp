#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <boost/config.hpp>
#include <boost/regex.hpp>

#include <unordered_set>

using namespace std;
string WORD_PATTERN;

string EMOTICONS = R"((?:[<>]?[:;=8][-o*']?[\)\]\([dDpP/:}{@|\\]|[\)\]\([dDpP/:}{@|\\][-o*']?[:;=8][<>]?|<3))";
//regex EMOTICONS(h);

regex emoticons(EMOTICONS);

string PHONE_NUMBER = R"((?:(?:\+?[01][*\-.\)]*)?(?:[\(]?\d{3}[ *\-.\)]*)?\d{3}[ *\-.\)]*\d{4}))";

string HTML_TAGS = "<[^>\\s]+>";

string ASCII_ARROWS = "[\\-]+>|<[\\-]+";

string TWITTER_USERNAME = "(?:@[\\w_]+)";

string TWITTER_HASHTAG = R"((?:\#+[\w_]+[\w\'_\-]*[\w_]+))";

string EMAIL_ADDRESS = R"([\w.+-]+@[\w-]+\.(?:[\w-]\.?)+[\w-])";

string WORD_WITH_DASH_AND_APOSTROPHE = R"((?:[^\W\d_](?:[^\W\d_]|['\-_])+[^\W\d_]))";

string NUMBERS = R"((?:[+\-]?\d+[,/.:-]\d+[+\-]?))";

string SIMPLE_WORD = "(?:[\\w_]+) ";

string ELLIPSIS = R"((?:\.(?:\s*\.){1,}) )";

string EXTRA = "(?:\\S) ";

string URLS = R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))";

string shorten_pattern = "(.)\\1{3,}";

string bad_pattern = "([^a-zA-Z0-9])\\1{3,}";


vector<string> WORD_RE{EMOTICONS, PHONE_NUMBER, HTML_TAGS, ASCII_ARROWS, TWITTER_HASHTAG, TWITTER_USERNAME,
                       EMAIL_ADDRESS, WORD_WITH_DASH_AND_APOSTROPHE, NUMBERS, SIMPLE_WORD, ELLIPSIS, EXTRA, URLS};


string reduce_lengthening(string text, const string &pattern) {
    regex repeat_regexp(pattern);
    using strmatch = std::match_results<std::string::iterator>;
    strmatch res;
    string result;
    int offset = 0;
    auto itr = text.begin();

    while (std::regex_search(itr, text.end(), res, repeat_regexp)) {
//        std::cout << "[" << res[0] << "]" << std::endl;
        string t = res[0];
        result += text.substr(offset, res.position()) + t[0] + t[0] + t[0];
        offset += res.position() + res.length();
        itr += res.position() + res.length();
    }
    result += text.substr(offset, text.length());
//    cout << text << "|" << '\n';
//    cout << result << "|";
    return result;

}

vector<string> find_word(string text, const regex &WORD_REGEX) {
    std::smatch m;
    vector<string> result;
    while (std::regex_search(text, m, WORD_REGEX)) {
        result.push_back(m[0]);
        text = m.suffix().str();
    }
    return result;
}


vector<string> tweet_tokenizer(const regex &WORD, string text, bool reduce_len = false,
                               bool preserve_case = true) {
    if (reduce_len)
        text = reduce_lengthening(text, shorten_pattern);
    text = reduce_lengthening(text, bad_pattern);
    vector<string> words = find_word(text, WORD);
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

std::string str = "8) XYZ111 d-dxxxxxxx GFT  ;((  <3   ://  GHY  :] XYZ222 t-nyyyyyYYyyy XYZ333 t-r jbhAAhhhh kjkjbhjgbj gfggggggggg yfyfy";


int main() {
    if (WORD_PATTERN.empty()) {
        for (int i = 0; i < WORD_RE.size(); ++i) {
            WORD_PATTERN += WORD_RE[i];
            if (i != WORD_RE.size() - 1)
                WORD_PATTERN += "|";
        }
    }
    regex WORD_REGEX(WORD_PATTERN);
//    find_word(str, WORD_REGEX);
    tweet_tokenizer(WORD_REGEX, str, true, false);
}