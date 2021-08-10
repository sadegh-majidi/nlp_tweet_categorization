//
// Created by sadegh on 09.08.21.
//

#ifndef NCFINAL_UTILS_H
#define NCFINAL_UTILS_H
#include <vector>
#include <tuple>
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>


template <class T>
int partition(std::vector<T> &arr, int low, int high, std::function<bool(T, T)> cmp) {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int random = low + (int)(rng() % (high - low + 1));
    T pivot = arr[random];

    int i = low - 1, j = high + 1;

    while (true) {

        do {
            i++;
        } while (cmp(arr[i], pivot) && i < high);

        do {
            j--;
        } while (cmp(pivot, arr[j]) && j > low);

        if (i >= j) {
            if(i == low && j == low && !cmp(pivot, arr[high]))
                return -1;
            return i;
        }

        std::swap(arr[i], arr[j]);
    }
}

template <class T>
void quick_sort(std::vector<T> &arr, int low, int high, std::function<bool(T, T)> cmp) {
    while (low < high) {
        int pi = partition<T>(arr, low, high, cmp);

        if (pi == -1) {
            return;
        }
        if (pi - low < high - pi) {
            quick_sort<T>(arr, low, pi - 1, cmp);
            low = pi;
        } else {
            quick_sort<T>(arr, pi, high, cmp);
            high = pi - 1;
        }
    }
}

#endif //NCFINAL_UTILS_H
