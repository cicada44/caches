// -----------------------------------------------------------------------------
//
// Header for class 'cache_t' and 'idealCache'
//
// -----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T, typename KeyT = int>
class cache_t {
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;

public:
    cache_t(const size_t sz) {
        const size_t remainder = sz % 3;
        if (remainder == 0) {
            in_sz_ = sz / 3;
            out_sz_ = sz / 3;
            hot_sz_ = sz / 3;
        } else if (remainder == 1) {
            in_sz_ = (sz / 3) + 1;
            out_sz_ = sz / 3;
            hot_sz_ = sz / 3;
        } else {
            in_sz_ = (sz / 3) + 1;
            out_sz_ = (sz / 3) + 1;
            hot_sz_ = sz / 3;
        }
    }

    size_t size() const { return in_sz_; }

    bool full_in() const { return (in_cache_.size() == in_sz_ && in_sz_ != 0); }

    bool full_out() const { return (out_cache_.size() == out_sz_ && out_sz_ != 0); }

    bool full_hot() const { return (hot_cache_.size() == hot_sz_ && hot_sz_ != 0); }

    /* Updates hot-cache. */
    template <typename F>
    bool lookup_update_hot(KeyT key, F slow_get_page) {
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
        if (eltit != hot_cache_.begin()) hot_cache_.splice(hot_cache_.begin(), hot_cache_, eltit, std::next(eltit));
        return true;
    }

    /* Updates out-cache. */
    template <typename F>
    bool lookup_update_out(KeyT key, F slow_get_page) {
        auto hit = out_hash_.find(key);

        if (hit == out_hash_.end()) {
            if (full_out()) {
                out_hash_.erase(out_cache_.back().first);
                out_cache_.pop_back();
            }
            out_cache_.emplace_front(in_cache_.back().first, slow_get_page(in_cache_.back().first));
            out_hash_.emplace(in_cache_.back().first, in_cache_.end());
            return false;
        }

        return true;
    }

    /* Updates whole cache structure. Returns true if cachehit */
    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page) {
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

private:
    size_t in_sz_, out_sz_, hot_sz_;
    std::list<std::pair<KeyT, T>> in_cache_;
    std::unordered_map<KeyT, ListIt> in_hash_;

    std::list<std::pair<KeyT, T>> out_cache_;
    std::unordered_map<KeyT, ListIt> out_hash_;

    std::list<std::pair<KeyT, T>> hot_cache_;
    std::unordered_map<KeyT, ListIt> hot_hash_;
};

template <typename T, typename KeyT = int>
class idealCache {
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    using MapIt = typename std::unordered_map<KeyT, size_t>::iterator;
    using elemRange = typename std::pair<KeyT, size_t>;

public:
    idealCache(const size_t sz) : sz_(sz) {}

    size_t size() const { return sz_; }

    bool full() const { return (cache_.size() == sz_); }

    void erase_farthest(elemRange& farthest) {
        cache_.erase(hash_.at(farthest.first));
        hash_.erase(farthest.first);
    }

    template <typename F>
    void add_to_cache(const KeyT& key, F slow_get_page) {
        cache_.emplace_front(std::make_pair(key, slow_get_page(key)));
        hash_.emplace(key, cache_.begin());
    }

    void decr_distances(size_t value) {
        for (auto& c : ranges_) {
            c.second -= value;
        }
    }

    elemRange get_farthest(const typename std::vector<KeyT>::const_iterator& actual, const typename std::vector<KeyT>::const_iterator& end) {
        elemRange farthest;
        size_t tmp_dist = 0;
        for (const auto& c : hash_) {
            tmp_dist = std::distance(actual, std::find(std::next(actual), end, c.first));
            if (tmp_dist > farthest.second) {
                farthest.first = c.first;
                farthest.second = tmp_dist;
            }
        }
        return farthest;
    }

    template <typename F>
    void update(elemRange& farthest, const KeyT& new_elem, F slow_get_page) {
        erase_farthest(farthest);
        add_to_cache(new_elem, slow_get_page);
    }

    template <typename F>
    size_t lookup_update(const std::vector<KeyT>& keys, F slow_get_page) {
        size_t hits = 0;
        elemRange farthest;
        size_t tmp_dist;

        for (auto actEl = keys.begin(); actEl != keys.end(); ++actEl) {
            tmp_dist = std::distance(actEl, std::find(std::next(actEl), keys.end(), *actEl));
            if (hash_.find(*actEl) == hash_.end()) {
                if (full()) {
                    farthest = get_farthest(actEl, keys.end());
                    if (tmp_dist < farthest.second) update(farthest, *actEl, slow_get_page);
                } else {
                    add_to_cache(*actEl, slow_get_page);
                    if (tmp_dist > farthest.second) {
                        farthest.first = *actEl;
                        farthest.second = tmp_dist;
                    }
                }
            } else {
                ++hits;
                ranges_[*actEl] = tmp_dist;
                if (tmp_dist > farthest.second) {
                    farthest.second = tmp_dist;
                }
            }
        }

        return hits;
    }

private:
    std::list<std::pair<KeyT, T>> cache_;
    std::unordered_map<KeyT, ListIt> hash_;
    std::unordered_map<KeyT, size_t> ranges_;

    size_t sz_;
};

}  // namespace caches