//
// Created by mahdi on 13.08.21.
//

#ifndef NCFINAL_GLOBAL_CONSTANTS_H
#define NCFINAL_GLOBAL_CONSTANTS_H

static const int VECTOR_COLUMNS = 300;
static const char *absolute_path_to_resources = "/home/mahdi/Desktop/nc-final-project/files_twitter/";
static const char *raw_data_file_names[][2] = {
        {"near_washington_1015_1016_tweets.csv", "cleaned_near_washington_1015_1016_tweets.csv"},
        {"near_washington_1016_1017_tweets.csv", "cleaned_near_washington_1016_1017_tweets.csv"},
        {"near_washington_1017_1018_tweets.csv", "cleaned_near_washington_1017_1018_tweets.csv"},
        {"near_washington_1018_1019_tweets.csv", "cleaned_near_washington_1018_1019_tweets.csv"},
        {"near_washington_1019_1022_tweets.csv", "cleaned_near_washington_1019_1022_tweets.csv"}
};
static const char *word2vec_crawl_file_path = "/hdd/crawl-300d-2M.vec";
static const int pairwise_distance_threads = 8;
static const char *pairwise_distance_output_directory_path = "/hdd/pairwise_distances/";

#endif //NCFINAL_GLOBAL_CONSTANTS_H
