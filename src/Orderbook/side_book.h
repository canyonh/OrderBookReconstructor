#pragma once

#include "Orderbook/orders.h"
#include "Orderbook/price_level.h"

namespace OrderbookReconstructor
{

class Sidebook
{
public:

    void SweepBook(Order::StockId stock_id_in, 
                   Order::Price target_price_in,
                   std::vector<Order::OrderId>& collected_orders_in)
    {
        auto& stock_order_book = GetStockOrderBook(stock_id_in);
        stock_order_book.SweepBook(target_price_in, collected_orders_in);
    }

    void AddOrder(const Order& order_in)
    {
        GetStockOrderBook(order_in.stock_id).AddOrder(order_in);
    }

    void DeleteOrder(const Order::OrderId& order_id_in, Order::Price price_in, Order::Timestamp timestamp_in)
    {
        GetStockOrderBook(order_id_in).DeleteOrder(order_id_in, price_in, timestamp_in);
    }

    void CancelOrder(const Order::OrderId& order_id_in, Order::Price price_in, Order::Timestamp timestamp_in, Order::Volume vol_in)
    {
        GetStockOrderBook(order_id_in).CancelOrder(order_id_in, price_in, timestamp_in, vol_in);
    }

private:
    // return ref since we assume it has been checked by caller
    inline StockOrderbook& GetStockOrderBook(Order::StockId stock_id_in)
    {
        return stock_order_books.find(stock_id_in)->second;
    }
    std::unordered_map<Order::StockId, StockOrderbook> stock_order_books;

};

}
