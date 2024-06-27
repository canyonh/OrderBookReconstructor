#pragma once

#include "Orderbook/orders.h"
#include "Orderbook/price_level.h"

namespace Orderbook
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

private:
    // return ref since we assume it has been checked by caller
    inline StockOrderbook& GetStockOrderBook(Order::StockId stock_id_in)
    {
        return stock_order_books.find(stock_id_in)->second;
    }
    std::unordered_map<Order::StockId, StockOrderbook> stock_order_books;

};

}
