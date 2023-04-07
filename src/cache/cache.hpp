#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T, typename KeyT = int>
struct cache_t {
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;

    size_t in_sz_, out_sz_, hot_sz_;
    std::list<std::pair<KeyT, T>> in_cache_;
    std::unordered_map<KeyT, ListIt> in_hash_;

    std::list<std::pair<KeyT, T>> out_cache_;
    std::unordered_map<KeyT, ListIt> out_hash_;

    std::list<std::pair<KeyT, T>> hot_cache_;
    std::unordered_map<KeyT, ListIt> hot_hash_;

    cache_t(const size_t sz) : in_sz_(sz), out_sz_(sz), hot_sz_(sz)
    {
    }

    size_t size() const
    {
        return in_sz_;
    }

    bool full_in() const
    {
        return (in_cache_.size() == in_sz_);
    }

    bool full_out() const
    {
        return (out_cache_.size() == out_sz_);
    }

    bool full_hot() const
    {
        return (hot_cache_.size() == hot_sz_);
    }

    template <typename F>
    bool lookup_update_hot(KeyT key, F slow_get_page)
    {
        auto hit = hot_hash_.find(key);

        if (hit == hot_hash_.end()) {
            if (full_hot()) {
                hot_hash_.erase(hot_cache_.back().first);
                hot_cache_.pop_back();
            }
            hot_cache_.emplace_front(key, slow_get_page(key));
            hot_hash_.emplace(key, hot_cache_.begin());
            return false;
        }

        auto eltit = hit->second;
        if (eltit != hot_cache_.begin())
            hot_cache_.splice(
                    hot_cache_.begin(), hot_cache_, eltit, std::next(eltit));
        return true;
    }

    template <typename F>
    bool lookup_update_out(KeyT key, F slow_get_page)
    {
        auto hit = out_hash_.find(key);

        if (hit == out_hash_.end()) {
            if (full_out()) {
                out_hash_.erase(out_cache_.back().first);
                out_cache_.pop_back();
            }
            out_cache_.emplace_front(
                    in_cache_.back().first,
                    slow_get_page(in_cache_.back().first));
            out_hash_.emplace(in_cache_.back().first, in_cache_.end());
            return false;
        }

        return true;
    }

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page)
    {
        if (in_sz_ == 0) {
            return 0;
        }

        auto hit_in = in_hash_.find(key);
        if (hit_in == in_hash_.end()) {
            bool hitted = false;
            if (full_in()) {
                hitted = lookup_update_out(key, slow_get_page);
                if (hitted == false) {
                    in_hash_.erase(in_cache_.back().first);
                    in_cache_.pop_back();
                } else {
                    lookup_update_hot(key, slow_get_page);
                    return hitted;
                }
            }
            in_cache_.emplace_front(key, slow_get_page(key));
            in_hash_.emplace(key, in_cache_.begin());
            return hitted;
        }

        return true;
    }
};

template <typename T, typename KeyT = int>
struct idealCache {
    std::vector<KeyT> cache_;

    size_t sz_;

    std::map<size_t, KeyT> usage_;

    idealCache(const size_t sz) : sz_(sz)
    {
    }

    size_t size() const
    {
        return sz_;
    }

    bool full() const
    {
        return (cache_.size() == sz_);
    }

    void fill_usage(const std::vector<KeyT>& keys)
    {
        for (auto i = keys.begin(); i != keys.end(); ++i) {
            usage_.emplace(
                    std::pair(std::count(keys.begin(), keys.end(), *i), *i));
        }

        for (auto i = usage_.rbegin(); i != usage_.rend(); ++i) {
            std::cout << "usage: " << i->first << " key: " << i->second << '\n';
        }
    }

    template <typename F>
    size_t lookup_update(const std::vector<KeyT>& keys, F slow_get_page)
    {
        usage_.clear();
        cache_.clear();

        fill_usage(keys);

        size_t inputEls = (keys.size() < sz_) ? keys.size() : sz_;
        size_t hits = 0;
        auto usageIt = usage_.rbegin();
        for (size_t i = 0; i != inputEls; ++i) {
            cache_.push_back((usageIt++)->second);
        }

        std::cout << "inputEls: " << inputEls << " size - " << cache_.size()
                  << '\n';

        for (const auto& c : cache_) {
            std::cout << c << '\n';
        }

        for (const auto& c : keys) {
            if (std::find(cache_.begin(), cache_.end(), c) != cache_.end()) {
                ++hits;
            }
        }

        return hits;
    }
};

} // namespace caches