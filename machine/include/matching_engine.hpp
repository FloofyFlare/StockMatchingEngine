#pragma once
#include "order_book.hpp"
#include "event.hpp"
#include "order_pool.hpp"

static constexpr int MAX_TRADES_PER_ORDER = 10;

class MatchingEngine {
    public:
        void process(Order* incomming, TradeEvent* trades_out, int& trade_count);

        void set_pool(OrderPool* pool) {
            pool_ = pool;
        }

    private:
        OrderBook book_;
        OrderPool* pool_ = nullptr;

        void match_buy (Order* incoming, TradeEvent* out , int& count);
        void match_sell(Order* incoming, TradeEvent* out , int& count);

        /* Trades are all or nothing, so we need to check if we
        can fill the entire order before executing any trades */
        bool can_fill_fok(const Order* incoming) const;

};





