#pragma once

#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace caches
{

template <typename T, typename KeyT = int>
struct cache_t
{
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;

    size_t in_sz_, out_sz_, hot_sz_;
    std::list<std::pair<KeyT, T>> in_cache_;
    std::unordered_map<KeyT, ListIt> in_hash_;

    std::list<std::pair<KeyT, T>> out_cache_;
    std::unordered_map<KeyT, ListIt> out_hash_;

    std::list<std::pair<KeyT, T>> hot_cache_;
    std::unordered_map<KeyT, ListIt> hot_hash_;

    cache_t(const size_t sz) : in_sz_(sz), out_sz_(sz), hot_sz_(sz) {}

    size_t size() const { return in_sz_; }

    bool full_in() const { return (in_cache_.size() == in_sz_); }

    bool full_out() const { return (out_cache_.size() == out_sz_); }

    bool full_hot() const { return (hot_cache_.size() == hot_sz_); }

    template <typename F>
    bool lookup_update_hot(KeyT key, F slow_get_page)
    {
        auto hit = hot_hash_.find(key);

        if (hit == hot_hash_.end())
        {
            if (full_hot())
            {
                hot_hash_.erase(hot_cache_.back().first);
                hot_cache_.pop_back();
            }
            hot_cache_.emplace_front(key, slow_get_page(key));
            hot_hash_.emplace(key, hot_cache_.begin());
            return false;
        }

        auto eltit = hit->second;
        if (eltit != hot_cache_.begin()) hot_cache_.splice(hot_cache_.begin(), hot_cache_, eltit, std::next(eltit));
        return true;
    }

    template <typename F>
    bool lookup_update_out(KeyT key, F slow_get_page)
    {
        auto hit = out_hash_.find(key);

        if (hit == out_hash_.end())
        {
            if (full_out())
            {
                out_hash_.erase(out_cache_.back().first);
                out_cache_.pop_back();
            }
            out_cache_.emplace_front(in_cache_.back().first, slow_get_page(in_cache_.back().first));
            out_hash_.emplace(in_cache_.back().first, in_cache_.end());
            return false;
        }

        return true;
    }

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page)
    {
        if (in_sz_ == 0)
        {
            return 0;
        }

        auto hit_in = in_hash_.find(key);
        if (hit_in == in_hash_.end())
        {
            bool hitted = false;
            if (full_in())
            {
                hitted = lookup_update_out(key, slow_get_page);
                if (hitted == false)
                {
                    in_hash_.erase(in_cache_.back().first);
                    in_cache_.pop_back();
                }
                else
                {
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
struct idealCache
{
    using vecIt = typename std::vector<std::pair<KeyT, T>>::iterator;
    using mapIt = typename std::map<KeyT, size_t>::const_iterator;
    using elemRange = typename std::pair<KeyT, size_t>;

    std::vector<std::pair<KeyT, T>> cache_;
    std::map<KeyT, size_t> ranges_;

    size_t sz_;

    idealCache(const size_t sz) : sz_(sz) {}

    size_t size() const { return sz_; }

    bool full() const { return (cache_.size() == sz_); }

    void remove_farthest(mapIt oldRangeIt)
    {
        ranges_.erase(oldRangeIt);
        cache_.erase(std::find_if(cache_.begin(), cache_.end(),
                                  [&oldRangeIt](const elemRange &el)
                                  {
                                      return (el.first == oldRangeIt->first);
                                  }));
    }

    template <typename F>
    size_t lookup_update(const std::vector<KeyT> &keys, F slow_get_page)
    {
        size_t hits = 0;

        for (auto i = keys.cbegin(); i != keys.cend(); ++i)
        {
            if (std::find_if(cache_.cbegin(), cache_.cend(),
                             [&i](const std::pair<KeyT, T> &p)
                             {
                                 return (p.first == *i);
                             }) == cache_.cend())
            {
                if (full())
                {
                    remove_farthest(std::max_element(ranges_.cbegin(), ranges_.cend(),
                                                     [](const elemRange &p1, const elemRange &p2)
                                                     {
                                                         return (p1.second < p2.second);
                                                     }));
                }
                cache_.push_back(std::make_pair(*i, slow_get_page(*i)));
            }
            else
            {
                ++hits;
            }

            ranges_[*i] = std::distance(i, std::find(std::next(i), keys.end(), *i));

            for (auto &p : ranges_)
            {
                --p.second;
            }
        }

        return hits;
    }
};

}  // namespace caches