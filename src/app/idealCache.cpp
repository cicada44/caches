#include "../cache/cache.hpp"

#include <cassert>
#include <iostream>

int ret(int key)
{
    return key;
}

int main()
{
    unsigned size, elCnt;

    std::cin >> size >> elCnt;
    assert(std::cin.good());

    caches::idealCache<int> cache(size);

    int insertable;
    size_t hits = 0;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable;
        assert(std::cin.good());
        hits += cache.lookup_update(insertable, ret);
    }

    std::cout << hits << '\n';
}
