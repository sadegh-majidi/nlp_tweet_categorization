cmake_minimum_required (VERSION 2.8.11)
project (NCFinal)
set (CMAKE_CXX_STANDARD 17)
add_executable (
        main
        main.cpp
        word2vec.cpp
        process_raw_data.cpp
        stop_words.h
)