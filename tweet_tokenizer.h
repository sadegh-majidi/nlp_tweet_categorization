//
// Created by sadegh on 12.08.21.
//

#ifndef NCFINAL_TWEET_TOKENIZER_H
#define NCFINAL_TWEET_TOKENIZER_H
#include <vector>
#include <string>

std::vector<std::string> tweet_tokenizer(std::string text, bool reduce_len, bool preserve_case);
void initialize_tweet_tokenizer();

#endif //NCFINAL_TWEET_TOKENIZER_H
