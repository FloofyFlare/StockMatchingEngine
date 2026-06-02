#include "order_book.hpp"

void OrderBook::insert(Order* order) {
    
    if(order->side == Side::BUY){
        auto& levels = bids_;
        auto [it, inserted] = levels.emplace(order->price_ticks, PriceLevel{});
        PriceLevel& level = it->second;
        // If new price level is being added
        if(inserted) {
            level.price_ticks = order->price_ticks;
            level.head = level.tail = order;
            order->next = order->prev = nullptr;
        } else {
            order->prev         = nullptr;
            order->next         = level.tail;
            level.tail->next    = order;
            level.tail          = order;

            level.total_quantity += order->quantity;
            level.order_count++;
            order_id_map[order->order_id] = order;
        }
    } else{
        auto& levels = asks_;
        auto [it, inserted] = levels.emplace(order->price_ticks, PriceLevel{});
        PriceLevel& level = it->second;
        // If new price level is being added
        if(inserted) {
            level.price_ticks = order->price_ticks;
            level.head = level.tail = order;
            order->next = order->prev = nullptr;
        } else {
            order->prev         = nullptr;
            order->next         = level.tail;
            level.tail->next    = order;
            level.tail          = order;

            level.total_quantity += order->quantity;
            level.order_count++;
            order_id_map[order->order_id] = order;
        }
    }

}

bool OrderBook::cancel(uint64_t order_id) {
    auto it = order_id_map.find(order_id);
    if (it == order_id_map.end()) return false;

    Order* order = it->second;
    remove_order(order);
    order_id_map.erase(it);
    remove_level_if_empty(order->side, order->price_ticks);
    return true;
}

