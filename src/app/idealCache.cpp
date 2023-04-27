#include <cassert>
#include <iostream>
#include <vector>

#include "../cache/cache.hpp"

int ret(int key) { return key; }

int main() {
    size_t cache_size;
    std::cin >> cache_size;

    caches::idealCache<int> cache(cache_size);

    size_t num_elements;
    std::cin >> num_elements;

    std::vector<int> elems;

    int tmp_element = 0;
    for (size_t i = 0; i != num_elements; ++i) {
        std::cin >> tmp_element;
        elems.push_back(tmp_element);
    }

    std::cout << cache.lookup_update(elems, ret);
}
