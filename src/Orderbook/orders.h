#pragma once

#include "Orderbook/itch.h"
#include <unordered_map>

namespace Orderbook
{

struct Order
{
    using OrderId = Itch::Int8;
    using StockId = Itch::Int4;
    using Timestamp = Itch::Int8;
    using Volume = Itch::Int4;
    using Price = float;
    using UserRef = Itch::Int4;

    enum class Side : Itch::Alpha {Buy = 'B', Sell = 'S'};

    StockId stock_id;
    OrderId order_id;
    Timestamp time_stamp;
    Side side;
    Price price;
    Volume volume;
    UserRef user_ref;

    bool operator==(const Order& rhs) const = default;
};

class Orders
{
public:
    bool AddOrder(const Order& order_in);
    bool RemoveOrder(Order::OrderId order_id_in);
    bool UpdateOrder(const Order& new_orde_in);
    bool ExecuteOrder(Order::OrderId order_id_in);

private:
    std::unordered_map<Itch::Order::OrderId, Itch::Order> orders;
};

};
