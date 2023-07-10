#include <cache/cache.hpp>
#include <cassert>
#include <iostream>
#include <vector>

int ret(int key) { return key; }

int main() {
    unsigned size, elCnt;
    std::vector<int> elems;

    size_t num_elements;
    std::cin >> num_elements;
    caches::idealCache<int> cache(num_elements);

    std::vector<int> elems;

    int insertable;
    size_t hits = 0;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable;
        assert(std::cin.good());
        elems.push_back(insertable);
    }
    std::cout << cache.lookup_update(elems, ret);
}
