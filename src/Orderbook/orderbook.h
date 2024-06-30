#pragma once

#include "unordered_map"
#include "src/Orderbook/itch.h"
#include "src/Orderbook/orders.h"
#include "src/Orderbook/side_book.h"

namespace OrderbookReconstructor
{


class Orderbook
{
public:
    bool AddOrder(const Order& order_in);
    bool DeleteOrder(Order::OrderId order_id);
    bool CancelOrder(Order::OrderId order_id, Order::Volume vol_in);
    bool ReplaceOrder(Order::OrderId old_order_id, Order::OrderId new_order_id, Order::Volume volume, Order::Price price);
    bool ExecuteOrder(Order::OrderId order_id, Order::Volume volume);
    bool ExecuteOrderWithPrice(Order::OrderId order_id, Order::Volume volume, Order::Price price);

    inline auto& GetSidebook(Order::Side side_in)
    {
        return side_in == Order::Side::Buy ? buy_book : sell_book;
    }
private:
    void DeleteOrder(const Order& order_in);

    Orders orders;
    Sidebook sell_book;
    Sidebook buy_book;
};

}
