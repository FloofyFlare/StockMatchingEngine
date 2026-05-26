#pragma once
#include <cstdint>
#include <cstddef>

enum class Side : uint8_t { BUY, SELL };
enum class Ordertype : uint8_t { LIMIT, MARKET, IOC, FOK };

// CPUs don't fetch individual bytes from RAM — they fetch 64-byte chunks called cache lines
struct alignas(64) Order {
    // cache line 0 first half: hot fields (matching)
    int64_t     price_ticks;
    int64_t     quantity;
    Order*      next;
    Side        side;
    Ordertype   type;
    uint8_t     _pad[6];

    // cache line 0 second half: warm fields
    uint64_t    order_id;
    uint64_t    timestamp_ns;
    uint64_t    sequence_num;
    int64_t     original_quantity;

    // cache line 1: cold fields (non-matching)
    Order*      prev;
    uint8_t     _pad2[56];
};

// Compile-time assertions to ensure Order struct is optimized for cache performance
static_assert(sizeof(Order)           == 128,  "Order must be exactly 2 cache lines");
static_assert(alignof(Order)          == 64,   "Order must be cache-line aligned");
static_assert(offsetof(Order, order_id) == 32, "Hot fields must fit in first 32 bytes");



