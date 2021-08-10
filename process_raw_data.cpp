#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <unordered_set>

using namespace std;

string file_address = "/home/solale/CLionProjects/nc-final-project/tweeter_data/near_washington_1015_1016_tweets.csv";


void write_csv(vector<vector<string>> data) {
    ofstream my_file("cleaned_data.csv");

    my_file << "date,content,id,hashtag,cleaned" << '\n';
    string d;

    for (auto &i : data) {

        for (int j = 0; j < i.size(); ++j) {
            d = i[j];
            if (d.find(',') != std::string::npos || d.find('\n') != std::string::npos )
                d = '\"' + d + '\"';
            if (j == i.size() - 1) {
                my_file << d << endl;
            } else
                my_file << d << ",";

        }
    }

    my_file.close();
}

vector<string> simple_tokenizer(string &s, int min_size = 0) {
    char delimiter = ',';
    vector<string> tokenized;
    std::string token;
    int last = -1, i;
    for (i = 0; i < (int) s.size(); i++)
        if (s[i] == delimiter && (s[last + 1] != '\"' || s[i - 1] == '\"')) {
            if (s[last + 1] == '\"')
                tokenized.push_back(s.substr(last + 2, i - last - 3));
            else
                tokenized.push_back(s.substr(last + 1, i - last - 1));
            last = i;
        }
    if (s[last + 1] == '\"' && i - last - 3 > 0)
        tokenized.push_back(s.substr(last + 2, i - last - 3));
    else if (i - last - 1 > 0)
        tokenized.push_back(s.substr(last + 1, i - last - 1));
    return tokenized;
}

vector<string> extend_data(vector<string> tokens, int size) {
    int extend_size = size - (int) tokens.size();
    if (extend_size < 0)
        extend_size = 0;
    for (int i = 0; i < extend_size; ++i) {
        tokens.emplace_back("");
    }
    return tokens;
}

map<string, int> index_map(string index_line) {
    map<string, int> indexes;
    vector<string> tokens = simple_tokenizer(index_line);
    for (int i = 0; i < tokens.size(); ++i) {
        indexes[tokens[i]] = i;
    }
    return indexes;
}

unordered_set<char> st;

string clean_content(string content) {
    static const char arr[] = {'#', '(', ')', ':', '\n', '\r', '!', '"', '$', '-', '&', '.', ':', '\'', ';', '*'};
    if (st.empty()) {
        for (char i : arr)
            st.insert(i);
    }
    regex tag_regex("@\\w+");
    regex num_regex("\\d+");

    regex url_regex(
            R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*))");
    string cleaned = std::move(content);
    cleaned = regex_replace(cleaned, url_regex, "");
    cleaned = regex_replace(cleaned, tag_regex, "");
    cleaned = regex_replace(cleaned, num_regex, "");

    string result;
    for (char &i : cleaned)
        if (st.find(i) == st.end())
            result += i;
    return result;
}

vector<string> hashtag_tokenizer(string &s, int min_size) {
    char delimiter = ',';
    vector<string> tokenized;
    std::string token;
    int last = -1, i;
    for (i = 0; i < (int) s.size(); i++)
        if (s[i] == delimiter && (s[last + 1] != '\"' || s[i - 1] == '\"')) {
            if (s[last + 1] == '\"') {
                token = s.substr(last + 2, i - last - 3);
                if (token.length() > min_size)
                    tokenized.push_back(token);
            } else {
                token = s.substr(last + 1, i - last - 1);
                if (token.length() > min_size)
                    tokenized.push_back(token);
            }
            last = i;
        }
    if (s[last + 1] == '\"' && i - last - 3 > 0) {
        token = s.substr(last + 2, i - last - 3);
        if (token.length() > min_size)
            tokenized.push_back(token);
    } else if (i - last - 1 > 0) {
        token = s.substr(last + 1, i - last - 1);
        if (token.length() > min_size)
            tokenized.push_back(token);
    }
    return tokenized;
}

void process_raw_data() {
    ifstream newfile(file_address);
    if (newfile.is_open()) {
        string tp;
        map<string, int> indexes;
        vector<string> tem_tokens;
        vector<string> tokens;
        vector<vector<string >> data;
        string content;
        static const string arr[] = {"date", "content", "id", "hashtag"};
        int i = 0;
        while (getline(newfile, tp)) {
            if (i == 0) {
                indexes = index_map(tp);
            } else {
                tokens = extend_data(simple_tokenizer(tp), indexes.size());
                vector<string> hashtags = hashtag_tokenizer(tokens[indexes["hashtag"]], 2);
                if (tokens[indexes["lang"]] == "en" && !hashtags.empty()) {
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
        newfile.close();
        write_csv(data);
    }
}

