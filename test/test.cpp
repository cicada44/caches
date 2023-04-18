#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>

#include "../src/cache/cache.hpp"

int retSame(int key) { return key; }

void print_2q(const caches::cache_t<int>& c) {
    std::cout << "IN\n";
    for (auto i = c.in_cache_.begin(); i != c.in_cache_.end(); ++i) {
        std::cout << i->first << ' ';
    }
    std::cout << '\n';
    std::cout << "OUT\n";
    for (auto i = c.out_cache_.begin(); i != c.out_cache_.end(); ++i) {
        std::cout << i->first << ' ';
    }
    std::cout << '\n';
    std::cout << "HOT\n";
    for (auto i = c.hot_cache_.begin(); i != c.hot_cache_.end(); ++i) {
        std::cout << i->first << ' ';
    }
    std::cout << '\n';
}

size_t run_test(std::istream& is) {
    size_t cache_size;
    is >> cache_size;

    caches::cache_t<int> cache(cache_size);

    size_t num_elements;
    is >> num_elements;

    size_t num_hits = 0;

    int tmp_element = 0;
    for (size_t i = 0; i != num_elements; ++i) {
        is >> tmp_element;
        num_hits += cache.lookup_update(tmp_element, retSame);
    }

    return num_hits;
}

size_t run_test_ideal(std::istream& is) {
    size_t cache_size;
    is >> cache_size;

    caches::idealCache<int> cache(cache_size);

    size_t num_elements;
    is >> num_elements;

    size_t num_hits = 0;

    std::vector<int> elems;

    int tmp_element = 0;
    for (size_t i = 0; i != num_elements; ++i) {
        is >> tmp_element;
        elems.push_back(tmp_element);
    }

    return cache.lookup_update(elems, retSame);
}

TEST(Cache2Q, CountOfHitTest) {
    int hits = 0;
    int sum = 0;
    std::vector<int> expectedNHits{9, 3, 6, 6, 11, 5, 11, 6, 1, 1, 8, 12, 7, 7, 0, 0,  7, 7, 4, 4, 2, 4, 6, 9,  4, 5, 8,  0, 7, 3, 1, 3, 6, 5, 3, 5, 8, 8, 1, 3, 4,  0, 5,  3, 5, 3,  13, 6, 6, 0,
                                   5, 8, 1, 7, 10, 6, 6,  1, 2, 4, 5, 2,  6, 4, 3, 15, 5, 0, 9, 2, 0, 5, 5, 14, 3, 4, 11, 0, 1, 0, 6, 2, 2, 8, 1, 3, 0, 3, 2, 1, 10, 0, 12, 0, 3, 10, 6,  0, 0, 5};

    auto start = expectedNHits.begin();
    for (int j = 0; j != 100; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".txt"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".txt"));
#endif
        ASSERT_EQ(ifs.is_open(), true);

        ASSERT_EQ(run_test(ifs), *start++);
    }
    std::cout << hits << '\n';
}

TEST(CacheIdeal, CountOfHitTestNormal) {
    int sum = 0, hits = 0;
    std::vector<int> expectedNHits{26, 6,  12, 11, 28, 11, 28, 18, 3,  1, 25, 35, 7,  26, 0,  2,  23, 23, 22, 12, 23, 26, 12, 28, 23, 9, 21, 2,  19, 7,  2, 4,  16, 12,
                                   8,  11, 28, 23, 2,  18, 11, 0,  17, 5, 7,  5,  32, 10, 22, 0,  12, 11, 3,  20, 27, 20, 10, 3,  2,  7, 10, 3,  12, 10, 7, 34, 10, 0,
                                   31, 4,  0,  7,  17, 29, 27, 11, 24, 0, 2,  0,  23, 5,  2,  24, 3,  28, 0,  7,  2,  1,  32, 4,  22, 0, 4,  30, 11, 0,  0, 10};

    auto start = expectedNHits.begin();
    for (int j = 0; j != 100; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".txt"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".txt"));
#endif
        ASSERT_EQ(ifs.is_open(), true);

        ASSERT_EQ(run_test_ideal(ifs), *start++);
    }
    std::cout << hits << '\n';
}

TEST(CacheIdeal, CountOfHitTestBig) {
    int sum = 0;
    std::vector<int> expectedNHits{9999, 4995, 5, 49995, 149990, 65};

    auto start = expectedNHits.begin();
    for (int j = 0; j != 6; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/datsIdealCache/") + std::to_string(j) + std::string(".txt"));
#elif _WIN32
        ifs.open(std::string("test\\datsIdealCache\\") + std::to_string(j) + std::string(".txt"));
#endif
        ASSERT_EQ(ifs.is_open(), true);

        ASSERT_EQ(run_test_ideal(ifs), *start++);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
