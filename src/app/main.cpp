#include <cassert>
#include <iostream>

#include "../cache/cache.hpp"

int ret(int key) { return key; }

int main() {
    unsigned size;
    unsigned elCnt;

    std::cin >> size >> elCnt;
    assert(std::cin.good());

    caches::cache_t<int> cache(size);

    int insertable;
    size_t hits = 0;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable;
        assert(std::cin.good());
        hits += static_cast<size_t>(cache.lookup_update(insertable, ret));
    }

    std::cout << hits << '\n';
}
