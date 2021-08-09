#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>

using namespace std;

string file_address = "/home/solale/CLionProjects/nc-final-project/tweeter_data/near_washington_1015_1016_tweets.csv";

vector<string> simple_tokenizer(string s) {
    std::string delimiter = ",";
    vector<string> tokenized;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        tokenized.push_back(token);
    }
    return tokenized;
}

vector<string> extend_data(vector<string> tokens, int size) {
    int extend_size = size - tokens.size();
    if (extend_size < 0)
        extend_size = 0;
    for (int i = 0; i < extend_size; ++i) {
        tokens.emplace_back("");
    }
    return tokens;
}

map<string, int> index_map(string index_line) {
    map<string, int> indexes;
    vector<string> tokens = simple_tokenizer(std::move(index_line));
    for (int i = 0; i < tokens.size(); ++i) {
        indexes[tokens[i]] = i;
    }
    return indexes;
}

string clean_content(string content) {
    static const char arr[] = {'#', '(', ')', ':', '\n', '\r', '!', '"', '$', '-', '&', '.', ':', '\'', ';', '*'};
    regex tag_regex("@\\w+");
    regex num_regex("\\d+");

    regex url_regex(
            R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))");
    string cleaned = std::move(content);
    cleaned = regex_replace(cleaned, url_regex, "");
    cleaned = regex_replace(cleaned, tag_regex, "");
    cleaned = regex_replace(cleaned, num_regex, "");

    for (int i = 0; i < 16; ++i) {
        replace(cleaned.begin(), cleaned.end(), arr[i], ' ');
    }

    return cleaned;
}

void process_raw_data(){
    ifstream newfile(file_address);
    if (newfile.is_open()) {
        string tp;
        map<string, int> indexes;
        vector<string> tem_tokens;
        vector<string> tokens;
        vector<vector<string >> data;
        string content;
        static const string arr[] = {"hashtag", "content", "date", "id"};
        int i = 0;
        while (getline(newfile, tp)) {
            if (i == 0) {
                indexes = index_map(tp);
            } else {
                tokens = extend_data(simple_tokenizer(tp), indexes.size());
                if (tokens[indexes["lang"]] == "en" && tokens[indexes["hashtag"]].length() > 2) {
                    tem_tokens.clear();
                    for (int j = 0; j < 4; ++j) {
                        tem_tokens.push_back(tokens[indexes[arr[j]]]);
                    }
                    content = clean_content(tokens[indexes["content"]]);
                    tem_tokens.push_back(content);
                    data.push_back(tem_tokens);
                }
            }
            i++;
        }
    }
    newfile.close();
}

int main() {
    process_raw_data();
}