cmake_minimum_required (VERSION 3.9)
project (NCFinal)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
add_executable (
        main
        main.cpp
        word2vec.cpp
        process_raw_data.cpp
        utils.cpp
        utils.h
        stop_words.h
        median_of_distances.cpp
        median_of_distances.h
        global_constants.h
        pairwise_distance.cpp
        pairwise_distance.h
)