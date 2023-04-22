#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>

#include "../src/cache/cache.hpp"

int retSame(int key) { return key; }

// void print_2q(const caches::cache_t<int>& c) {
//     std::cout << "IN\n";
//     for (auto i = c.in_cache_.begin(); i != c.in_cache_.end(); ++i) {
//         std::cout << i->first << ' ';
//     }
//     std::cout << '\n';
//     std::cout << "OUT\n";
//     for (auto i = c.out_cache_.begin(); i != c.out_cache_.end(); ++i) {
//         std::cout << i->first << ' ';
//     }
//     std::cout << '\n';
//     std::cout << "HOT\n";
//     for (auto i = c.hot_cache_.begin(); i != c.hot_cache_.end(); ++i) {
//         std::cout << i->first << ' ';
//     }
//     std::cout << '\n';
// }

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
    for (int j = 0; j != 15; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".dat"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".txt"));
#endif
        // ASSERT_EQ(ifs.is_open(), true);

        std::cout << run_test(ifs) << '\n';
    }
}

TEST(CacheIdeal, CountOfHitTest) {
    for (int j = 0; j != 15; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".dat"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".txt"));
#endif
        // ASSERT_EQ(ifs.is_open(), true);

        if (!ifs.is_open()) {
            continue;
        }

        std::cout << run_test_ideal(ifs) << '\n';

        ifs.close();
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
