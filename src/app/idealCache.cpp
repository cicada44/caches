#include <cassert>
#include <iostream>
#include <vector>

#include "../cache/cache.hpp"

int ret(int key) { return key; }

int main() {
    unsigned size = 0;
    unsigned elCnt = 0;
    std::vector<int> elems;

    std::cin >> size;
    caches::idealCache<int> cache(size);

    std::cin >> elCnt;
    int insertable;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable;
        assert(std::cin.good());
        elems.push_back(insertable);
    }
    std::cout << cache.lookup_update(elems, ret);
}
