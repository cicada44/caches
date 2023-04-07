#include "../cache/cache.hpp"

#include <cassert>
#include <iostream>
#include <vector>

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

    std::vector<int> insertable;
    int insertable_tmp;
    for (size_t i = 0; i != elCnt; ++i) {
        std::cin >> insertable_tmp;
        assert(std::cin.good());
        insertable.push_back(insertable_tmp);
    }

    size_t hits = cache.lookup_update(insertable, ret);

    std::cout << hits << '\n';
}
