#include "../src/cache/cache.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>

#include <fstream>

int retSame(int key)
{
    return key;
}

void print_2q(const caches::cache_t<int>& c)
{
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

TEST(Cache, Cache2Q_0)
{
    caches::cache_t<int> cache(10);

    ASSERT_EQ(cache.in_sz_, 10);
    ASSERT_EQ(cache.out_sz_, 10);
    ASSERT_EQ(cache.hot_sz_, 10);

    ASSERT_EQ(cache.full_in(), false);
    ASSERT_EQ(cache.full_out(), false);
    ASSERT_EQ(cache.full_hot(), false);

    for (int i = 0; i != 10; ++i) {
        ASSERT_EQ(cache.lookup_update(i, retSame), false);
    }
    for (int i = 0; i != 10; ++i) {
        ASSERT_EQ(cache.lookup_update(i, retSame), true);
    }

    int cnter = 9;
    for (auto i = cache.in_cache_.begin(); i != cache.in_cache_.end(); ++i) {
        ASSERT_EQ(i->first, cnter--);
    }

    ASSERT_EQ(cache.out_cache_.empty(), true);
    ASSERT_EQ(cache.hot_cache_.empty(), true);
}

TEST(Cache, Cache2Q_1)
{
    caches::cache_t<int> cache(5);

    ASSERT_EQ(cache.in_sz_, 5);
    ASSERT_EQ(cache.out_sz_, 5);
    ASSERT_EQ(cache.hot_sz_, 5);

    ASSERT_EQ(cache.full_in(), false);
    ASSERT_EQ(cache.full_out(), false);
    ASSERT_EQ(cache.full_hot(), false);

    for (int i = 0; i != 10; ++i) {
        if (i < 1) {
            ASSERT_EQ(cache.lookup_update(1, retSame), false);
        } else {
            ASSERT_EQ(cache.lookup_update(1, retSame), true);
        }
    }
    for (int i = 0; i != 10; ++i) {
        if (i < 1) {
            ASSERT_EQ(cache.lookup_update(2, retSame), false);
        } else {
            ASSERT_EQ(cache.lookup_update(2, retSame), true);
        }
    }

    int cnter = 2;
    for (auto i = cache.in_cache_.begin(); i != cache.in_cache_.end(); ++i) {
        ASSERT_EQ(i->first, cnter--);
    }

    ASSERT_EQ(cache.out_cache_.empty(), true);
    ASSERT_EQ(cache.hot_cache_.empty(), true);
}

TEST(Cache, Cache2Q_2)
{
    caches::cache_t<int> cache(0);

    ASSERT_EQ(cache.in_cache_.empty(), true);
    ASSERT_EQ(cache.out_cache_.empty(), true);
    ASSERT_EQ(cache.hot_cache_.empty(), true);

    ASSERT_EQ(cache.in_sz_, 0);
    ASSERT_EQ(cache.out_sz_, 0);
    ASSERT_EQ(cache.hot_sz_, 0);

    for (int i = 0; i != 10; ++i) {
        ASSERT_EQ(cache.lookup_update(i, retSame), false);
    }

    ASSERT_EQ(cache.in_cache_.empty(), true);
    ASSERT_EQ(cache.out_cache_.empty(), true);
    ASSERT_EQ(cache.hot_cache_.empty(), true);
}

/* Returns number of hits. */
size_t run_test(std::istream& is, std::ostream& os)
{
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

TEST(Cache2Q, CountOfHitTest)
{
    std::vector<int> expectedNHits{
            24, 6,  12, 11, 20, 9,  24, 16, 3,  1,  19, 29, 7,  18, 0,  2,  19,
            17, 14, 10, 10, 18, 12, 21, 14, 7,  15, 2,  17, 6,  2,  4,  12, 11,
            6,  11, 17, 16, 2,  13, 10, 0,  10, 5,  7,  5,  26, 10, 17, 0,  8,
            11, 3,  15, 17, 18, 10, 3,  2,  7,  10, 3,  11, 10, 7,  26, 10, 0,
            22, 4,  0,  7,  14, 25, 16, 9,  23, 0,  2,  0,  12, 5,  2,  15, 3,
            18, 0,  7,  2,  1,  26, 4,  17, 0,  4,  20, 11, 0,  0,  10};

    auto start = expectedNHits.begin();
    for (int j = 0; j != 100; ++j) {
        std::fstream ifs;
        
#if defined(__linux__)
        ifs.open(std::string("../../../test/dats/") + std::to_string(j) + std::string(".txt"));

#elif _WIN32
        ifs.open(std::string("..\\..\\..\\test\\dats\\") + std::to_string(j) + std::string(".txt"));

        ASSERT_EQ(run_test(ifs, std::cout), *start++);
#endif

    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
