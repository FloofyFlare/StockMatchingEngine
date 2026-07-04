#include "matching_engine.hpp"

void MatchingEngine::match_buy(Order* incoming, TradeEvent* out, int& count) {
    while (incoming->quantity > 0) {
        PriceLevel* best_ask = book_.best_ask();
        if (!best_ask) [[unlikely]]break; // No asks available
        if (incoming->price_ticks < best_ask->price_ticks) [[likely]] break;

        Order*  resting  = best_ask->head;
        int64_t fill_qty = std::min(incoming->quantity, resting->quantity);

        out[count++] = TradeEvent{
            .aggressor_id     = incoming->order_id,
            .resting_id       = resting->order_id,
            .price_ticks      = resting->price_ticks,  // trade prints at resting price
            .quantity         = fill_qty,
            .sequence_number  = incoming->sequence_number,
            .timestamp_ns     = incoming->timestamp_ns,
        };

        incoming->quantity          -= fill_qty;
        resting->quantity           -= fill_qty;
        best_ask->total_quantity    -= fill_qty;

        if (resting->quantity == 0) [[likely]] {
            book_.remove_order(resting);
            book_.remove_level_if_empty(Side::Sell, best_ask->price_ticks);
            pool_->release(resting); // return to pool — zero free() calls
        }
    }

    if (incoming->quantity > 0 && incoming->type == OrderType::Limit) {
        book_.insert(incoming);
    } else if (incoming->quantity > 0) {
        pool_->release(incoming); // IOC/Market: discard unfilled remainder
    }

}

bool MatchingEngine::can_fill_fok(const Order* incoming) const {
    int64_t available = 0;
    // Walk ask levels from best ask upward
    for (auto& [price, level] : /* asks */ ...) {
        if (incoming->price_ticks < price) break;
        available += level.total_quantity;
        if (available >= incoming->quantity) return true;
    }
    return false;
}
