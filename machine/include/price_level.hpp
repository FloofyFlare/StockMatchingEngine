#pragma once
#include "order.hpp"

struct alignas(64) PriceLevel {
    int64_t price_ticks;
    int64_t total_quantity;
    Order* head;
    Order* tail;
    uint32_t order_count;
    uint8_t _pad[28];
};

static_assert(sizeof(PriceLevel) == 64, "PriceLevel must be exactly one cache line");