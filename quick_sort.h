#pragma once
#include <span>
#include <vector>
#include <random>
#include <parlay/primitives.h>
static std::mt19937 gen{};

template<typename T>
static void quick_sort_impl_seq(std::span<T> vec) {
    using std::swap;
    if (vec.size() <= 1) {
        return;
    }
    std::uniform_int_distribution<size_t> rnd(0, vec.size() - 1);
    size_t x = rnd(gen);
    swap(vec[vec.size() - 1], vec[x]);
    size_t i = 0;
    for (size_t j = 0; j < vec.size(); j++) {
        if (vec[j] < vec[vec.size() - 1]) {
            swap(vec[i], vec[j]);
            i++;
        }
    }
    swap(vec[vec.size() - 1], vec[i]);
    quick_sort_impl_seq(vec.subspan(0, i));
    quick_sort_impl_seq(vec.subspan(i + 1, vec.size() - i - 1));
}

template<typename T>
void quick_sort_seq(std::span<T> &vec) {
    return quick_sort_impl_seq(vec);
}

template<typename T>
static void quick_sort_impl_par(std::span<T> vec, std::span<T> buff, size_t limit) {
    if (vec.size() <= limit) {
        quick_sort_seq(vec);
        return;
    }
    std::uniform_int_distribution<size_t> rnd(0, vec.size() - 1);
    parlay::random_generator pargen;
    size_t x = vec[rnd(pargen)];
    size_t lower_s = parlay::filter_into_uninitialized(vec, buff, [&x](auto &el) { return el < x; });
    size_t middle_s = parlay::filter_into_uninitialized(vec, buff.subspan(lower_s), [&x](auto &el) { return el == x; });
    parlay::filter_into_uninitialized(vec, buff.subspan(lower_s + middle_s), [&x](auto &el) { return el > x; });
    std::copy(buff.begin(), buff.end(), vec.begin());
    parlay::par_do(
        [&]() {
            quick_sort_impl_par(vec.subspan(0, lower_s), buff.subspan(0, lower_s), limit);
        },
        [&]() {
            quick_sort_impl_par(vec.subspan(lower_s + middle_s), buff.subspan(lower_s + middle_s), limit);
        }
    );
}

template<typename T>
void quick_sort_par(std::span<T> &vec, std::span<T> &buff, size_t limit) {
    return quick_sort_impl_par(vec, buff, limit);
}

template<typename T>
static void quick_sort_impl_par_without_copy(std::span<T> vec, size_t limit) {
    using std::swap;
    if (vec.size() <= limit) {
        quick_sort_seq(vec);
        return;
    }
    parlay::random_generator pargen;
    std::uniform_int_distribution<size_t> rnd(0, vec.size() - 1);
    size_t x = rnd(pargen);
    swap(vec[vec.size() - 1], vec[x]);
    size_t i = 0;
    for (size_t j = 0; j < vec.size(); j++) {
        if (vec[j] < vec[vec.size() - 1]) {
            swap(vec[i], vec[j]);
            i++;
        }
    }
    swap(vec[vec.size() - 1], vec[i]);
    parlay::par_do(
        [&]() { quick_sort_impl_par_without_copy(vec.subspan(0, i), limit); },
        [&]() { quick_sort_impl_par_without_copy(vec.subspan(i + 1, vec.size() - i - 1), limit); });
}

template<typename T>
void quick_sort_par_without_copy(std::span<T> &vec, size_t limit) {
    return quick_sort_impl_par_without_copy(vec, limit);
}
