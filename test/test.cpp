#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>

#include "../src/cache/cache.hpp"

int retSame(int key) { return key; }

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
    std::vector<int> expected{0, 4, 3, 31, 89999};
    auto expected_elem = expected.begin();

    for (int j = 0; j != 5; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".dat"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".dat"));
#endif
        ASSERT_EQ(ifs.is_open(), true);

        ASSERT_EQ(run_test(ifs), *expected_elem++);

        ifs.close();
    }
}

TEST(CacheIdeal, CountOfHitTest) {
    std::vector<int> expected{0, 4, 7, 65, 89999, 4117, 89999, 99900, 990};
    auto expected_elem = expected.begin();

    for (int j = 0; j != 9; ++j) {
        std::ifstream ifs;

#if defined(__linux__)
        ifs.open(std::string("test/dats/") + std::to_string(j) + std::string(".dat"));
#elif _WIN32
        ifs.open(std::string("test\\dats\\") + std::to_string(j) + std::string(".dat"));
#endif
        ASSERT_EQ(ifs.is_open(), true);

        ASSERT_EQ(run_test_ideal(ifs), *expected_elem++);

        ifs.close();
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
