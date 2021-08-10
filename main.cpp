#include <iostream>
#include <chrono>
#include "word2vec.h"

using namespace std;
const char *sentence[] = {"You", "should", "never", "take", "advice", "from", "someone", "who", "thinks", "red",
                          "paint", "dries", "quicker", "than", "blue", "paint", "His", "ultimate", "dream", "fantasy",
                          "consisted", "of", "being", "content", "and", "sleeping", "eight", "hours", "in", "a", "row",
                          "I", "often", "see", "the", "time", "on", "clocks", "There", "can",
                          "never", "be", "too", "many", "cherries", "on", "an", "ice", "cream", "sundae", "The",
                          "shark-infested", "South", "Pine", "channel", "was", "the", "only", "way", "in", "or", "out",
                          "Kevin", "embraced", "his", "ability", "to", "be", "at", "the", "wrong", "place", "at", "the",
                          "wrong", "time", "They", "desperately", "needed", "another", "drummer", "since", "the",
                          "current", "one", "only", "knew", "how", "to", "play", "bongos", "Three", "years", "later",
                          "the", "coffin", "was", "still", "full", "of", "Jello", "She", "is", "never", "happy",
                          "until", "she", "finds", "something", "to", "be", "unhappy", "about", "then", "she", "is",
                          "overjoyed", "Twin", "slept", "in", "the", "shade", "of", "the", "palm",
                          "tree", "while", "the", "mother", "tanned", "in", "the", "sun", "My", "uncles", "favorite",
                          "pastime", "was", "building", "cars", "out", "of", "noodles", "25", "years", "later", "she",
                          "still", "regretted", "that", "specific", "moment", "He", "hated", "that", "he", "loved",
                          "what", "she", "hated", "about", "hate", "Nobody", "loves", "a", "pig", "wearing", "lipstick",
                          "Jason", "understand", "why", "his", "parents", "let",
                          "him", "sell", "his", "little", "sister", "at", "the", "garage", "sale", "The", "tree",
                          "fell", "unexpectedly", "short", "It", "was", "a", "slippery", "slope", "and", "he", "was",
                          "willing", "to", "slide", "all", "the", "way", "to", "the", "deepest", "depths", "A", "suit",
                          "of", "armor", "provides", "excellent", "sun", "protection", "on", "hot", "days", "Iguanas",
                          "were", "falling", "out", "of", "the", "trees", "He", "drank", "life", "before", "spitting",
                          "it", "out", "Video", "Player", "is", "loading"};

int main() {
    auto y = chrono::high_resolution_clock::now();
    word2vec_setup();
    cout << "HERE" << endl;
    y = chrono::high_resolution_clock::now();
    for (auto word : sentence) {
//        cout << word << endl;
        auto x = get_vector(word);
        if(x.empty()) {
            cout << word << endl;
        }
    }
    cout << std::chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now() - y).count() / 1e9 << endl;
    cout << "Starting...";
    return 0;
}
