cmake_minimum_required(VERSION 3.9)
project(NCFinal)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(
        main
        main.cpp
        word2vec.cpp
        process_raw_data.cpp
        stop_words.h
        html_decoder/entities.c
        median_of_distances.cpp
        median_of_distances.h
        global_constants.h
        pairwise_distance.cpp
        pairwise_distance.h
        calculate_tweet_vector.cpp
        calculate_tweet_vector.h
        tweet_tokenizer.cpp
        tweet_tokenizer.h
        effective_edges.cpp
        effective_edges.h
        effective_hashtags.cpp
        effective_hashtags.h
        graph.cpp graph.h
)
