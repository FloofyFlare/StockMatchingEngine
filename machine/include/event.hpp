#pragma once
#include "symbol.hpp"
#include <cstdint>


enum class InboundType : uint8_t { NewOrder, Cancel, Replace };

struct InboundEvent {
    InboundType type;
    SymbolId    symbol_id;
    uint64_t    order_id;
    uint64_t    client_id;
    uint64_t    price_ticks;
    int64_t     quantity;
    Side        side;
    OrderType   order_type;
};

struct TradeEvent {
    uint64_t    aggressor_id;
    uint64_t    resting_id;
    uint64_t    price_ticks;
    int64_t     quantity;
    uint64_t    sequence_number;
    uint64_t    timestamp_ns;
};

struct AckEvent {
    uint64_t    order_id;
    uint64_t    sequence_number;
    bool        accepted;
    char        reject_reason[15]; // fixed-size reason string for simplicity
};

struct BookUpdateEvent {
    SymbolId    symbol_id;
    uint64_t    bid_price;
    uint64_t    ask_price;
    uint64_t    bid_quantity;
    uint64_t    ask_quantity;
    uint64_t    sequence_number;
};

static_assert(sizeof(TradeEvent) <= 64);
static_assert(sizeof(AckEvent) <= 64);
static_assert(sizeof(BookUpdateEvent) <= 64);