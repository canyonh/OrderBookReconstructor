#pragma once

#include "unordered_map"
#include "Orderbook/itch.h"
#include "Orderbook/side_book.h"

namespace Orderbook
{

struct Order
{
    using OrderId = Itch::Int8;
    using StockId = Itch::Int4;
    using Timestamp = Itch::Int8;
    using Volume = Itch::Int4;
    using Price = Itch::Int4; // fix point xxx.xxxx
    using UserRef = Itch::Int4;

    enum class Side : Itch::Alpha {Buy = 'B', Sell = 'S'};

    StockId stock_id = 0;
    OrderId order_id = 0;
    Timestamp time_stamp = 0;
    Side side = Side::Buy;
    Price price = 0;
    Volume volume = 0;
    UserRef user_ref = 0;

    bool operator==(const Order& rhs) const = default;
};

class Orders
{
public:
    bool Insert(const Order& order_in) { return orders.emplace(order_in.order_id, order_in).second; }
    void Remove(Order::OrderId order_id_in)
    {
        auto it = orders.find(order_id_in);
        if (it == orders.end())
            return;

        orders.erase(it);
    }
    Order* Find(Order::OrderId order_id_in)
    {
        auto it = orders.find(order_id_in);
        return it == orders.end() ? nullptr : &it->second;
    }

private:
    std::unordered_map<Order::OrderId, Order> orders;
};

class Orderbook
{
public:
    bool AddOrder(const Order& order_in);
    bool DeleteOrder(Order::OrderId order_id);
    bool CancelOrder(Order::OrderId order_id);
    bool ReplaceOrder(Order::OrderId old_order_id, Order::OrderId new_order_id, Order::Volume volume, Order::Price price);
    bool ExecuteOrder(Order::OrderId order_id, Order::Volume volume);
    bool ExecuteOrderWithPrice(Order::OrderId order_id, Order::Volume volume, Order::Price price);
private:
    Orders orders;
    Sidebook sell_book;
    Sidebook buy_book;
};

}
