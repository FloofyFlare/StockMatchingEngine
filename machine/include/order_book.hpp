#pragma once
#include "price_level.hpp"
#include <map>
#include <unordered_map>
#include "order.hpp"

class  OrderBook {
    public:
        void        insert(Order*order);
        bool        cancel(uint64_t order_id);
        PriceLevel* best_bid();
        PriceLevel* best_ask();
        void        remove_order(Order* order);
        void        remove_level_if_empty(Side side, int64_t price_ticks);
    private:

        // Currently using a red-black tree 
        // will cache miss on every level traversal 
        // replace with a flat array for possibly for first levels for better cache locality
        std::map<int64_t, PriceLevel, std::greater<int64_t>> bids_; // highest first
        std::map<int64_t, PriceLevel, std::less<int64_t>>    asks_; // lowest first

        // For O(1) order cancellation and updates
        // should replace with flat_hash_map for better chache locality
        std::unordered_map<uint64_t, Order*> order_id_map; // order_id -> Order*
};
