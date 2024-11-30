#include <random>
#include <gtest/gtest.h>

#include "quick_sort.h"
static int size = 10000;
static std::mt19937 gen1{};

TEST(parallel1, seq) {
    std::vector<int> v(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    std::ranges::shuffle(v, gen1);
    quick_sort_seq(sp);
    auto sortb = v;
    std::ranges::sort(sortb);
    ASSERT_EQ(v, sortb);
}

TEST(parallel1, par) {
    std::vector<int> v(size);
    std::vector<int> buff(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    auto sp_buff = std::span(buff);
    std::ranges::shuffle(v, gen1);
    quick_sort_par(sp, sp_buff, size / 4);
    auto sortb = v;
    std::ranges::sort(sortb);
    ASSERT_EQ(v, sortb);
}

TEST(parallel1, par_withoutcopy) {
    std::vector<int> v(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    std::ranges::shuffle(v, gen1);
    quick_sort_par_without_copy(sp, size / 4);
    auto sortb = v;
    std::ranges::sort(sortb);
    ASSERT_EQ(v, sortb);
}