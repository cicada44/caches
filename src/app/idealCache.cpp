#include "../cache/cache.hpp"

#include <cassert>
#include <iostream>
#include <vector>

int ret(int key) { return key; }

int main() {
    unsigned size, elCnt;
    std::vector<int> elems;

    std::cin >> size >> elCnt;
    assert(std::cin.good());

    caches::idealCache<int> cache(size);

    int insertable;
    size_t hits = 0;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable;
        assert(std::cin.good());
        elems.push_back(insertable);
    }

    hits = cache.lookup_update(elems, ret);

    std::cout << hits << '\n';
}
