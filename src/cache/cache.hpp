#pragma once

#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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
};

template <typename T, typename KeyT = int>
struct idealCache {
    using VecIt = typename std::vector<std::pair<KeyT, T>>::iterator;

    std::vector<std::pair<KeyT, T>> cache_;
    std::unordered_map<KeyT, VecIt> hash_;
    std::pair<VecIt, size_t> farthest_, pre_farthest_;

    size_t sz_;

    idealCache(const size_t sz) : sz_(sz) {}

    size_t size() const { return sz_; }

    bool full() const { return (cache_.size() == sz_); }

    void print_cache() {
        std::cout << "CACHE\n";
        for (const auto& c : cache_) {
            std::cout << c.first << ' ';
        }
        std::cout << "\n\n";
    }

    template <typename F>
    size_t lookup_update(const std::vector<KeyT>& keys, F slow_get_page) {
        size_t hits = 0;
        size_t dist_next_el = 0;

        for (auto actualEl = keys.begin(); actualEl != keys.end(); ++actualEl) {
            dist_next_el = std::distance(actualEl, std::find(std::next(actualEl), keys.end(), *actualEl));
            if (hash_.find(*actualEl) == hash_.end()) {
                if (full()) {
                    if (dist_next_el < farthest_.second) {
                        cache_.erase(farthest_.first);
                        hash_.erase(farthest_.first->first);
                        cache_.push_back(std::make_pair(*actualEl, slow_get_page(*actualEl)));
                        hash_.emplace(*actualEl, std::prev(cache_.end()));
                        farthest_ = pre_farthest_;
                    }
                } else {
                    cache_.push_back(std::make_pair(*actualEl, slow_get_page(*actualEl)));
                    hash_.emplace(*actualEl, std::prev(cache_.end()));
                    if (dist_next_el > farthest_.second) {
                        pre_farthest_ = farthest_;
                        farthest_ = std::make_pair(std::prev(cache_.end()), dist_next_el);
                    }
                }
            } else {
                std::cout << "HIT INTO: " << *actualEl << '\n';
                ++hits;
                if (dist_next_el > farthest_.second) {
                    // pre_farthest_ = farthest_;
                    farthest_.second = dist_next_el;
                } else {
                    farthest_ = pre_farthest_;
                }
            }
            std::cout << "cache size - " << cache_.size() << '\n';
            print_cache();
            std::cout << "HITS: " << hits << '\n';
            std::cout << "FARTHEST: " << farthest_.first->first << '\n';
            std::cout << "PREFARTHEST: " << pre_farthest_.first->first << '\n';
            sleep(1);
            system("clear");
            --farthest_.second;
        }

        return hits;
    }
};

}  // namespace caches