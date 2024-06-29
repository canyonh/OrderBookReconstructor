#pragma once

#include <cassert>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include "Orderbook/orders.h"

namespace OrderbookReconstructor
{

class PriceLevel
{
public:
    struct OrderEntry
    {
        Order::Timestamp timestamp = 0;
        Order::OrderId order_id = 0;
        Order::Volume volume = 0;
    };

    void AddOrder(Order::Timestamp timestamp_in, Order::OrderId order_id_in, Order::Volume volume_in)
    {
        auto& table_entry = order_entries[timestamp_in];
        table_entry.emplace_back(timestamp_in, order_id_in, volume_in);
    }

    // we can lookup timestamp in orders
    void DeleteOrder(Order::Timestamp timestamp_in, Order::OrderId order_id_in)
    {
        // find the order entry, assertion should be enough for now
        // since it is considered as system error. 
        auto map_it = order_entries.find(timestamp_in);
        assert(map_it != order_entries.end());

        auto& entry_list = map_it->second;
        auto list_it = std::find_if(entry_list.begin(), entry_list.end(), [timestamp_in](const auto& elem) {
                return elem.timestamp == timestamp_in;
                });

        assert(list_it != entry_list.end());
        total_volume -= list_it->volume;
        entry_list.erase(list_it);
    }

private:

    // find an order entry so we can modify it
    OrderEntry* FindOrder(Order::Timestamp timestamp_in, Order::OrderId id_in)
    {
        auto map_it = order_entries.find(timestamp_in);
        assert(map_it != order_entries.end());

        auto& entry_list = map_it->second;
        auto list_it = std::find_if(entry_list.begin(), entry_list.end(), [timestamp_in](const auto& elem) {
                return elem.timestamp == timestamp_in;
                });

        return list_it == entry_list.end() ? nullptr : &(*list_it);
    }

    Order::Volume total_volume = 0;

    // use map since we need timestamp to be sorted
    std::map<Order::Timestamp, std::list<OrderEntry>> order_entries;
};

class StockOrderbook 
{
public:
    StockOrderbook(Order::Side side_in)
        : side(side_in)
    {}

    // Remove all orders and return all the order_id that is better than
    // target price
    bool SweepBook(Order::Price target_price, std::vector<Order::OrderId>& collected_orders)
    {
        assert(!"@write me");
    }

    // should create a price level if not exist
    // should assert if two order has the same id
    void AddOrder(const Order& order_in)
    {
        auto& price_level = GetPriceLevel(order_in.price);
        price_level.AddOrder(order_in.time_stamp, order_in.order_id, order_in.volume);
    }

    void DeleteOrder(const Order::OrderId& order_id_in, Order::Price price_in, Order::Timestamp timestamp_in)
    {
        auto& price_level = GetPriceLevel(price_in);
        price_level.DeleteOrder(order_id_in, timestamp_in);
    }

    void CancelOrder(const Order::OrderId& order_id_in, Order::Price price_in, Order::Timestamp timestamp_in, Order::Volume vol_in)
    {
        auto& price_level = GetPriceLevel(price_in);
        price_level.DeleteOrder(order_id_in, timestamp_in);
    }
private:
    PriceLevel& GetPriceLevel(Order::Price price_in)
    {
        return price_levels[price_in];
    }

    Order::Side side;
    std::unordered_map<Order::Price, PriceLevel> price_levels;
};


} // namespace OrderbookReconstructor
