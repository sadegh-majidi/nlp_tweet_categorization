//
// Created by sadegh on 09.08.21.
//

#include <tuple>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <chrono>
#include <random>


std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

bool cmp(std::tuple<int, int> a, std::tuple<int, int> b) {
    return std::get<1>(a) < std::get<1>(b);
}

int partition(std::vector<std::tuple<int, int>> &list_of_tuples, int low, int high) {
    int random = low + (int)(rng() % (high - low + 1));

    swap(list_of_tuples[random], list_of_tuples[low]);

    std::tuple<int, int> pivot = list_of_tuples[low];

    int i = low - 1, j = high + 1;

    while (true) {

        do {
            i++;
        } while (cmp(list_of_tuples[i], pivot) && i < high);

        do {
            j--;
        } while (!cmp(list_of_tuples[j], pivot) && j > low);

        if (i >= j) {
            if(i == low && j == low)
                return -1;
            return i;
        }

        std::swap(list_of_tuples[i], list_of_tuples[j]);
    }
}


void quick_sort(std::vector<std::tuple<int, int>> &arr, int low, int high) {

    while (low < high) {
        int pi = partition(arr, low, high);

        if (pi == -1) {
            return;
        }
        if (pi - low < high - pi) {
            quick_sort(arr, low, pi - 1);
            low = pi;
        } else {
            quick_sort(arr, pi, high);
            high = pi - 1;
        }
    }

}

void sort_tuple(std::vector<std::tuple<int, int>> &list_of_tuples) {
    quick_sort(list_of_tuples, 0, (int) list_of_tuples.size() - 1);
}

double get_median(std::vector<double> l) {
    std::nth_element(l.begin(), l.begin() + l.size() / 2, l.end());
    return l[l.size() / 2];
}


